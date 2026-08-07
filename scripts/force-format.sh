#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd "$(dirname "$0")" && pwd)"

bash "${script_dir}/force-clang-format.sh"
bash "${script_dir}/force-qml-format.sh"
