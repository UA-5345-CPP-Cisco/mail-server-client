ALTER TABLE mail_messages
    ADD COLUMN archived_at TEXT;

UPDATE mail_messages
SET archived_at = CURRENT_TIMESTAMP
WHERE is_archive = 1;
