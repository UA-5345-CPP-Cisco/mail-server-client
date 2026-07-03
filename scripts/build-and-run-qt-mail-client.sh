#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd "$(dirname "$0")" && pwd)"
project_root="$(cd "${script_dir}/.." && pwd)"
source_dir="${project_root}/apps/qt-mail-client"
build_dir="${project_root}/cmake-build-qt-mail-client"
mode="${1:-run}"

if [[ "${mode}" != "run" && "${mode}" != "build" ]]; then
  printf 'Usage: %s [run|build]\n' "$0" >&2
  exit 1
fi

platform="$(uname -s)"
qt_prefix="${QT_PREFIX:-}"
mingw_bin=""
ninja_dir=""

require_tool() {
  if ! command -v "$1" >/dev/null 2>&1; then
    printf '%s was not found in PATH\n' "$1" >&2
    exit 1
  fi
}

find_macos_qt() {
  if [[ -n "${qt_prefix}" ]]; then
    return
  fi

  if command -v brew >/dev/null 2>&1; then
    qt_prefix="$(brew --prefix qt 2>/dev/null || true)"
  fi
}

find_linux_qt() {
  if [[ -n "${qt_prefix}" ]]; then
    return
  fi

  if command -v qmake6 >/dev/null 2>&1; then
    qt_prefix="$(qmake6 -query QT_INSTALL_PREFIX)"
  elif command -v qmake >/dev/null 2>&1; then
    qt_prefix="$(qmake -query QT_INSTALL_PREFIX)"
  fi
}

find_windows_qt() {
  local qt_base=""

  for directory in /d/Qt /c/Qt; do
    if [[ -d "${directory}" ]]; then
      qt_base="${directory}"
      break
    fi
  done

  if [[ -z "${qt_base}" ]]; then
    printf 'Qt installation was not found in /c/Qt or /d/Qt\n' >&2
    exit 1
  fi

  qt_prefix="$(find "${qt_base}" -maxdepth 2 -type d -name mingw_64 | sort -V | tail -1)"
  mingw_bin="$(find "${qt_base}/Tools" -maxdepth 2 -type d -path '*/bin' | grep mingw | head -1 || true)"
  ninja_dir="${qt_base}/Tools/Ninja"

  if [[ -z "${qt_prefix}" ]]; then
    printf 'Could not locate Qt mingw_64 installation\n' >&2
    exit 1
  fi

  if [[ -z "${mingw_bin}" ]]; then
    printf 'Could not locate MinGW\n' >&2
    exit 1
  fi

  if [[ ! -d "${ninja_dir}" ]]; then
    printf 'Could not locate Ninja\n' >&2
    exit 1
  fi

  export PATH="${qt_prefix}/bin:${mingw_bin}:${ninja_dir}:${PATH}"
}

parallel_arg() {
  if command -v nproc >/dev/null 2>&1; then
    nproc
  elif command -v getconf >/dev/null 2>&1; then
    getconf _NPROCESSORS_ONLN
  elif command -v sysctl >/dev/null 2>&1; then
    sysctl -n hw.ncpu 2>/dev/null || printf '2'
  else
    printf '2'
  fi
}

case "${platform}" in
  Darwin*)
    find_macos_qt
    ;;
  Linux*)
    find_linux_qt
    ;;
  MINGW*|MSYS*|CYGWIN*)
    find_windows_qt
    ;;
  *)
    printf 'Unsupported platform: %s\n' "${platform}" >&2
    exit 1
    ;;
esac

require_tool cmake
require_tool ninja

cmake_args=(
  -S "${source_dir}"
  -B "${build_dir}"
  -G Ninja
  -DCMAKE_BUILD_TYPE=Debug
  -DCMAKE_CXX_STANDARD=17
  -DCMAKE_CXX_STANDARD_REQUIRED=ON
)

if [[ -n "${qt_prefix}" ]]; then
  cmake_args+=("-DCMAKE_PREFIX_PATH=${qt_prefix}")
fi

cmake "${cmake_args[@]}"
cmake --build "${build_dir}" --target appqtapptestmail --parallel "$(parallel_arg)"

if [[ "${mode}" == "build" ]]; then
  exit 0
fi

case "${platform}" in
  Darwin*)
    executable="${build_dir}/appqtapptestmail.app/Contents/MacOS/appqtapptestmail"
    ;;
  Linux*)
    executable="${build_dir}/appqtapptestmail"
    ;;
  MINGW*|MSYS*|CYGWIN*)
    executable="${build_dir}/appqtapptestmail.exe"

    for dll in libstdc++-6.dll libgcc_s_seh-1.dll libwinpthread-1.dll; do
      if [[ -f "${mingw_bin}/${dll}" ]]; then
        cp -f "${mingw_bin}/${dll}" "${build_dir}/"
      fi
    done

    if command -v windeployqt.exe >/dev/null 2>&1; then
      windeployqt.exe "${executable}"
    fi
    ;;
esac

if [[ ! -x "${executable}" ]]; then
  printf 'Qt mail client executable was not found: %s\n' "${executable}" >&2
  exit 1
fi

cd "${source_dir}"
exec "${executable}"
