CREATE TABLE mail_messages_new
(
    id                  INTEGER PRIMARY KEY AUTOINCREMENT,
    subject             TEXT,
    body                TEXT NOT NULL,
    reply_to_message_id INTEGER,
    created_at          TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,
    message_status      TEXT NOT NULL DEFAULT 'queued',

    FOREIGN KEY (reply_to_message_id)
        REFERENCES mail_messages_new (id)
        ON DELETE SET NULL,

    CHECK (
        reply_to_message_id IS NULL
            OR reply_to_message_id != id
    ),

    CHECK (message_status IN (
        'draft',
        'queued',
        'sending',
        'sent',
        'failed'
    ))
);

INSERT INTO mail_messages_new (
    id,
    subject,
    body,
    reply_to_message_id,
    created_at,
    message_status
)
SELECT
    id,
    subject,
    body,
    reply_to_message_id,
    created_at,
    CASE
        WHEN is_draft = 1 THEN 'draft'
        WHEN status = 'archive' THEN 'sent'
        ELSE status
    END
FROM mail_messages;

CREATE TABLE mail_message_actors_new
(
    id              INTEGER PRIMARY KEY AUTOINCREMENT,
    message_id      INTEGER NOT NULL,
    actor_email     TEXT NOT NULL,
    actor_type      TEXT NOT NULL,
    delivery_status TEXT,
    delivered_at    TEXT,
    next_attempt_at TEXT,
    attempt_count   INTEGER NOT NULL DEFAULT 0,
    last_error      TEXT,
    read_at         TEXT,
    starred_at      TEXT,
    archived_at     TEXT,
    deleted_at      TEXT,

    FOREIGN KEY (message_id)
        REFERENCES mail_messages_new (id)
        ON DELETE CASCADE,

    CHECK (actor_type IN ('FROM', 'TO', 'CC', 'BCC')),

    CHECK (
        (actor_type = 'FROM' AND delivery_status IS NULL)
        OR
        (actor_type IN ('TO', 'CC', 'BCC') AND delivery_status IN (
            'pending',
            'queued',
            'delivering',
            'delivered',
            'temporary_failed',
            'bounced',
            'failed'
        ))
    ),

    CHECK (attempt_count >= 0)
);

INSERT INTO mail_message_actors_new (
    message_id,
    actor_email,
    actor_type,
    delivery_status,
    delivered_at,
    next_attempt_at,
    attempt_count,
    last_error,
    starred_at,
    archived_at
)
SELECT
    id,
    sender_email,
    'FROM',
    NULL,
    NULL,
    NULL,
    0,
    NULL,
    CASE WHEN is_starred = 1 THEN CURRENT_TIMESTAMP ELSE NULL END,
    CASE WHEN is_archive = 1 THEN COALESCE(archived_at, CURRENT_TIMESTAMP) ELSE NULL END
FROM mail_messages;

INSERT INTO mail_message_actors_new (
    message_id,
    actor_email,
    actor_type,
    delivery_status,
    delivered_at,
    next_attempt_at,
    attempt_count,
    last_error,
    starred_at,
    archived_at
)
SELECT
    message_id,
    recipient_email,
    recipient_type,
    delivery_status,
    delivered_at,
    next_attempt_at,
    attempt_count,
    last_error,
    CASE WHEN mail_messages.is_starred = 1 THEN CURRENT_TIMESTAMP ELSE NULL END,
    CASE WHEN mail_messages.is_archive = 1 THEN COALESCE(mail_messages.archived_at, CURRENT_TIMESTAMP) ELSE NULL END
FROM message_recipients
INNER JOIN mail_messages
    ON mail_messages.id = message_recipients.message_id;

DROP TABLE message_recipients;
DROP TABLE mail_messages;

ALTER TABLE mail_messages_new
    RENAME TO mail_messages;

ALTER TABLE mail_message_actors_new
    RENAME TO mail_message_actors;

CREATE INDEX idx_mail_messages_status_created_at
    ON mail_messages (message_status, created_at);

CREATE INDEX idx_mail_messages_reply_to_message_id
    ON mail_messages (reply_to_message_id);

CREATE INDEX idx_mail_message_actors_message_id
    ON mail_message_actors (message_id);

CREATE INDEX idx_mail_message_actors_mailbox
    ON mail_message_actors (actor_email, deleted_at, archived_at);

CREATE INDEX idx_mail_message_actors_delivery_queue
    ON mail_message_actors (delivery_status, next_attempt_at);
