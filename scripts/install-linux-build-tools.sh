#!/usr/bin/env bash
set -euo pipefail

readonly CMAKE_VERSION="4.5.0"
readonly CMAKE_SHA256="67a05954850976717d14ae9bd6fea305a084c650c8eb58c69b8fb92231ee39cb"
readonly NINJA_VERSION="1.13.2"
readonly NINJA_SHA256="5749cbc4e668273514150a80e387a957f933c6ed3f5f11e03fb30955e2bbead6"
readonly INSTALL_DIR="${HOME}/.local"
readonly LOCAL_BIN="${INSTALL_DIR}/bin"
readonly CMAKE_DATA="${INSTALL_DIR}/share/cmake-${CMAKE_VERSION%.*}"

cmake_is_valid() {
    [[ -x "${LOCAL_BIN}/cmake" && -x "${LOCAL_BIN}/ctest" && -x "${LOCAL_BIN}/cpack" ]] ||
        return 1
    [[ -f "${CMAKE_DATA}/Modules/CMake.cmake" ]] || return 1
    "${LOCAL_BIN}/cmake" --version | grep -Fq "cmake version ${CMAKE_VERSION}" || return 1
    "${LOCAL_BIN}/cmake" --help >/dev/null 2>&1
}

cmake_installer=""
ninja_archive=""
ninja_temp=""
cleanup() {
    [[ -z "${cmake_installer}" ]] || rm -f -- "${cmake_installer}"
    [[ -z "${ninja_archive}" ]] || rm -f -- "${ninja_archive}"
    [[ -z "${ninja_temp}" ]] || rm -f -- "${ninja_temp}"
}
trap cleanup EXIT

if cmake_is_valid; then
    printf 'Reusing verified CMake %s installation.\n' "${CMAKE_VERSION}"
else
    cmake_installer="$(mktemp)"
    curl -fsSL \
        "https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-linux-x86_64.sh" \
        -o "${cmake_installer}"
    printf '%s  %s\n' "${CMAKE_SHA256}" "${cmake_installer}" | sha256sum --check

    mkdir -p "${INSTALL_DIR}/bin"
    sh "${cmake_installer}" --skip-license --prefix="${INSTALL_DIR}"
fi

if [[ -x "${LOCAL_BIN}/ninja" ]] &&
   [[ "$("${LOCAL_BIN}/ninja" --version 2>/dev/null || true)" == "${NINJA_VERSION}" ]]; then
    printf 'Reusing verified Ninja %s installation.\n' "${NINJA_VERSION}"
else
    ninja_archive="$(mktemp)"
    curl -fsSL \
        "https://github.com/ninja-build/ninja/releases/download/v${NINJA_VERSION}/ninja-linux.zip" \
        -o "${ninja_archive}"
    printf '%s  %s\n' "${NINJA_SHA256}" "${ninja_archive}" | sha256sum --check
    ninja_temp="$(mktemp "${LOCAL_BIN}/.ninja.XXXXXX")"
    unzip -p "${ninja_archive}" ninja > "${ninja_temp}"
    chmod 755 "${ninja_temp}"
    mv -f -- "${ninja_temp}" "${LOCAL_BIN}/ninja"
    ninja_temp=""
fi

if [[ -n "${GITHUB_PATH:-}" ]]; then
    printf '%s\n' "${LOCAL_BIN}" >> "${GITHUB_PATH}"
fi
export PATH="${LOCAL_BIN}:${PATH}"

cmake_is_valid
"${LOCAL_BIN}/cmake" --version | grep -F "cmake version ${CMAKE_VERSION}"
[[ "$("${LOCAL_BIN}/ninja" --version)" == "${NINJA_VERSION}" ]]
