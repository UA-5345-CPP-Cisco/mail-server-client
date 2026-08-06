#!/usr/bin/env bash
set -euo pipefail

QMLFORMAT="${QMLFORMAT:-qmlformat}"

if ! command -v "$QMLFORMAT" >/dev/null 2>&1; then
  echo "Error: qmlformat not found."
  echo "Add your Qt bin directory to PATH or set the QMLFORMAT environment variable."
  exit 1
fi

failed=0

while IFS= read -r -d '' source_file; do
  if ! "$QMLFORMAT" --check "$source_file"; then
    failed=1
  fi
done < <("$(dirname "$0")/list-qml-format-files.sh")

exit $failed