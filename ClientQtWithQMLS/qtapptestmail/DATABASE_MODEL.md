# Database Model

This document describes the SQLite layer used by the mail client, including every file in `models/database`, the schema in `include/migrations`, and how the runtime code uses it.

## Overview

The database layer has three jobs:

1. Open and configure the SQLite connection.
2. Apply schema migrations on startup.
3. Expose typed repositories for users, messages, recipients, and cache entries.

The runtime flow is:

1. `main.cpp` calls `ISXDatabaseManager::DatabaseManager::EnsureInitialized()`.
2. `DatabaseManager` resolves the application database path and migration directory.
3. `MigrationRunner` creates `schema_migrations` if needed and applies SQL files from `include/migrations`.
4. UI-facing code then opens the same database file through repository-backed classes.

The database file is stored in the application data directory as:

`qtapptestmail.sqlite`

## Files In `models/database`

### `models/database/Database.cpp`

Responsible for opening and configuring SQLite.

Key behavior:

- Opens the database with:
  - `SQLITE_OPEN_READWRITE`
  - `SQLITE_OPEN_CREATE`
  - `SQLITE_OPEN_FULLMUTEX`
- Enables a 5 second busy timeout.
- Sets these pragmas after opening:
  - `foreign_keys = ON`
  - `journal_mode = WAL`
  - `synchronous = NORMAL`

Why it matters:

- `foreign_keys = ON` is required for the `mail_messages` and `message_recipients` relationship to work.
- `WAL` improves concurrency for read/write access from the UI and background actions.

### `models/database/Statement.cpp`

Thin RAII wrapper around `sqlite3_stmt`.

Responsibilities:

- Prepare a SQL statement.
- Bind integers, 64-bit integers, text, and nulls.
- Step through result rows.
- Read column values.
- Report:
  - last inserted row id
  - number of changed rows

Design notes:

- `BindText` uses `SQLITE_TRANSIENT`, so it is safe to pass temporary strings.
- `Step()` returns:
  - `true` for `SQLITE_ROW`
  - `false` for `SQLITE_DONE`
- Errors are translated into `std::runtime_error` with SQLite’s message attached.

### `models/database/MigrationRunner.cpp`

Applies SQL migration files from a directory.

Startup flow:

1. Create `schema_migrations` if it does not exist.
2. Read all `*.sql` files from the migration directory.
3. Sort them by version prefix.
4. Apply each migration inside a transaction.
5. Record the migration version and file name in `schema_migrations`.

Important details:

- Migration filenames must start with a numeric prefix followed by `_`.
  - Example: `001_initial_schema.sql`
  - Example: `002_mail_delivery.sql`
- Duplicate migration versions are rejected.
- Each migration is applied only once.

### `models/database/UserRepository.cpp`

Repository for the `users` table.

Methods:

- `CreateUser(...)`
- `FindById(...)`
- `FindByEmail(...)`
- `FindByUsername(...)`
- `UpdateStatus(...)`

Behavior:

- `CreateUser` inserts a user and returns the new row id.
- Reads map directly to `Storage::UserRecord`.
- `UpdateStatus` updates `status` by id.

Current schema expectation:

- `users.id`
- `users.username`
- `users.email`
- `users.password_hash`
- `users.status`
- `users.created_at`

### `models/database/MailMessageRepository.cpp`

Repository for the `mail_messages` table.

Methods:

- `CreateMessage(...)`
- `FindById(...)`
- `FindAll()`
- `FindByStatus(...)`
- `UpdateStatus(...)`
- `FinalizeDelivery(...)`

Behavior:

- `CreateMessage` inserts a message row and returns the message id.
- `FindAll()` loads every message ordered by newest first.
- `FindByStatus()` returns messages for a specific status with a row limit.
- `UpdateStatus()` performs an optimistic update using the expected status.
- `FinalizeDelivery()` marks a queued/sending message as:
  - `sent` if at least one recipient is delivered
  - `failed` otherwise, if no recipients remain in transit

Current schema expectation:

- `mail_messages.id`
- `mail_messages.sender_user_id`
- `mail_messages.sender_email`
- `mail_messages.subject`
- `mail_messages.body`
- `mail_messages.reply_to_message_id`
- `mail_messages.created_at`
- `mail_messages.status`

### `models/database/MessageRecipientRepository.cpp`

Repository for the `message_recipients` table.

Methods:

- `CreateRecipient(...)`
- `FindById(...)`
- `FindByMessageId(...)`
- `QueueRecipient(...)`
- `ClaimReadyRecipients(...)`
- `MarkDelivered(...)`
- `MarkTemporaryFailed(...)`
- `MarkBounced(...)`
- `MarkFailed(...)`

Behavior:

- Supports the delivery queue lifecycle.
- Uses status strings such as:
  - `pending`
  - `queued`
  - `delivering`
  - `delivered`
  - `temporary_failed`
  - `bounced`
  - `failed`
- `ClaimReadyRecipients()` uses a `WITH` query plus `RETURNING` to atomically claim work.

Current schema expectation:

- `message_recipients.id`
- `message_recipients.message_id`
- `message_recipients.recipient_email`
- `message_recipients.recipient_type`
- `message_recipients.delivery_status`
- `message_recipients.attempt_count`
- `message_recipients.next_attempt_at`
- `message_recipients.last_error`
- `message_recipients.delivered_at`

### `models/database/cachestore.cpp`

Generic cache table helper.

Schema:

- `cache_entries`

Columns:

- `cache_namespace`
- `cache_key`
- `payload`
- `version`
- `created_at`
- `expires_at`

Methods:

- `EnsureSchema()`
- `Put(...)`
- `Get(...)`
- `Exists(...)`
- `Invalidate(...)`
- `InvalidateNamespace(...)`
- `InvalidateNamespacePrefix(...)`

Behavior:

- `Put` stores payload with TTL.
- `Get` returns `std::nullopt` when the entry is expired unless `allow_stale` is true.
- `InvalidateNamespacePrefix` deletes a group of cache namespaces with `LIKE`.

This file is used by:

- `models/users/usercache.cpp`
- `models/mail/emailcache.cpp`

### `models/database/databasemanager.cpp`

Startup helper for database initialization.

Responsibilities:

- Resolve the app data directory.
- Resolve the SQLite database file path.
- Resolve the migration directory.
- Run migrations before any repository is used.

Methods:

- `EnsureInitialized()`
- `DatabasePath()`
- `MigrationsPath()`
- `ResolveAppDataDirectory()`

Path resolution behavior:

- Database path is stored in the app data directory.
- Migration path is resolved by checking:
  - current working directory + `include/migrations`
  - application directory and up to 6 parent directories

This is what prevents startup crashes like:

`Failed to prepare SQLite statement: no such table: mail_messages`

## Schema In `include/migrations`

### `include/migrations/001_initial_schema.sql`

Creates the `users` table.

Fields:

- `id`
- `username`
- `email`
- `password_hash`
- `status`
- `created_at`

Constraints:

- `username` unique
- `email` unique
- `status` limited to `active` or `disabled`

### `include/migrations/002_mail_delivery.sql`

Creates the mail delivery tables.

Tables:

- `mail_messages`
- `message_recipients`

Also creates indexes for:

- message status + created time
- sender user id
- reply-to message id
- recipient message id
- delivery queue lookup

## Domain Records

These are not repository classes, but they define the data shape used by the repositories.

### `headers/database/UserRecord.h`

Represents a row from `users`.

Fields:

- `id`
- `username`
- `email`
- `password_hash`
- `status`
- `created_at`

### `headers/database/MailMessageRecord.h`

Represents a row from `mail_messages`.

Fields:

- `id`
- `sender_user_id`
- `sender_email`
- `subject`
- `body`
- `reply_to_message_id`
- `created_at`
- `status`

### `headers/database/MessageRecipientRecord.h`

Represents a row from `message_recipients`.

Fields:

- `id`
- `message_id`
- `recipient_email`
- `recipient_type`
- `delivery_status`
- `attempt_count`
- `next_attempt_at`
- `last_error`
- `delivered_at`

## How The UI Uses The Database

### Compose Flow

`NewMessageQML.qml` calls `messageComposer.SendMessage(...)` or `messageComposer.SaveDraft(...)`.

That object:

- inserts a message into `mail_messages`
- inserts the recipient into `message_recipients`
- wraps both inserts in a transaction

### Startup Load Flow

`EmailListModel` loads persisted messages on construction:

1. `FindAll()` returns all messages.
2. `FindByMessageId()` fetches the first recipient for display.
3. Each row becomes an `EmailData` item.

This makes the message list survive app restarts.

## Error Handling

Common error paths are converted to `std::runtime_error`:

- SQLite open failure
- SQL execution failure
- statement prepare failure
- invalid migration filename
- duplicate migration version
- invalid cache timestamp

That keeps the error boundary explicit and avoids silent data corruption.

## Notes And Current Constraints

- The runtime still has some domain duplication between UI-facing `EmailListModel` and the raw database schema.
- `EmailListModel` currently maps:
  - `theme` from `subject`
  - `name` from `sender_email`
  - `send_to` from the first recipient
- `created_at` is stored and currently displayed as the raw SQLite timestamp string.

## File Map

Database-related files currently in `models/database`:

- `models/database/Database.cpp`
- `models/database/Statement.cpp`
- `models/database/MigrationRunner.cpp`
- `models/database/UserRepository.cpp`
- `models/database/MailMessageRepository.cpp`
- `models/database/MessageRecipientRepository.cpp`
- `models/database/cachestore.cpp`
- `models/database/databasemanager.cpp`

