#!/bin/bash

# Copyright (c) 2019 Tim Perkins

set -o errexit
set -o nounset
set -o pipefail
IFS=$'\n\t'

readonly SCRIPT_DIR="$(dirname "$(readlink -f "$0")")"
cd "${SCRIPT_DIR}"

readonly FIRMWARE_FNAME="${SCRIPT_DIR}/build/massdrop_ctrl_tprk77.bin"
readonly MDLOADER_DIR="${SCRIPT_DIR}/mdloader_util"

if [ ! -f "${FIRMWARE_FNAME}" ]; then
    echo "ERROR: Firmware file is missing!" 1>&2
    exit 1
fi

# NOTE The mdloader utility requires the applet binary file to be located in
# the current working directory. Meaning we need to temporarily change our path
# over to it when we execute the command.

(cd "${MDLOADER_DIR}" && ./mdloader_linux --first --download "${FIRMWARE_FNAME}" --restart)

exit 0
