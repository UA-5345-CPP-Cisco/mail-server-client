#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd "$(dirname "$0")" && pwd)"
project_root="$(cd "${script_dir}/.." && pwd)"
build_dir="${project_root}/cmake-build-debug"

if ! command -v cmake >/dev/null 2>&1; then
  printf 'cmake was not found in PATH\n' >&2
  exit 1
fi

if ! command -v ninja >/dev/null 2>&1; then
  printf 'ninja was not found in PATH\n' >&2
  exit 1
fi

cmake \
  -S "${project_root}" \
  -B "${build_dir}" \
  -G Ninja \
  -DCMAKE_BUILD_TYPE=Debug \
  "$@"
