#
# Copyright (C) 2024 The Android Open Source Project
#
# SPDX-License-Identifier: Apache-2.0
#

LOCAL_PATH := $(call my-dir)

ifneq ($(filter houji shennong aurora ruyi goku manet zorn,$(TARGET_DEVICE)),)
	include $(call all-makefiles-under,$(LOCAL_PATH))
	include $(CLEAR_VARS)

RFS_MSM_MPSS_SYMLINKS := $(TARGET_OUT_VENDOR)/rfs/msm/mpss
$(RFS_MSM_MPSS_SYMLINKS): $(LOCAL_INSTALLED_MODULE)
	@echo "Creating RFS MSM MPSS folder structure: $@"
	@rm -rf $@/*
	@mkdir -p $@/readonly/vendor
	$(hide) ln -sf /data/vendor/tombstones/rfs/modem $@/ramdumps
	$(hide) ln -sf /mnt/vendor/persist/rfs/msm/mpss $@/readwrite
	$(hide) ln -sf /mnt/vendor/persist/rfs/shared $@/shared
	$(hide) ln -sf /mnt/vendor/persist/hlos_rfs/shared $@/hlos
	$(hide) ln -sf /data/vendor/mbnconfig $@/readonly/mbnconfig
	$(hide) ln -sf /vendor/modem_firmware $@/readonly/firmware
	$(hide) ln -sf /vendor/firmware $@/readonly/vendor/firmware

ALL_DEFAULT_INSTALLED_MODULES += \
	$(RFS_MSM_MPSS_SYMLINKS)

endif
