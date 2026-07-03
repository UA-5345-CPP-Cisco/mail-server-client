#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd "$(dirname "$0")" && pwd)"
project_root="$(cd "${script_dir}/.." && pwd)"

source_dir="${project_root}/apps/qt-mail-client"
build_dir="${project_root}/cmake-build-qt-mail-client"

mode="${1:-run}"

if [[ "$mode" != "run" && "$mode" != "build" ]]; then
    echo "Usage: $0 [run|build]"
    exit 1
fi

# Find Qt installation if possible
cmake_args=(
    -S "$source_dir"
    -B "$build_dir"
    -G Ninja
    -DCMAKE_BUILD_TYPE=Debug
    -DCMAKE_CXX_STANDARD=17
    -DCMAKE_CXX_STANDARD_REQUIRED=ON
)

if command -v qmake6 >/dev/null 2>&1; then
    qt_prefix="$(qmake6 -query QT_INSTALL_PREFIX)"
    cmake_args+=("-DCMAKE_PREFIX_PATH=$qt_prefix")
elif command -v qmake >/dev/null 2>&1; then
    qt_prefix="$(qmake -query QT_INSTALL_PREFIX)"
    cmake_args+=("-DCMAKE_PREFIX_PATH=$qt_prefix")
fi

cmake "${cmake_args[@]}"

cmake --build "$build_dir" \
    --target appqtapptestmail \
    --parallel "$(nproc)"

exe="$build_dir/appqtapptestmail"

if [[ ! -x "$exe" ]]; then
    echo "Executable not found:"
    echo "$exe"
    exit 1
fi

if [[ "$mode" == "run" ]]; then
    cd "$source_dir"
    exec "$exe"
fi