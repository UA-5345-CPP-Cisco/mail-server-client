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

qt_base=""

for d in /d/Qt /c/Qt; do
    if [[ -d "$d" ]]; then
        qt_base="$d"
        break
    fi
done

if [[ -z "$qt_base" ]]; then
    echo "Qt installation not found."
    exit 1
fi

qt_prefix="$(find "$qt_base" -maxdepth 2 -type d -name mingw_64 | sort -V | tail -1)"

if [[ -z "$qt_prefix" ]]; then
    echo "Could not locate Qt mingw_64 installation."
    exit 1
fi

mingw_bin="$(find "$qt_base/Tools" -maxdepth 2 -type d -path "*/bin" | grep mingw | head -1 || true)"

if [[ -z "$mingw_bin" ]]; then
    echo "Could not locate MinGW."
    exit 1
fi

ninja_dir="$qt_base/Tools/Ninja"

if [[ ! -d "$ninja_dir" ]]; then
    echo "Could not locate Ninja."
    exit 1
fi

export PATH="$qt_prefix/bin:$PATH"
export PATH="$mingw_bin:$PATH"
export PATH="$ninja_dir:$PATH"

echo "$qt_prefix"

for tool in cmake ninja gcc g++ windeployqt.exe; do
    if ! command -v "$tool" >/dev/null 2>&1; then
        echo
        echo "ERROR: $tool not found."
        exit 1
    fi
done

echo "Qt:      $qt_prefix"
echo "MinGW:   $mingw_bin"
echo "Ninja:   $ninja_dir"
echo

cmake \
    -S "$source_dir" \
    -B "$build_dir" \
    -G Ninja \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_PREFIX_PATH="$qt_prefix"

cmake --build "$build_dir" \
    --target appqtapptestmail \
    --parallel

exe="$build_dir/appqtapptestmail.exe"

[[ -f "$exe" ]] || {
    echo "Executable not found!"
    exit 1
}

deployqt="$qt_prefix/bin/windeployqt.exe"

if [[ ! -f "$deployqt" ]]; then
    echo "windeployqt not found:"
    echo "$deployqt"
    exit 1
fi
for dll in \
    libstdc++-6.dll \
    libgcc_s_seh-1.dll \
    libwinpthread-1.dll
do
    [[ -f "$mingw_bin/$dll" ]] && cp -f "$mingw_bin/$dll" "$build_dir/"
done

if [[ "$mode" == "run" ]]; then
    cd "$source_dir"
    exec "$exe"
fi