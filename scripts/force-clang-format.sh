#!/usr/bin/env bash
set -euo pipefail

echo "=== force-clang-format.sh ==="

echo "Current directory : $(pwd)"
echo "Script directory  : $(cd "$(dirname "$0")" && pwd)"

echo

echo "Checking clang-format..."
if ! command -v "${CLANG_FORMAT:-clang-format}" >/dev/null 2>&1; then
    echo "ERROR: ${CLANG_FORMAT:-clang-format} was not found in PATH"
    echo "PATH=$PATH"
    exit 1
fi

echo "Using: $(command -v "${CLANG_FORMAT:-clang-format}")"
"${CLANG_FORMAT:-clang-format}" --version

echo

list_script="$(dirname "$0")/list-clang-format-files.sh"

echo "Checking list script..."
echo "Path: $list_script"

if [[ ! -f "$list_script" ]]; then
    echo "ERROR: list-clang-format-files.sh does not exist."
    exit 1
fi

if [[ ! -x "$list_script" ]]; then
    echo "WARNING: list-clang-format-files.sh is not executable."
fi

echo

echo "Collecting source files..."

sources=()

while IFS= read -r -d '' source_file; do
    echo "  $source_file"
    sources+=("$source_file")
done < <("$list_script")

echo
echo "Found ${#sources[@]} source files."

if [[ "${#sources[@]}" -eq 0 ]]; then
    echo "Nothing to format."
    exit 0
fi

echo
echo "Running clang-format..."

printf '  %q ' "${CLANG_FORMAT:-clang-format}" -i "${sources[@]}"
echo
echo

"${CLANG_FORMAT:-clang-format}" -i "${sources[@]}"

echo
echo "Done."