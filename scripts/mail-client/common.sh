#!/usr/bin/env bash

mail_client_init_paths() {
  local caller_script_dir="$1"

  project_root="$(cd "${caller_script_dir}/.." && pwd)"
  source_dir="${project_root}/apps/mail-client"
  build_dir="${project_root}/cmake-build-mail-client"
  qt_prefix="${QT_PREFIX:-}"
}

mail_client_require_tool() {
  if ! command -v "$1" >/dev/null 2>&1; then
    printf '%s was not found in PATH\n' "$1" >&2
    exit 1
  fi
}

mail_client_parallel_arg() {
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

mail_client_configure_and_build() {
  mail_client_require_tool cmake
  mail_client_require_tool ninja

  local cmake_args=(
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

  if [[ -d "${project_root}/cmake-build-debug/_deps/boost-src" ]]; then
    cmake_args+=("-DFETCHCONTENT_SOURCE_DIR_BOOST=${project_root}/cmake-build-debug/_deps/boost-src")
  fi

  if [[ -d "${project_root}/cmake-build-debug/_deps/cryptopp_modern-src" ]]; then
    cmake_args+=(
      "-DFETCHCONTENT_SOURCE_DIR_CRYPTOPP_MODERN=${project_root}/cmake-build-debug/_deps/cryptopp_modern-src"
    )
  fi

  cmake "${cmake_args[@]}"
  cmake --build "${build_dir}" --target appqtapptestmail --parallel "$(mail_client_parallel_arg)"
}

mail_client_run_executable() {
  local executable="$1"

  if [[ ! -x "${executable}" ]]; then
    printf 'Qt mail client executable was not found: %s\n' "${executable}" >&2
    exit 1
  fi

  cd "${source_dir}"
  "${executable}"
}
