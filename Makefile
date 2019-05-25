# Copyright (c) 2019 Tim Perkins

QMK_DIR := qmk_firmware
QMK_TMP_DIR := build/qmk_tmp
BUILD_DIR := build

all:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(QMK_TMP_DIR)
	mkdir -p $(QMK_TMP_DIR)/keyboards
	cp -R -f -t $(QMK_TMP_DIR) $(QMK_DIR)/Makefile
	cp -R -f -t $(QMK_TMP_DIR) $(QMK_DIR)/*.mk
	cp -R -f -t $(QMK_TMP_DIR) $(QMK_DIR)/lib
	cp -R -f -t $(QMK_TMP_DIR) $(QMK_DIR)/drivers
	cp -R -f -t $(QMK_TMP_DIR) $(QMK_DIR)/quantum
	cp -R -f -t $(QMK_TMP_DIR) $(QMK_DIR)/tmk_core
	mkdir -p $(QMK_TMP_DIR)/keyboards/massdrop
	cp -R -f -t $(QMK_TMP_DIR)/keyboards/massdrop $(QMK_DIR)/keyboards/massdrop/ctrl
	ln -r -f -T -s . $(QMK_TMP_DIR)/keyboards/massdrop/ctrl/keymaps/tprk77
	make -C $(QMK_TMP_DIR) massdrop/ctrl:tprk77
	cp -t $(BUILD_DIR) $(QMK_TMP_DIR)/massdrop_ctrl_tprk77.bin

clean:
	-rm -rf build
