#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd "$(dirname "$0")" && pwd)"
project_root="$(cd "${script_dir}/.." && pwd)"

source_dirs=(
  "${project_root}/apps"
)

sources=()

for source_dir in "${source_dirs[@]}"; do
  if [[ -d "${source_dir}" ]]; then
    while IFS= read -r -d '' source_file; do
      sources+=("${source_file}")
    done < <(
      find "${source_dir}" \
        -type f \
        -name '*.qml' \
        ! -path '*/build/*' \
        ! -path '*/cmake-build-*/*' \
        -print0
    )
  fi
done

printf '%s\0' "${sources[@]}"