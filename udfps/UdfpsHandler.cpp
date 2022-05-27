/*
 * Copyright (C) 2022 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "UdfpsHander.xiaomi_sm8550"

#include "UdfpsHandler.h"

#include <android-base/logging.h>
#include <android-base/unique_fd.h>
#include <fcntl.h>
#include <poll.h>
#include <thread>
#include <unistd.h>

#define COMMAND_NIT 10
#define PARAM_NIT_FOD 1
#define PARAM_NIT_NONE 0

#define FOD_STATUS_ON 1
#define FOD_STATUS_OFF -1

// defines from drivers/input/touchscreen/xiaomi/xiaomi_touch.h
#define SET_CUR_VALUE 0
#define Touch_Fod_Enable 10

#define TOUCH_DEV_PATH "/dev/xiaomi-touch"
#define TOUCH_ID 0
#define TOUCH_MAGIC 0x5400
#define TOUCH_IOC_SETMODE TOUCH_MAGIC + SET_CUR_VALUE

class XiaomiSm8550UdfpsHander : public UdfpsHandler {
  public:
    void init(fingerprint_device_t *device) {
        mDevice = device;
        touch_fd_ = android::base::unique_fd(open(TOUCH_DEV_PATH, O_RDWR));
    }

    void onFingerDown(uint32_t /*x*/, uint32_t /*y*/, float /*minor*/, float /*major*/) {
        int arg[3] = {TOUCH_ID, Touch_Fod_Enable, FOD_STATUS_ON};
        ioctl(touch_fd_.get(), TOUCH_IOC_SETMODE, &arg);

        mDevice->extCmd(mDevice, COMMAND_NIT, PARAM_NIT_FOD);
    }

    void onFingerUp() {
        mDevice->extCmd(mDevice, COMMAND_NIT, PARAM_NIT_NONE);

        int arg[3] = {TOUCH_ID, Touch_Fod_Enable, FOD_STATUS_OFF};
        ioctl(touch_fd_.get(), TOUCH_IOC_SETMODE, &arg);
    }

  private:
    fingerprint_device_t *mDevice;
    android::base::unique_fd touch_fd_;
};

static UdfpsHandler* create() {
    return new XiaomiSm8550UdfpsHander();
}

static void destroy(UdfpsHandler* handler) {
    delete handler;
}

extern "C" UdfpsHandlerFactory UDFPS_HANDLER_FACTORY = {
    .create = create,
    .destroy = destroy,
};
