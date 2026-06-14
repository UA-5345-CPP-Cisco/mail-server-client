# EmailListModel

## Overview

`EmailListModel` is a custom `QAbstractListModel` implementation designed for displaying email messages in QML applications. It provides efficient pagination and role-based data access for seamless UI integration.

---

## ✨ Key Features

### Data Storage
- **Starred Status** - Track favorited emails
- **Subject/Theme** - Email subject line
- **Sender Name** - Name of the email sender
- **Message Preview** - Short preview of email content
- **Timestamp** - Email date and time

### Pagination
- Fixed page size: **8 emails per page**
- Navigation methods:
  - `NextPage()` - Move to the next page
  - `PrevPage()` - Move to the previous page
  - `SetPage(int page)` - Jump to a specific page
- Automatic UI notifications on page or page count changes

### Additional Features
- Dynamic insertion of email entries
- Exposes data to QML through custom roles
- Reactive updates for ListView components

---

## 📋 Roles Reference

| Role | Type | Description |
|------|------|-------------|
| `StarredRole` | `bool` | Indicates whether the email is starred/favorited |
| `ThemeRole` | `QString` | Email subject/theme line |
| `NameRole` | `QString` | Sender's name |
| `PreviewRole` | `QString` | Short preview of the email body |
| `TimeRole` | `QString` | Email timestamp (formatted) |

---

## 🎯 Purpose

This model is intended for use with QML views such as `ListView`, providing an efficient and scalable way to display large email collections while keeping only a subset of items visible through pagination.

---

## 💡 Usage Example

```qml
ListView {
    model: emailListModel
    delegate: EmailItem {
        starred: model.StarredRole
        subject: model.ThemeRole
        sender: model.NameRole
        preview: model.PreviewRole
        timestamp: model.TimeRole
    }
}
```

---

## 🔄 Navigation Example

```cpp
// Move to next page
emailListModel->NextPage();

// Move to previous page
emailListModel->PrevPage();

// Jump to page 3
emailListModel->SetPage(3);
```
