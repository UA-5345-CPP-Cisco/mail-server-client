CREATE TABLE mail_messages
(
    id                  INTEGER PRIMARY KEY AUTOINCREMENT,
    sender_user_id      INTEGER,
    sender_email        TEXT NOT NULL,
    subject             TEXT,
    body                TEXT NOT NULL,
    reply_to_message_id INTEGER,
    created_at          TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,
    status              TEXT NOT NULL DEFAULT 'queued',

    FOREIGN KEY (sender_user_id)
        REFERENCES users (id)
        ON DELETE SET NULL,

    FOREIGN KEY (reply_to_message_id)
        REFERENCES mail_messages (id)
        ON DELETE SET NULL,

    CHECK (
        reply_to_message_id IS NULL
            OR reply_to_message_id != id
) ,

	CHECK (status IN (
		'draft',
		'queued',
		'sending',
		'sent',
		'failed'
	))
);

CREATE TABLE message_recipients
(
    id              INTEGER PRIMARY KEY AUTOINCREMENT,
    message_id      INTEGER NOT NULL,
    recipient_email TEXT    NOT NULL,
    recipient_type  TEXT    NOT NULL DEFAULT 'TO',
    delivery_status TEXT    NOT NULL DEFAULT 'pending',
    attempt_count   INTEGER NOT NULL DEFAULT 0,
    next_attempt_at TEXT,
    last_error      TEXT,
    delivered_at    TEXT,

    FOREIGN KEY (message_id)
        REFERENCES mail_messages (id)
        ON DELETE CASCADE,

    CHECK (recipient_type IN ('TO', 'CC', 'BCC')),

    CHECK (delivery_status IN (
                               'pending',
                               'queued',
                               'delivering',
                               'delivered',
                               'temporary_failed',
                               'bounced',
                               'failed'
        )),

    CHECK (attempt_count >= 0)
);

CREATE INDEX idx_mail_messages_status_created_at
    ON mail_messages (status, created_at);

CREATE INDEX idx_mail_messages_sender_user_id
    ON mail_messages (sender_user_id);

CREATE INDEX idx_mail_messages_reply_to_message_id
    ON mail_messages (reply_to_message_id);

CREATE INDEX idx_message_recipients_message_id
    ON message_recipients (message_id);

CREATE INDEX idx_message_recipients_delivery_queue
    ON message_recipients (delivery_status, next_attempt_at);
