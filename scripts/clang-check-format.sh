#!/usr/bin/env bash
set -euo pipefail

sources=()

while IFS= read -r -d '' source_file; do
  sources+=("${source_file}")
done < <("$(dirname "$0")/list-clang-format-files.sh")

if [[ "${#sources[@]}" -eq 0 ]]; then
  exit 0
fi

clang-format --dry-run --Werror "${sources[@]}"
