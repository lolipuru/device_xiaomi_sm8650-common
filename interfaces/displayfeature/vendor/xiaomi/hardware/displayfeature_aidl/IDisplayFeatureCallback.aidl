//
// SPDX-FileCopyrightText: The LineageOS Project
// SPDX-License-Identifier: Apache-2.0
//

package vendor.xiaomi.hardware.displayfeature_aidl;

@VintfStability
interface IDisplayFeatureCallback {
    void displayfeatureInfoChanged(int category, int type, float value1, float value2, float value3);
}
