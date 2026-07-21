#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd "$(dirname "$0")" && pwd)"
project_root="$(cd "${script_dir}/.." && pwd)"

db_path="${project_root}/data/mail-server-example.db"
migrations_path="${project_root}/libs/mail-storage/migrations"

show_help() {
  cat <<'EOF'
Usage:
  scripts/seed-mail-storage.sh [--db PATH] [--migrations PATH]

Seeds the mail storage SQLite database with demo users and messages.

Examples:
  scripts/seed-mail-storage.sh
  scripts/seed-mail-storage.sh --db data/mail-server.db

The Docker compose setup mounts ./data to /app/data, so --db data/mail-server.db
seeds the same database used by the mail-server and smtp-server containers.
EOF
}

while [[ $# -gt 0 ]]; do
  case "$1" in
    --db)
      db_path="$2"
      shift 2
      ;;
    --migrations)
      migrations_path="$2"
      shift 2
      ;;
    -h|--help)
      show_help
      exit 0
      ;;
    *)
      printf 'Unknown argument: %s\n\n' "$1" >&2
      show_help >&2
      exit 1
      ;;
  esac
done

case "${db_path}" in
  /*) ;;
  *) db_path="${project_root}/${db_path}" ;;
esac

case "${migrations_path}" in
  /*) ;;
  *) migrations_path="${project_root}/${migrations_path}" ;;
esac

if ! command -v sqlite3 >/dev/null 2>&1; then
  printf 'sqlite3 was not found in PATH\n' >&2
  exit 1
fi

if [[ ! -d "${migrations_path}" ]]; then
  printf 'Migrations directory was not found: %s\n' "${migrations_path}" >&2
  exit 1
fi

mkdir -p "$(dirname "${db_path}")"

sqlite3 "${db_path}" <<'SQL'
PRAGMA foreign_keys = ON;

CREATE TABLE IF NOT EXISTS schema_migrations (
  version INTEGER PRIMARY KEY,
  name TEXT NOT NULL,
  applied_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP
);
SQL

for migration_file in "${migrations_path}"/*.sql; do
  migration_name="$(basename "${migration_file}")"
  migration_version="${migration_name%%_*}"

  if [[ ! "${migration_version}" =~ ^[0-9]+$ ]]; then
    printf 'Skipping migration with invalid filename: %s\n' "${migration_name}" >&2
    continue
  fi

  migration_is_applied="$(
    sqlite3 "${db_path}" \
      "SELECT EXISTS(SELECT 1 FROM schema_migrations WHERE version = ${migration_version});"
  )"

  if [[ "${migration_is_applied}" == "1" ]]; then
    continue
  fi

  sqlite3 "${db_path}" <<SQL
PRAGMA foreign_keys = ON;
BEGIN IMMEDIATE;
.read ${migration_file}
INSERT INTO schema_migrations (version, name)
VALUES (${migration_version}, '${migration_name}');
COMMIT;
SQL
done

sqlite3 "${db_path}" <<'SQL'
PRAGMA foreign_keys = ON;
BEGIN IMMEDIATE;

INSERT INTO users (username, email, password_hash, status)
VALUES
  ('alice', 'alice@mail.local', 'Password1!', 'active'),
  ('bob', 'bob@mail.local', 'Password1!', 'active'),
  ('carol', 'carol@mail.local', 'Password1!', 'active'),
  ('dave_disabled', 'dave@mail.local', 'Password1!', 'disabled')
ON CONFLICT(email) DO UPDATE SET
  username = excluded.username,
  password_hash = excluded.password_hash,
  status = excluded.status;

DELETE FROM mail_messages
WHERE substr(COALESCE(subject, ''), 1, 7) = '[demo] ';

INSERT INTO mail_messages (
  sender_user_id,
  sender_email,
  subject,
  body,
  is_inbox,
  is_starred,
  is_draft,
  is_archive,
  status,
  archived_at
)
VALUES (
  (SELECT id FROM users WHERE email = 'alice@mail.local'),
  'alice@mail.local',
  '[demo] Welcome to Mail Server',
  'Hello Bob and Carol. This message demonstrates a delivered mail with multiple recipients.',
  0,
  1,
  0,
  0,
  'sent',
  NULL
);

INSERT INTO message_recipients (message_id, recipient_email, recipient_type, delivery_status, delivered_at)
VALUES
  ((SELECT id FROM mail_messages WHERE subject = '[demo] Welcome to Mail Server'), 'bob@mail.local', 'TO', 'delivered', CURRENT_TIMESTAMP),
  ((SELECT id FROM mail_messages WHERE subject = '[demo] Welcome to Mail Server'), 'carol@mail.local', 'CC', 'delivered', CURRENT_TIMESTAMP);

INSERT INTO mail_messages (
  sender_user_id,
  sender_email,
  subject,
  body,
  is_inbox,
  is_starred,
  is_draft,
  is_archive,
  status,
  archived_at
)
VALUES (
  (SELECT id FROM users WHERE email = 'bob@mail.local'),
  'bob@mail.local',
  '[demo] Re: Welcome to Mail Server',
  'Thanks Alice. I can see the seeded inbox message through the HTTP API.',
  1,
  0,
  0,
  0,
  'sent',
  NULL
);

INSERT INTO message_recipients (message_id, recipient_email, recipient_type, delivery_status, delivered_at)
VALUES (
  (SELECT id FROM mail_messages WHERE subject = '[demo] Re: Welcome to Mail Server'),
  'alice@mail.local',
  'TO',
  'delivered',
  CURRENT_TIMESTAMP
);

INSERT INTO mail_messages (
  sender_user_id,
  sender_email,
  subject,
  body,
  is_inbox,
  is_starred,
  is_draft,
  is_archive,
  status,
  archived_at
)
VALUES (
  (SELECT id FROM users WHERE email = 'carol@mail.local'),
  'carol@mail.local',
  '[demo] Queued delivery example',
  'This message is queued so the SMTP delivery path has sample data to inspect.',
  0,
  0,
  0,
  0,
  'queued',
  NULL
);

INSERT INTO message_recipients (message_id, recipient_email, recipient_type, delivery_status)
VALUES (
  (SELECT id FROM mail_messages WHERE subject = '[demo] Queued delivery example'),
  'alice@mail.local',
  'TO',
  'queued'
);

INSERT INTO mail_messages (
  sender_user_id,
  sender_email,
  subject,
  body,
  is_inbox,
  is_starred,
  is_draft,
  is_archive,
  status,
  archived_at
)
VALUES (
  (SELECT id FROM users WHERE email = 'alice@mail.local'),
  'alice@mail.local',
  '[demo] Draft message',
  'This draft demonstrates local message state before it is sent.',
  0,
  0,
  1,
  0,
  'draft',
  NULL
);

INSERT INTO mail_messages (
  sender_user_id,
  sender_email,
  subject,
  body,
  is_inbox,
  is_starred,
  is_draft,
  is_archive,
  status,
  archived_at
)
VALUES (
  (SELECT id FROM users WHERE email = 'bob@mail.local'),
  'bob@mail.local',
  '[demo] Archived message',
  'This message demonstrates archive flags in the storage schema.',
  1,
  0,
  0,
  1,
  'archive',
  CURRENT_TIMESTAMP
);

INSERT INTO message_recipients (message_id, recipient_email, recipient_type, delivery_status, delivered_at)
VALUES (
  (SELECT id FROM mail_messages WHERE subject = '[demo] Archived message'),
  'alice@mail.local',
  'TO',
  'delivered',
  CURRENT_TIMESTAMP
);

COMMIT;
SQL

printf 'Seeded mail storage database: %s\n' "${db_path}"
printf 'Demo users: alice@mail.local, bob@mail.local, carol@mail.local, dave@mail.local\n'
printf 'Demo password: Password1!\n'
