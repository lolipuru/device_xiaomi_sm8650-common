/*
 * Copyright (C) 2022 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*
 * Changes from Qualcomm Innovation Center are provided under the following license:
 * Copyright (c) 2023 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 */

#include "BootControl.h"
#include <cstdint>

#include <android-base/logging.h>

#include <memory>

#include <log/log.h>

#include "BootControl.h"

using HIDLMergeStatus = ::android::hardware::boot::V1_1::MergeStatus;
using ndk::ScopedAStatus;

namespace aidl::android::hardware::boot {

BootControl::BootControl() {
    CHECK(bootcontrol_init());
}

ScopedAStatus BootControl::getActiveBootSlot(int32_t* _aidl_return) {
    int32_t ret  = get_active_boot_slot();
    if (ret < 0){
        *_aidl_return = 0;
    }
    else {
        *_aidl_return = ret;
    }
    return ScopedAStatus::ok();
}

ScopedAStatus BootControl::getCurrentSlot(int32_t* _aidl_return) {
    *_aidl_return = get_current_slot();
    return ScopedAStatus::ok();
}

ScopedAStatus BootControl::getNumberSlots(int32_t* _aidl_return) {
    *_aidl_return = get_number_slots();
    return ScopedAStatus::ok();
}

namespace {

static constexpr MergeStatus ToAIDLMergeStatus(HIDLMergeStatus status) {
    switch (status) {
        case HIDLMergeStatus::NONE:
            return MergeStatus::NONE;
        case HIDLMergeStatus::UNKNOWN:
            return MergeStatus::UNKNOWN;
        case HIDLMergeStatus::SNAPSHOTTED:
            return MergeStatus::SNAPSHOTTED;
        case HIDLMergeStatus::MERGING:
            return MergeStatus::MERGING;
        case HIDLMergeStatus::CANCELLED:
            return MergeStatus::CANCELLED;
        default:
            return MergeStatus::NONE;
    }
}

static constexpr HIDLMergeStatus ToHIDLMergeStatus(MergeStatus status) {
    switch (status) {
        case MergeStatus::NONE:
            return HIDLMergeStatus::NONE;
        case MergeStatus::UNKNOWN:
            return HIDLMergeStatus::UNKNOWN;
        case MergeStatus::SNAPSHOTTED:
            return HIDLMergeStatus::SNAPSHOTTED;
        case MergeStatus::MERGING:
            return HIDLMergeStatus::MERGING;
        case MergeStatus::CANCELLED:
            return HIDLMergeStatus::CANCELLED;
        default:
            return HIDLMergeStatus::NONE;
    }
}

}

ScopedAStatus BootControl::getSnapshotMergeStatus(MergeStatus* _aidl_return) {
    *_aidl_return = ToAIDLMergeStatus(get_snapshot_merge_status());
    return ScopedAStatus::ok();
}

ScopedAStatus BootControl::getSuffix(int32_t in_slot, std::string* _aidl_return) {
    if (!get_suffix(in_slot)) {
        // Old HIDL hal returns empty string for invalid slots. We should maintain this behavior in
        // AIDL for compatibility.
        _aidl_return->clear();
    }
    else {
        *_aidl_return = get_suffix(in_slot);
    }
    return ScopedAStatus::ok();
}

ScopedAStatus BootControl::isSlotBootable(int32_t in_slot, bool* _aidl_return) {
    int32_t ret = is_slot_bootable(in_slot);
    if (ret < 0) {
        return ScopedAStatus::fromServiceSpecificErrorWithMessage(INVALID_SLOT, (std::string("Invalid slot ") + std::to_string(in_slot)).c_str());
    }
    if (ret) {
        *_aidl_return = true;
    }
    else {
        *_aidl_return = false;
    }
    return ScopedAStatus::ok();
}

ScopedAStatus BootControl::isSlotMarkedSuccessful(int32_t in_slot, bool* _aidl_return) {
    int32_t ret = is_slot_marked_successful(in_slot);
    if (ret < 0) {
        return ScopedAStatus::fromServiceSpecificErrorWithMessage(INVALID_SLOT, (std::string("Invalid slot ") + std::to_string(in_slot)).c_str());
    }
    if (ret) {
        *_aidl_return = true;
    }
    else {
        *_aidl_return = false;
    }
    return ScopedAStatus::ok();
}

ScopedAStatus BootControl::markBootSuccessful() {
    int ret = mark_boot_successful();
    if (ret == 0) {
        return ScopedAStatus::ok();
    }
    else {
        return ScopedAStatus::fromServiceSpecificErrorWithMessage(COMMAND_FAILED, "Operation failed");
    }
}

ScopedAStatus BootControl::setActiveBootSlot(int32_t in_slot) {
    int ret = set_active_boot_slot(in_slot);
    if (ret == 0) {
        return ScopedAStatus::ok();
    }
    else {
        return ScopedAStatus::fromServiceSpecificErrorWithMessage(COMMAND_FAILED, "Operation failed");
    }
}

ScopedAStatus BootControl::setSlotAsUnbootable(int32_t in_slot) {
   int ret = set_slot_as_unbootable(in_slot);
   if (ret == 0) {
        return ScopedAStatus::ok();
   }
    else {
        return ScopedAStatus::fromServiceSpecificErrorWithMessage(COMMAND_FAILED, "Operation failed");
    }
}

ScopedAStatus BootControl::setSnapshotMergeStatus(MergeStatus in_status) {
    if (!set_snapshot_merge_status(ToHIDLMergeStatus(in_status))) {
        return ScopedAStatus::fromServiceSpecificErrorWithMessage(COMMAND_FAILED,
                                                                  "Operation failed");
    }
    return ScopedAStatus::ok();
}

}  // namespace aidl::android::hardware::boot
