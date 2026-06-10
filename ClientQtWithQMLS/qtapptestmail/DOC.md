\### EmailListModel



`EmailListModel` is a custom `QAbstractListModel` implementation designed for displaying email messages in QML applications.



\#### Features



\* Stores email information:



&#x20; \* Starred status

&#x20; \* Subject/theme

&#x20; \* Sender name

&#x20; \* Message preview

&#x20; \* Timestamp

\* Exposes data to QML through custom roles.

\* Supports pagination with a fixed page size of \*\*8 emails per page\*\*.

\* Provides methods for navigating between pages:



&#x20; \* `NextPage()`

&#x20; \* `PrevPage()`

&#x20; \* `SetPage(int page)`

\* Allows dynamic insertion of email entries.

\* Notifies the UI when the current page or total page count changes.



\#### Roles



| Role          | Description                            |

| ------------- | -------------------------------------- |

| `StarredRole` | Indicates whether the email is starred |

| `ThemeRole`   | Email subject/theme                    |

| `NameRole`    | Sender name                            |

| `PreviewRole` | Short preview of the email content     |

| `TimeRole`    | Email timestamp                        |



\#### Purpose



This model is intended for use with QML views such as `ListView`, providing an efficient and scalable way to display large email collections while keeping only a subset of items visible through pagination.



