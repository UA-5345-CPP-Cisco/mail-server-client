#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd "$(dirname "$0")" && pwd)"
sources=()

while IFS= read -r -d '' source_file; do
  sources+=("${source_file}")
done < <("${script_dir}/list-clang-format-files.sh")

if [[ "${#sources[@]}" -gt 0 ]]; then
  "${CLANG_FORMAT:-clang-format}" --style=file --dry-run --Werror "${sources[@]}"
fi
