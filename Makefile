# Copyright (c) 2019 Tim Perkins

QMK_DIR := qmk_firmware
QMK_TMP_DIR := build/qmk_tmp
BUILD_DIR := build

all:
	mkdir -p "$(BUILD_DIR)"
	mkdir -p "$(QMK_TMP_DIR)/keyboards/massdrop"
	rsync -a "$(QMK_DIR)/Makefile" "$(QMK_TMP_DIR)"
	rsync -a --include='*.mk' --exclude='*' "$(QMK_DIR)/" "$(QMK_TMP_DIR)"
	rsync -a "$(QMK_DIR)/lib" "$(QMK_TMP_DIR)"
	rsync -a "$(QMK_DIR)/drivers" "$(QMK_TMP_DIR)"
	rsync -a "$(QMK_DIR)/quantum" "$(QMK_TMP_DIR)"
	rsync -a "$(QMK_DIR)/tmk_core" "$(QMK_TMP_DIR)"
	rsync -a "$(QMK_DIR)/keyboards/massdrop/ctrl" "$(QMK_TMP_DIR)/keyboards/massdrop"
	ln -r -f -T -s . "$(QMK_TMP_DIR)/keyboards/massdrop/ctrl/keymaps/tprk77"
	make -C "$(QMK_TMP_DIR)" massdrop/ctrl:tprk77
	cp -t "$(BUILD_DIR)" "$(QMK_TMP_DIR)/massdrop_ctrl_tprk77.bin"

clean:
	-rm -rf build
