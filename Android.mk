#
# Copyright (C) 2024 The Android Open Source Project
#
# SPDX-License-Identifier: Apache-2.0
#

LOCAL_PATH := $(call my-dir)

ifneq ($(filter houji shennong aurora ruyi goku manet zorn,$(TARGET_DEVICE)),)
	include $(call all-makefiles-under,$(LOCAL_PATH))
	include $(CLEAR_VARS)

endif
