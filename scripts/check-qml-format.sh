#!/usr/bin/env bash
set -euo pipefail

QMLFORMAT="${QMLFORMAT:-qmlformat}"
script_dir="$(cd "$(dirname "$0")" && pwd)"
temporary_dir="$(mktemp -d)"

trap 'rm -rf "$temporary_dir"' EXIT

if [[ -n "${QT_ROOT:-}" && -x "$QT_ROOT/6.11.1/mingw_64/bin/qmlformat.exe" ]]; then
  QMLFORMAT="$QT_ROOT/6.11.1/mingw_64/bin/qmlformat.exe"
fi

if ! command -v "$QMLFORMAT" >/dev/null 2>&1; then
  echo "Error: qmlformat not found."
  echo "Add your Qt bin directory to PATH or set the QMLFORMAT environment variable."
  exit 1
fi

failed=0

while IFS= read -r -d '' source_file; do
  formatted_file="${temporary_dir}/$(basename "$source_file").formatted"

  if ! "$QMLFORMAT" --newline unix "$source_file" >"$formatted_file"; then
    failed=1
    continue
  fi

  if ! cmp -s "$source_file" "$formatted_file"; then
    echo "${source_file}: QML format differs"
    failed=1
  fi
done < <(bash "${script_dir}/list-qml-format-files.sh")

exit $failed
