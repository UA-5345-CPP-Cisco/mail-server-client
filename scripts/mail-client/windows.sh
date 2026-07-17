#!/usr/bin/env bash
set -euo pipefail

mode="$1"
qt_base=""
mingw_bin=""
ninja_dir=""

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

if [[ -z "${qt_prefix}" ]]; then
  qt_prefix="$(find "${qt_base}" -maxdepth 2 -type d -name mingw_64 | sort -V | tail -1)"
fi

mingw_bin="$(find "${qt_base}/Tools" -maxdepth 2 -type d -path '*/bin' | grep mingw | head -1 || true)"
cmake_bin="$(find "${qt_base}/Tools" -maxdepth 2 -type d -path '*/CMake_*/bin' | head -1 || true)"
ninja_dir="${qt_base}/Tools/Ninja"

if [[ -z "${qt_prefix}" ]]; then
  printf 'Could not locate Qt mingw_64 installation\n' >&2
  exit 1
fi

if [[ -z "${mingw_bin}" ]]; then
  printf 'Could not locate MinGW\n' >&2
  exit 1
fi

if [[ -z "${cmake_bin}" ]]; then
  printf 'Could not locate CMake\n' >&2
  exit 1
fi

if [[ ! -d "${ninja_dir}" ]]; then
  printf 'Could not locate Ninja\n' >&2
  exit 1
fi

export PATH="${qt_prefix}/bin:${cmake_bin}:${mingw_bin}:${ninja_dir}:${PATH}"

mail_client_require_tool gcc
mail_client_require_tool g++
mail_client_configure_and_build

executable="${build_dir}/appqtapptestmail.exe"

for dll in libstdc++-6.dll libgcc_s_seh-1.dll libwinpthread-1.dll; do
  if [[ -f "${mingw_bin}/${dll}" ]]; then
    cp -f "${mingw_bin}/${dll}" "${build_dir}/"
  fi
done

if command -v windeployqt.exe >/dev/null 2>&1; then
  windeployqt.exe "${executable}"
fi

if [[ "${mode}" == "build" ]]; then
  exit 0
fi

mail_client_run_executable "${executable}"
