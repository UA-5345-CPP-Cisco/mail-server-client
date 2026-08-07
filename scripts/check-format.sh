#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd "$(dirname "$0")" && pwd)"

bash "${script_dir}/check-clang-format.sh"
bash "${script_dir}/check-qml-format.sh"
