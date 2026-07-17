#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd "$(dirname "$0")" && pwd)"
project_root="$(cd "${script_dir}/.." && pwd)"

source_dirs=(
  "${project_root}/apps/mail-server"
  "${project_root}/apps/smtp-server"
  "${project_root}/apps/simple-client"
  "${project_root}/libs"
  "${project_root}/tests"
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
