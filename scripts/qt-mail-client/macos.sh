#!/usr/bin/env bash
set -euo pipefail

mode="$1"

if [[ -z "${qt_prefix}" ]] && command -v brew >/dev/null 2>&1; then
  qt_prefix="$(brew --prefix qt 2>/dev/null || true)"
fi

qt_mail_client_configure_and_build

if [[ "${mode}" == "build" ]]; then
  exit 0
fi

qt_mail_client_run_executable "${build_dir}/appqtapptestmail.app/Contents/MacOS/appqtapptestmail"
