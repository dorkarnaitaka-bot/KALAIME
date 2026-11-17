#!/usr/bin/env bash
set -euo pipefail

readonly LLVM_VERSION="23.1.5"
readonly LLVM_SHA256="c3107384835f788768f614db610e87fba7077f125ca950eb02a78bab0a308065"
readonly INSTALL_ROOT="${HOME}/.local/opt"
readonly LLVM_ROOT="${INSTALL_ROOT}/LLVM-${LLVM_VERSION}-Linux-X64"
readonly LLVM_BIN="${LLVM_ROOT}/bin"
readonly LOCAL_BIN="${HOME}/.local/bin"
readonly -a LLVM_TOOLS=(
    clang clang++ clangd clang-format clang-tidy lld ld.lld
    llvm-ar llvm-ranlib llvm-nm llvm-objcopy llvm-objdump llvm-strip llvm-readelf llvm-symbolizer
)

command -v zstd >/dev/null 2>&1 || {
    printf 'ERROR: zstd is required to extract the official LLVM release.\n' >&2
    exit 1
}

llvm_installation_is_valid() {
    for tool in "${LLVM_TOOLS[@]}"; do
        [[ -x "${LLVM_BIN}/${tool}" ]] || return 1
        "${LLVM_BIN}/${tool}" --version 2>&1 | grep -Fq "${LLVM_VERSION}" || return 1
    done
}

link_tool() {
    local source="$1"
    local target="$2"
    local expected
    expected="$(readlink -f "${source}")"
    if [[ -e "${target}" || -L "${target}" ]]; then
        [[ "$(readlink -f "${target}")" == "${expected}" ]] || {
            printf 'ERROR: refusing to replace existing tool link at %s.\n' "${target}" >&2
            return 1
        }
    else
        ln -s "${source}" "${target}"
    fi
}

link_llvm_tools() {
    mkdir -p "${LOCAL_BIN}"
    for tool in "${LLVM_TOOLS[@]}"; do
        link_tool "${LLVM_BIN}/${tool}" "${LOCAL_BIN}/${tool}-23"
    done
    link_tool "${LLVM_BIN}/ld.lld" "${LOCAL_BIN}/ld.lld"
}

if llvm_installation_is_valid; then
    printf 'Reusing verified LLVM %s installation at %s.\n' "${LLVM_VERSION}" "${LLVM_ROOT}"
    link_llvm_tools
    export PATH="${LOCAL_BIN}:${PATH}"
    if [[ -n "${GITHUB_PATH:-}" ]]; then
        printf '%s\n' "${LOCAL_BIN}" >> "${GITHUB_PATH}"
    fi
    clang-23 --version | grep -F "${LLVM_VERSION}"
    clang++-23 --version | grep -F "${LLVM_VERSION}"
    clang-format-23 --version | grep -F "${LLVM_VERSION}"
    clang-tidy-23 --version | grep -F "${LLVM_VERSION}"
    clangd-23 --version | grep -F "${LLVM_VERSION}"
    lld-23 --version | grep -F "${LLVM_VERSION}"
    ld.lld-23 --version | grep -F "${LLVM_VERSION}"
    llvm-ar-23 --version | grep -F "${LLVM_VERSION}"
    llvm-ranlib-23 --version | grep -F "${LLVM_VERSION}"
    exit 0
elif [[ -e "${LLVM_ROOT}" ]]; then
    if [[ "${KALAIME_REPAIR_CACHED_TOOLCHAINS:-0}" == 1 ]]; then
        printf 'Cached LLVM installation is incomplete; replacing %s.\n' "${LLVM_ROOT}" >&2
        rm -rf -- "${LLVM_ROOT}"
    else
        printf 'ERROR: refusing to overwrite the existing LLVM installation at %s.\n' \
            "${LLVM_ROOT}" >&2
        exit 1
    fi
fi

archive="$(mktemp)"
file_list="$(mktemp)"
trap 'rm -f -- "${archive}" "${file_list}"' EXIT
if [[ -n "${LLVM_ARCHIVE:-}" ]]; then
    cp -- "${LLVM_ARCHIVE}" "${archive}"
else
    curl -fsSL \
        "https://github.com/llvm/llvm-project/releases/download/llvmorg-${LLVM_VERSION}/LLVM-${LLVM_VERSION}-Linux-X64.tar.zst" \
        -o "${archive}"
fi
printf '%s  %s\n' "${LLVM_SHA256}" "${archive}" | sha256sum --check

archive_root="$(zstd --long=30 -dc "${archive}" | tar -tf - | sed -n '1{s#/.*##;p;}')"
[[ -n "${archive_root}" && "${archive_root}" != */* ]] || {
    printf 'ERROR: the LLVM release archive has an unexpected layout.\n' >&2
    exit 1
}
[[ "${archive_root}" == "$(basename -- "${LLVM_ROOT}")" ]] || {
    printf 'ERROR: LLVM archive root %s does not match expected %s.\n' \
        "${archive_root}" "$(basename -- "${LLVM_ROOT}")" >&2
    exit 1
}
zstd --long=30 -dc "${archive}" | tar -tf - |
    grep -E "^${archive_root}/(bin/(clang|clang\\+\\+|clangd|clang-tidy|clang-format|lld|ld\\.lld|llvm-(ar|ranlib|nm|objcopy|objdump|strip|readelf|symbolizer))(-[0-9]+)?|lib/lib(clang|LLVM)[^/]*\\.so[^/]*|lib/clang/23/)" \
    | grep -v '/$' \
        > "${file_list}"
mkdir -p "${INSTALL_ROOT}"
zstd --long=30 -dc "${archive}" | tar -xf - -C "${INSTALL_ROOT}" -T "${file_list}"
llvm_installation_is_valid || {
    printf 'ERROR: the extracted LLVM installation failed version verification.\n' >&2
    exit 1
}
link_llvm_tools

export PATH="${LOCAL_BIN}:${PATH}"
if [[ -n "${GITHUB_PATH:-}" ]]; then
    printf '%s\n' "${LOCAL_BIN}" >> "${GITHUB_PATH}"
fi

clang-23 --version | grep -F "${LLVM_VERSION}"
clang++-23 --version | grep -F "${LLVM_VERSION}"
clang-format-23 --version | grep -F "${LLVM_VERSION}"
clang-tidy-23 --version | grep -F "${LLVM_VERSION}"
clangd-23 --version | grep -F "${LLVM_VERSION}"
lld-23 --version | grep -F "${LLVM_VERSION}"
ld.lld-23 --version | grep -F "${LLVM_VERSION}"
llvm-ar-23 --version | grep -F "${LLVM_VERSION}"
llvm-ranlib-23 --version | grep -F "${LLVM_VERSION}"
