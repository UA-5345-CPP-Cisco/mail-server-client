#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd "$(dirname "$0")" && pwd)"
platform=""
mode="run"

usage() {
  printf 'Usage: %s [--mac|--linux|--windows] [run|build]\n' "$0" >&2
}

while [[ "$#" -gt 0 ]]; do
  case "$1" in
    --mac|--macos)
      platform="macos"
      ;;
    --linux)
      platform="linux"
      ;;
    --windows|--win)
      platform="windows"
      ;;
    run|build)
      mode="$1"
      ;;
    -h|--help)
      usage
      exit 0
      ;;
    *)
      usage
      exit 1
      ;;
  esac

  shift
done

if [[ -z "${platform}" ]]; then
  case "$(uname -s)" in
    Darwin*)
      platform="macos"
      ;;
    Linux*)
      platform="linux"
      ;;
    MINGW*|MSYS*|CYGWIN*)
      platform="windows"
      ;;
    *)
      printf 'Unsupported platform: %s\n' "$(uname -s)" >&2
      exit 1
      ;;
  esac
fi

source "${script_dir}/qt-mail-client/common.sh"
qt_mail_client_init_paths "${script_dir}"

case "${platform}" in
  macos)
    source "${script_dir}/qt-mail-client/macos.sh" "${mode}"
    ;;
  linux)
    source "${script_dir}/qt-mail-client/linux.sh" "${mode}"
    ;;
  windows)
    source "${script_dir}/qt-mail-client/windows.sh" "${mode}"
    ;;
esac
