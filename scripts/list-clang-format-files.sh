#!/usr/bin/env bash
set -euo pipefail

source_dirs=(
  "apps/mail-server"
  "apps/mail-server-neo"
  "apps/smtp-server"
  "apps/simple-client"
  "libs"
  "tests"
)

sources=()

for source_dir in "${source_dirs[@]}"; do
  if [[ -d "${source_dir}" ]]; then
    while IFS= read -r -d '' source_file; do
      sources+=("${source_file}")
    done < <(
      find "${source_dir}" \
        -type f \
        \( -name '*.cpp' -o -name '*.h' -o -name '*.hpp' \) \
        -print0
    )
  fi
done

printf '%s\0' "${sources[@]}"
