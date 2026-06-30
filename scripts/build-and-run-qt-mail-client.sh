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

cmake_args=(
  -S "${source_dir}"
  -B "${build_dir}"
  -G Ninja
  -DCMAKE_BUILD_TYPE=Debug
  -DCMAKE_CXX_STANDARD=17
  -DCMAKE_CXX_STANDARD_REQUIRED=ON
)

if command -v brew >/dev/null 2>&1; then
  qt_prefix="$(brew --prefix qt 2>/dev/null || true)"

  if [[ -n "${qt_prefix}" ]]; then
    cmake_args+=("-DCMAKE_PREFIX_PATH=${qt_prefix}")
  fi
fi

cmake "${cmake_args[@]}"
cmake --build "${build_dir}" --parallel

if [[ "${mode}" == "build" ]]; then
  exit 0
fi

app_bundle="${build_dir}/appqtapptestmail.app/Contents/MacOS/appqtapptestmail"
app_binary="${build_dir}/appqtapptestmail"

if [[ -x "${app_bundle}" ]]; then
  cd "${source_dir}"
  exec "${app_bundle}"
fi

if [[ -x "${app_binary}" ]]; then
  cd "${source_dir}"
  exec "${app_binary}"
fi

printf 'Qt mail client executable was not found in %s\n' "${build_dir}" >&2
exit 1
