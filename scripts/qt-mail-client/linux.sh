#!/usr/bin/env bash
set -euo pipefail

mode="$1"

if [[ -z "${qt_prefix}" ]]; then
  if command -v qmake6 >/dev/null 2>&1; then
    qt_prefix="$(qmake6 -query QT_INSTALL_PREFIX)"
  elif command -v qmake >/dev/null 2>&1; then
    qt_prefix="$(qmake -query QT_INSTALL_PREFIX)"
  fi
fi

qt_mail_client_configure_and_build

if [[ "${mode}" == "build" ]]; then
  exit 0
fi

qt_mail_client_run_executable "${build_dir}/appqtapptestmail"
