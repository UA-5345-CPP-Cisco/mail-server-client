CREATE TABLE cache_entries (
                               cache_namespace TEXT NOT NULL,
                               cache_key TEXT NOT NULL,
                               payload TEXT NOT NULL,
                               version INTEGER NOT NULL,
                               created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,
                               expires_at TEXT NOT NULL,
                               PRIMARY KEY (cache_namespace, cache_key)
);

CREATE INDEX idx_cache_entries_expires_at ON cache_entries (expires_at);