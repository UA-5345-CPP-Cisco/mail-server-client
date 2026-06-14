# Email List Model Architecture

## Overview

This document describes the three-layer architecture for displaying and managing email messages in a Qt/QML application. The system uses **EmailListModel** as the primary data source, **EmailFilterProxy** to filter emails by folder type (Inbox, Sent, Starred, Draft), and **EmailPageProxy** to implement pagination with fixed page sizes.

---

## Architecture Layers

### Layer 1: EmailListModel (Data Source)
### Layer 2: EmailFilterProxy (Filtering)
### Layer 3: EmailPageProxy (Pagination)

The chain flows as follows:
```
EmailListModel (all emails) 
    ↓
EmailFilterProxy (filtered by folder type)
    ↓
EmailPageProxy (current page only)
    ↓
QML ListView (displays 8 emails per page)
```

---

## 1. EmailListModel

### Purpose
**EmailListModel** is the foundational `QAbstractListModel` that stores all email data in memory. It acts as the primary data source and does not perform any filtering or pagination itself.

### Data Structure

```cpp
struct EmailData {
    bool is_starred;      // Whether email is favorited
    bool is_sent;         // Whether email is in Sent folder
    bool is_draft;        // Whether email is a draft
    QString theme;        // Email subject line
    QString name;         // Sender's name
    QString sendTo;       // Recipient name/email
    QString preview;      // Auto-truncated preview of content
    QString content;      // Full email body
    QString time;         // Email timestamp (formatted)
};
```

### Available Roles

The model exposes the following Qt roles for QML binding:

| Role | QML Name | Type | Description |
|------|----------|------|-------------|
| `StarredRole` | `emailsStarred` | bool | Email is favorited |
| `SentRole` | `emailsSent` | bool | Email is in Sent folder |
| `DraftRole` | `emailsDraft` | bool | Email is a draft |
| `ThemeRole` | `emailsTheme` | QString | Email subject |
| `NameRole` | `emailsName` | QString | Sender's name |
| `SendToRole` | `emailsSendTo` | QString | Recipient |
| `PreviewRole` | `emailsPreview` | QString | Truncated content preview |
| `ContentRole` | `emailsContent` | QString | Full email body |
| `TimeRole` | `emailsTime` | QString | Formatted timestamp |

### Public Methods

#### `AddData(const QString& theme, const QString& name, ...)`
Adds a new email entry to the model. Automatically generates a preview by truncating the content to 30 characters and uses the current time if no timestamp is provided.

```cpp
model->AddData(
    true,                           // is_starred
    false,                          // is_sent
    false,                          // is_draft
    "Project Update",               // theme
    "john@example.com",             // name
    "recipient@company.com",        // sendTo
    "Here is the full email content that will be truncated...",
    ""                              // time (auto-generated)
);
```

#### `AddData(const EmailData& item)`
Adds a pre-constructed `EmailData` struct. Used internally by the overloaded method above.

### Key Features

- **Raw Data Storage**: Stores all emails without filtering or pagination
- **Dynamic Growth**: Uses `std::vector<EmailData>` for efficient memory management
- **Auto Preview Generation**: `makePreview()` truncates content at word boundaries
- **Qt Signals**: Emits `dataAdded()` signal when new emails are inserted
- **Proper Model Integration**: Correctly calls `beginInsertRows()` and `endInsertRows()` to notify QML views

### Usage in QML

```qml
import QtQuick
import QtQuick.Controls

ListView {
    model: emailListModel  // Direct binding to EmailListModel
    delegate: Column {
        Text { text: model.emailsTheme }
        Text { text: model.emailsName }
        Text { text: model.emailsPreview }
    }
}
```

---

## 2. EmailFilterProxy

### Purpose
**EmailFilterProxy** is a `QSortFilterProxyModel` that filters emails based on folder type. It sits between the `EmailListModel` and the UI, showing only emails that match the current folder criteria.

### Folder Types

```cpp
enum FolderType {
    Inbox,   // Emails not sent and not draft
    Sent,    // Emails with is_sent=true
    Starred, // Emails with is_starred=true
    Draft    // Emails with is_draft=true
};
```

### Filtering Logic

The `filterAcceptsRow()` method determines which rows pass through the filter:

| FolderType | Condition | Result |
|-----------|-----------|--------|
| `Inbox` | `!is_sent && !is_draft` | Shows received/normal emails |
| `Sent` | `is_sent == true` | Shows sent emails only |
| `Starred` | `is_starred == true` | Shows favorited emails only |
| `Draft` | `is_draft == true` | Shows draft emails only |

### Implementation Details

```cpp
bool EmailFilterProxy::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
    const bool is_sent    = src->data(idx, SentRole).toBool();
    const bool is_draft   = src->data(idx, DraftRole).toBool();
    const bool is_starred = src->data(idx, StarredRole).toBool();
    
    switch (m_type) {
        case Inbox:   return !is_sent && !is_draft;
        case Sent:    return  is_sent;
        case Starred: return  is_starred;
        case Draft:   return  is_draft;
    }
    return true;
}
```

### Usage Example

```cpp
// Create model with all emails
EmailListModel* listModel = new EmailListModel(this);

// Add sample data
listModel->AddData(true, false, false, "Subject 1", "Alice", "Bob", "Content...", "");
listModel->AddData(false, true, false, "Subject 2", "Carol", "Dave", "Content...", "");

// Create filter for Inbox folder
EmailFilterProxy* inboxFilter = new EmailFilterProxy(EmailFilterProxy::Inbox, this);
inboxFilter->setSourceModel(listModel);

// Create filter for Sent folder
EmailFilterProxy* sentFilter = new EmailFilterProxy(EmailFilterProxy::Sent, this);
sentFilter->setSourceModel(listModel);

// Use in QML
engine.rootContext()->setContextProperty("inboxModel", inboxFilter);
engine.rootContext()->setContextProperty("sentModel", sentFilter);
```

### QML Integration

```qml
ListView {
    model: inboxModel  // Uses EmailFilterProxy
    delegate: EmailItem { ... }
}

// Switch folders by changing the proxy
onFolderSelected: {
    if (folder === "Sent") {
        listView.model = sentModel
    }
}
```

### Key Characteristics

- **Non-destructive**: Original data in `EmailListModel` is unchanged
- **Dynamic Filtering**: Filter re-applies automatically when source data changes
- **Efficient**: Only recalculates when necessary
- **Chainable**: Can be used as the source for another proxy (like `EmailPageProxy`)

---

## 3. EmailPageProxy

### Purpose
**EmailPageProxy** is a `QSortFilterProxyModel` that implements pagination on top of filtered data. It shows only the emails for the current page (8 emails per page by default).

### Key Properties

```cpp
Q_PROPERTY(int currentPage READ currentPage NOTIFY currentPageChanged)
Q_PROPERTY(int pageCount   READ pageCount   NOTIFY pageCountChanged)
Q_PROPERTY(QString emailsCount READ emailsCount NOTIFY emailsCountChanged)
```

| Property | Type | Description |
|----------|------|-------------|
| `currentPage` | int | Currently displayed page (0-indexed) |
| `pageCount` | int | Total number of pages |
| `emailsCount` | QString | Total number of emails in current filter |

### Public Methods

#### `nextPage()`
Advances to the next page if available.

```cpp
pageProxy->nextPage();  // Moves to page+1 if possible
```

#### `prevPage()`
Goes back to the previous page if possible.

```cpp
pageProxy->prevPage();  // Moves to page-1 if possible
```

### How Pagination Works

The system displays exactly **8 emails per page**:

```
Total emails: 25
Page 0: emails 0-7
Page 1: emails 8-15
Page 2: emails 16-23
Page 3: email 24

pageCount = ceil(25 / 8) = 4 pages (0-3)
```

### Filtering Logic

```cpp
bool EmailPageProxy::filterAcceptsRow(int sourceRow, const QModelIndex&) const
{
    int start = m_current_page * m_per_page;      // Start row for current page
    int end   = start + m_per_page - 1;            // End row for current page
    return sourceRow >= start && sourceRow <= end;
}
```

Only rows within the current page's range are shown.

### State Management

The proxy automatically:

1. **Recalculates on data changes**: Listens to `rowsInserted`, `rowsRemoved`, `modelReset`, `layoutChanged` signals
2. **Updates email count**: Tracks total filtered emails
3. **Updates page count**: Recalculates based on total emails and page size
4. **Clamps page index**: If current page exceeds max, resets to last valid page
5. **Emits signals**: Notifies QML of changes to `currentPage`, `pageCount`, `emailsCount`

### Usage Example

```cpp
// Create chain: EmailListModel → EmailFilterProxy → EmailPageProxy
EmailListModel* listModel = new EmailListModel(this);
EmailFilterProxy* filterProxy = new EmailFilterProxy(EmailFilterProxy::Inbox, this);
EmailPageProxy* pageProxy = new EmailPageProxy(this);

// Set up chain
filterProxy->setSourceModel(listModel);
pageProxy->setSourceModel(filterProxy);

// Add data
listModel->AddData(...);  // Add multiple emails

// Navigate pages
pageProxy->nextPage();     // Go to page 1
pageProxy->nextPage();     // Go to page 2
pageProxy->prevPage();     // Go back to page 1
```

### QML Integration

```qml
import QtQuick
import QtQuick.Controls

ListView {
    id: emailList
    model: pageProxy  // Uses EmailPageProxy
    delegate: EmailDelegate { ... }
}

Row {
    Button {
        text: "< Previous"
        enabled: pageProxy.currentPage > 0
        onClicked: pageProxy.prevPage()
    }
    
    Text {
        text: `Page ${pageProxy.currentPage + 1} of ${pageProxy.pageCount} (${pageProxy.emailsCount} total)`
    }
    
    Button {
        text: "Next >"
        enabled: pageProxy.currentPage < pageProxy.pageCount - 1
        onClicked: pageProxy.nextPage()
    }
}
```

---

## Complete Data Flow Example

### Setup Phase

```cpp
// 1. Create EmailListModel (stores all emails)
EmailListModel* emailList = new EmailListModel(this);

// 2. Create EmailFilterProxy (filters by folder)
EmailFilterProxy* inboxFilter = new EmailFilterProxy(EmailFilterProxy::Inbox, this);
inboxFilter->setSourceModel(emailList);

// 3. Create EmailPageProxy (paginates filtered results)
EmailPageProxy* pageProxy = new EmailPageProxy(this);
pageProxy->setSourceModel(inboxFilter);

// Expose to QML
engine.rootContext()->setContextProperty("emailPageProxy", pageProxy);
```

### Adding Emails

```cpp
// Add 25 emails to the model
for (int i = 0; i < 25; ++i) {
    emailList->AddData(
        i % 3 == 0,           // is_starred
        i % 5 == 0,           // is_sent
        i % 7 == 0,           // is_draft
        QString("Subject %1").arg(i),
        QString("sender%1@mail.com").arg(i),
        "recipient@mail.com",
        QString("Email body content %1...").arg(i),
        ""                    // auto-generate time
    );
}
```

### Automatic Chain Reaction

When emails are added:

1. **EmailListModel** stores them in `m_data` vector
2. **EmailFilterProxy** applies folder filter (e.g., Inbox only)
3. **EmailPageProxy** calculates page count and shows only page 0 (rows 0-7)
4. **QML ListView** receives update and displays 8 emails
5. **Properties update**: `pageCount = 4`, `emailsCount = 20`, `currentPage = 0`

### User Navigation

```
User clicks "Next" button
    ↓
pageProxy->nextPage() is called
    ↓
currentPage changes from 0 to 1
    ↓
filterAcceptsRow() now returns rows 8-15
    ↓
QML ListView updates with new emails
    ↓
currentPageChanged() signal emitted
    ↓
UI updates: "Page 2 of 4"
```

---

## 🎯 Key Design Patterns

### 1. Proxy Chain Pattern
Each proxy wraps the previous layer without modifying data:
```
Source → Filter → Paginate → UI
```

### 2. Lazy Filtering
Filtering happens only when needed (when data changes or page navigation occurs).

### 3. Reactive Updates
All properties use Qt's signal/slot mechanism for automatic UI updates.

### 4. Separation of Concerns
- **EmailListModel**: Data storage only
- **EmailFilterProxy**: Filtering logic only
- **EmailPageProxy**: Pagination logic only

---

## 🔧 Customization

### Changing Page Size

Edit in `emailpageproxy.h`:
```cpp
static constexpr int m_per_page = 8;  // Change to desired size
```

### Adding New Folder Types

1. Add to enum in `emailfilterproxy.h`:
   ```cpp
   enum FolderType { Inbox, Sent, Starred, Draft, Archive };  // Add Archive
   ```

2. Add filter logic in `emailfilterproxy.cpp`:
   ```cpp
   case Archive: return is_archived;  // New role needed in EmailData
   ```

### Adding New Email Properties

1. Extend `EmailData` struct in `emaillistmodel.h`
2. Add new role to `EmailRole` enum
3. Handle in `data()` method's switch statement
4. Add to `roleNames()` mapping

---

## 📊 Performance Considerations

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| Add email | O(1) | Append to vector |
| Filter rows | O(n) | Checks each row's flags |
| Paginate | O(1) | Simple range check |
| Page navigation | O(n) | Rebuilds view for new range |

For applications with thousands of emails, consider:
- Lazy loading from database
- Virtual scrolling (only render visible rows)
- Background filtering on worker thread

---

## 🐛 Common Issues & Solutions

### Issue: Changes to source model don't update pagination

**Cause**: `EmailPageProxy` didn't connect signals properly

**Solution**: Ensure `setSourceModel()` is called after proxy creation

### Issue: Page navigation shows unexpected emails

**Cause**: Filter conditions are incorrect

**Solution**: Verify `filterAcceptsRow()` logic in `EmailFilterProxy`

### Issue: Memory grows unbounded

**Cause**: Continuously adding emails without limits

**Solution**: Implement email removal or implement lazy loading from database

---

## Summary

The three-class architecture provides a clean, maintainable solution for displaying paginated, filtered email lists in Qt/QML:

- **EmailListModel**: Core data storage (all emails)
- **EmailFilterProxy**: Folder filtering (Inbox, Sent, Starred, Draft)
- **EmailPageProxy**: Pagination (8 emails per page)

Each layer is independent, reusable, and follows Qt best practices for model-view architecture.
