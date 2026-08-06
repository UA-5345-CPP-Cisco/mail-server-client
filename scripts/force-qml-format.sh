#!/usr/bin/env bash
set -euo pipefail

QMLFORMAT="${QMLFORMAT:-qmlformat}"

if ! command -v "$QMLFORMAT" >/dev/null 2>&1; then
  echo "Error: qmlformat not found."
  echo "Add your Qt bin directory to PATH or set the QMLFORMAT environment variable."
  exit 1
fi

sources=()

while IFS= read -r -d '' source_file; do
  sources+=("$source_file")
done < <("$(dirname "$0")/list-qml-format-files.sh")

if [[ "${#sources[@]}" -eq 0 ]]; then
  exit 0
fi

for source in "${sources[@]}"; do
  "$QMLFORMAT" -i "$source"
done