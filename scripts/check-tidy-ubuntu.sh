#!/usr/bin/env bash
set -euo pipefail

readonly SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
readonly PROJECT_ROOT="$(cd -- "${SCRIPT_DIR}/.." && pwd)"
build_dir="${1:-${PROJECT_ROOT}/build/ubuntu-dev}"
tidy="${CLANG_TIDY:-clang-tidy-18}"

command -v "${tidy}" >/dev/null 2>&1 || {
    printf 'ERROR: %s is required. Run scripts/bootstrap-ubuntu.sh.\n' "${tidy}" >&2
    exit 1
}
[[ -f "${build_dir}/compile_commands.json" ]] || {
    printf 'ERROR: %s/compile_commands.json is missing. Configure the Ubuntu preset first.\n' "${build_dir}" >&2
    exit 1
}
"${tidy}" -p "${build_dir}" \
    "${PROJECT_ROOT}/src/core/project.cpp" \
    "${PROJECT_ROOT}/tests/unit/project_smoke_test.cpp" \
    "${PROJECT_ROOT}/tests/integration/target_graph_smoke_test.cpp"
