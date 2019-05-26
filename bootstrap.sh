#!/bin/bash

# Copyright (c) 2019 Tim Perkins

set -o errexit
set -o nounset
set -o pipefail
IFS=$'\n\t'

readonly SCRIPT_DIR="$(dirname "$(readlink -f "$0")")"
cd "${SCRIPT_DIR}"

####################
# INSTALL PACKAGES #
####################

NEEDS_INSTALL_PACKAGES=()
NEEDED_PACKAGES=(
    "build-essential"
    "binutils-arm-none-eabi"
    "gcc"
    "gcc-arm-none-eabi"
    "libnewlib-arm-none-eabi"
    "libnewlib-dev"
    "rsync"
)

for package in ${NEEDED_PACKAGES[@]}; do
    SFORMAT='${db:Status-Abbrev}\n'
    installed_text="$(dpkg-query --show -f "${SFORMAT}" "${package}" 2>/dev/null || true)"
    if ! egrep -q "^ii" <<< "${installed_text}"; then
        NEEDS_INSTALL_PACKAGES+=("${package}")
    fi
done

if [ "${#NEEDS_INSTALL_PACKAGES[@]}" -gt 0 ]; then
    echo ""
    echo "############################################################"
    echo ""
    echo "Some packages need to be installed:"
    echo ""
    for package in ${NEEDS_INSTALL_PACKAGES[@]}; do
        echo "    * ${package}"
    done
    echo ""
    echo "############################################################"
    echo ""
    sleep 1
    if ! sudo apt-get install -y "${NEEDS_INSTALL_PACKAGES[@]}"; then
        echo "" 1>&2
        echo "ERROR: Packages NOT installed! Aborting!" 1>&2
        exit 1
    fi
fi

#######################
# FIX BUSTED PACKAGES #
#######################

NEEDS_FIX_PACKAGES=()
BROKEN_PACKAGES=()

# Check for Ubuntu 18.04 and broken packages
if grep -q "18.04" <<< "$(lsb_release -r)"; then
    BROKEN_PACKAGES=(
        "libnewlib-arm-none-eabi"
        "libnewlib-dev"
    )
fi

for package in ${BROKEN_PACKAGES[@]}; do
    SFORMAT='${Version}\n'
    version_text="$(dpkg-query --show -f "${SFORMAT}" "${package}" 2>/dev/null || true)"
    version_major="$(grep -o "^[0-9]\+" <<< "${version_text}")"
    if [ "${version_major}" -lt 3 ]; then
        echo "WARNING: ${package} is less than version 3.0.0" \
             "(detected version ${version_text})" 1>&2
        NEEDS_FIX_PACKAGES+=("${package}")
    fi
done

if [ "${#NEEDS_FIX_PACKAGES[@]}" -gt 0 ]; then
    echo ""
    echo "############################################################"
    echo ""
    echo "Some packages need fixed versions:"
    echo ""
    for package in ${NEEDS_FIX_PACKAGES[@]}; do
        echo "    * ${package}"
    done
    echo ""
    echo "See also: https://github.com/qmk/qmk_firmware/issues/4209"
    echo ""
    echo "############################################################"
    echo ""
    FIXED_PACKAGES=()
    for package in ${NEEDS_FIX_PACKAGES[@]}; do
        # Get the corresponding package from the fixed_packages directory
        package_fname="$(find "fixed_packages" -maxdepth 1 -name "${package}*.deb" | head -n 1)"
        if [ -z "${package_fname}" ]; then
            echo "ERROR: Cannot find fixed package for ${package}! Aborting!" 1>&2
            exit 1
        else
            FIXED_PACKAGES+=("${package_fname}")
        fi
    done
    sleep 1
    if ! eval "sudo dpkg -i ${FIXED_PACKAGES[@]}"; then
        echo "" 1>&2
        echo "ERROR: Packages NOT installed! Aborting!" 1>&2
        exit 1
    fi
fi

#########################
# UPDATE GIT SUBMODULES #
#########################

git submodule sync --recursive
git submodule update --init --recursive

exit 0
