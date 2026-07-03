#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd "$(dirname "$0")" && pwd)"
project_root="$(cd "${script_dir}/.." && pwd)"
build_dir="${project_root}/cmake-build-debug"

if ! command -v cmake >/dev/null 2>&1; then
  printf 'cmake was not found in PATH\n' >&2
  exit 1
fi

if [[ ! -f "${build_dir}/CMakeCache.txt" ]]; then
  if ! command -v ninja >/dev/null 2>&1; then
    printf 'ninja was not found in PATH\n' >&2
    exit 1
  fi

  cmake \
    -S "${project_root}" \
    -B "${build_dir}" \
    -G Ninja \
    -DCMAKE_BUILD_TYPE=Debug \
    -DMAIL_BUILD_TESTS=ON
fi

if ! grep -q '^MAIL_BUILD_TESTS:BOOL=ON$' "${build_dir}/CMakeCache.txt"; then
  printf 'Tests are not enabled in %s\n' "${build_dir}" >&2
  printf 'Configure once with: scripts/cmake-configure-debug.sh -DMAIL_BUILD_TESTS=ON\n' >&2
  exit 1
fi

cmake \
  --build "${build_dir}" \
  --target \
  mail_storage_tests \
  smtp_client_tests \
  logger_tests \
  thread_pool_tests \
  smtp_server_configuration_tests \
  --parallel

ctest \
  --test-dir "${build_dir}" \
  -LE cryptopp \
  --output-on-failure \
  "$@"
