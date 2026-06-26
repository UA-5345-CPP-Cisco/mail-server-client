# Server Interface Documentation

This document lists the client interfaces that collect or persist data intended for server-side use.

Important:

- In the current client codebase, there is no active network transport layer for these actions.
- Most "send" operations currently write to the local SQLite database and update UI state.
- The QML account login and registration screens emit signals, but the actual server authentication logic is not
  implemented yet.

## 1. Message composition

### `ISXMail::MessageComposer::SendMessage`

- Declared in [`headers/mail/messagecomposer.h`](headers/mail/messagecomposer.h)
- Implemented in [`models/mail/messagecomposer.cpp`](models/mail/messagecomposer.cpp)

Signature:

```cpp
bool SendMessage(
    const QString& sender_name,
    const QString& sender_email,
    const QString& recipient_email,
    const QString& subject,
    const QString& body,
    bool is_inbox = false
);
```

Input:

- `sender_name`: currently unused.
- `sender_email`: sender address stored with the message.
- `recipient_email`: required.
- `subject`: optional.
- `body`: required.
- `is_inbox`: controls the inbox flag on the stored message.

Current behavior:

- Rejects empty recipient or empty body.
- Inserts a row into `mail_messages` with status `sent`.
- Inserts one recipient row into `message_recipients` with status `pending`.
- Returns `true` only after the local database transaction commits.

Notes:

- This is the main interface used when the user clicks `Send`
  in [`screens/navigation/new_message/NewMessageQML.qml`](screens/navigation/new_message/NewMessageQML.qml).

### `ISXMail::MessageComposer::SaveDraft`

- Declared in [`headers/mail/messagecomposer.h`](headers/mail/messagecomposer.h)
- Implemented in [`models/mail/messagecomposer.cpp`](models/mail/messagecomposer.cpp)

Signature:

```cpp
bool SaveDraft(
    const QString& sender_name,
    const QString& sender_email,
    const QString& recipient_email,
    const QString& subject,
    const QString& body
);
```

Input:

- `sender_name`: currently unused.
- `sender_email`: sender address stored with the draft.
- `recipient_email`: optional.
- `subject`: optional.
- `body`: optional.

Current behavior:

- Rejects the call only if recipient, subject, and body are all empty after trimming.
- Inserts a row into `mail_messages` with status `draft`.
- Inserts a recipient row only when `recipient_email` is not empty.
- Returns `true` only after the local database transaction commits.

Notes:

- Used when a compose window is closed with unsent content
  in [`screens/navigation/new_message/NewMessageQML.qml`](screens/navigation/new_message/NewMessageQML.qml).

## 2. Message mutations exposed to QML

### `ISXMail::EmailListModel::SetStarred`

- Declared in [`headers/mail/emaillistmodel.h`](headers/mail/emaillistmodel.h)
- Implemented in [`models/mail/emaillistmodel.cpp`](models/mail/emaillistmodel.cpp)

Signature:

```cpp
bool SetStarred(int row, bool starred);
```

Input:

- `row`: row index in the list model.
- `starred`: desired star state.

Current behavior:

- Updates the local SQLite `mail_messages.is_starred` field when the row has a persisted message id.
- Updates the in-memory model and emits `dataChanged`.
- Returns `false` for invalid rows or database update failure.

Notes:

- Called from the email list UI when the user toggles the star icon.

### `ISXMail::EmailListModel::DeleteEmail`

- Declared in [`headers/mail/emaillistmodel.h`](headers/mail/emaillistmodel.h)
- Implemented in [`models/mail/emaillistmodel.cpp`](models/mail/emaillistmodel.cpp)

Signature:

```cpp
bool DeleteEmail(int row);
```

Input:

- `row`: row index in the list model.

Current behavior:

- Deletes the message from the local database when it has a persisted id.
- Removes the row from the model.
- Returns `false` for invalid rows or database delete failure.

### `ISXMail::EmailListModel::RemoveData`

- Declared in [`headers/mail/emaillistmodel.h`](headers/mail/emaillistmodel.h)
- Implemented in [`models/mail/emaillistmodel.cpp`](models/mail/emaillistmodel.cpp)

Signature:

```cpp
void RemoveData(int row);
```

Current behavior:

- Same local delete path as `DeleteEmail`, but void-returning.
- Used by proxy layers when the UI removes an item through a filtered or paged model.

### `ISXMail::EmailListModel::AddData`

- Declared in [`headers/mail/emaillistmodel.h`](headers/mail/emaillistmodel.h)
- Implemented in [`models/mail/emaillistmodel.cpp`](models/mail/emaillistmodel.cpp)

Signature:

```cpp
void AddData(
    bool is_starred,
    bool is_sent,
    bool is_draft,
    const QString& theme,
    const QString& name,
    const QString& send_to,
    const QString& content,
    const QString& time,
    bool is_inbox = false
);
```

Current behavior:

- Creates an in-memory row only.
- Does not write to the database directly.

Notes:

- Used by compose and draft flows after the local save/send operation succeeds.

## 3. Proxy-layer message mutation helpers

### `ISXMail::EmailPageProxy::setEmailData`

- Declared in [`headers/mail/emailpageproxy.h`](headers/mail/emailpageproxy.h)
- Implemented in [`models/mail/emailpageproxy.cpp`](models/mail/emailpageproxy.cpp)

Signature:

```cpp
bool setEmailData(int proxyRow, const QVariant& value, int role);
```

Current behavior:

- Maps a proxy row to the source model and forwards `setData`.
- Used for model edits originating from the paged UI.

### `ISXMail::EmailPageProxy::setStarred`

- Declared in [`headers/mail/emailpageproxy.h`](headers/mail/emailpageproxy.h)
- Implemented in [`models/mail/emailpageproxy.cpp`](models/mail/emailpageproxy.cpp)

Signature:

```cpp
bool setStarred(int proxyRow, bool starred);
```

Current behavior:

- Convenience wrapper over `setEmailData(..., StarredRole)`.

### `ISXMail::EmailPageProxy::removeEmailData`

- Declared in [`headers/mail/emailpageproxy.h`](headers/mail/emailpageproxy.h)
- Implemented in [`models/mail/emailpageproxy.cpp`](models/mail/emailpageproxy.cpp)

Signature:

```cpp
void removeEmailData(int proxyRow);
```

Current behavior:

- Maps through proxy layers and deletes the underlying message row.

## 4. Account selection and profile state

### `ISXCurrentUser::CurrentUser::authorize`

- Declared in [`headers/users/currentuser.h`](headers/users/currentuser.h)
- Implemented in [`models/users/currentuser.cpp`](models/users/currentuser.cpp)

Signature:

```cpp
void authorize(const QString& username, const QString& email, const QString& avatar_path = "");
```

Input:

- `username`: display name for the active account.
- `email`: active account email.
- `avatar_path`: optional avatar path.

Current behavior:

- Updates the in-memory current-user profile.
- Optionally copies the avatar to the application data directory.
- Emits profile and authorization change signals.
- Does not contact a server.

### `ISXCurrentUser::CurrentUser::logout`

- Declared in [`headers/users/currentuser.h`](headers/users/currentuser.h)
- Implemented in [`models/users/currentuser.cpp`](models/users/currentuser.cpp)

Current behavior:

- Clears the in-memory current-user profile.
- Does not contact a server.

### `ISXCurrentUser::CurrentUser::update_avatar_path`

- Declared in [`headers/users/currentuser.h`](headers/users/currentuser.h)
- Implemented in [`models/users/currentuser.cpp`](models/users/currentuser.cpp)

Current behavior:

- Copies a local avatar file into app storage when possible.
- Updates the stored avatar path.
- Does not contact a server.

### `ISXMail::AccountListModel::AddAccount`

- Declared in [`headers/users/accountlistmodel.h`](headers/users/accountlistmodel.h)
- Implemented in [`models/users/accountlistmodel.cpp`](models/users/accountlistmodel.cpp)

Signature:

```cpp
void AddAccount(
    const QString& name,
    const QString& email,
    const QString& avatarUrl,
    const QString& avatarColor,
    const QString& avatarInitial,
    bool isActive = false
);
```

Current behavior:

- Adds an account to the in-memory model only.
- Does not contact a server.

### `ISXMail::AccountListModel::SetActiveAccount`

- Declared in [`headers/users/accountlistmodel.h`](headers/users/accountlistmodel.h)
- Implemented in [`models/users/accountlistmodel.cpp`](models/users/accountlistmodel.cpp)

Signature:

```cpp
bool SetActiveAccount(int row);
```

Current behavior:

- Updates the active flag across the local model.
- Emits `activeAccountChanged`.
- Does not contact a server.

### `ISXMail::AccountListModel::RemoveAccount`

- Declared in [`headers/users/accountlistmodel.h`](headers/users/accountlistmodel.h)
- Implemented in [`models/users/accountlistmodel.cpp`](models/users/accountlistmodel.cpp)

Current behavior:

- Removes an account from the local model only.
- Does not contact a server.

## 5. UI signals that are intended to feed server-side auth

These are not server calls yet. They are the current handoff points where server auth logic should be attached.

### `screens/navigation/account/LoginQML.qml`

- Signal: `loginSubmitted(string email, string password)`

### `screens/navigation/account/RegistrationQML.qml`

- Signal: `loginSubmitted(string email, string password)`
- Despite the signal name, it currently emits the registration form values.

### `screens/navigation/account/AddAccountQML.qml`

- Routes to login/registration screens and contains the `Connections` object that currently closes the dialog without
  server work.

## 6. Data fields currently assembled for transmission

### Login and registration

- `email`
- `password`
- `name` on the registration screen

### Message send

- `sender_name`
- `sender_email`
- `recipient_email`
- `subject`
- `body`
- `is_inbox`

### Draft save

- `sender_name`
- `sender_email`
- `recipient_email`
- `subject`
- `body`

## 7. Current gap

If the goal is true client-to-server documentation, the missing piece is a network transport layer or API client. At the
moment, these interfaces only cover:

- local database persistence
- local model updates
- QML-to-C++ signal handoff

When the transport layer is added, this document should be extended with:

- endpoint names
- request methods
- request/response schemas
- error handling
- retry behavior
