//
// SPDX-FileCopyrightText: The LineageOS Project
// SPDX-License-Identifier: Apache-2.0
//

package vendor.xiaomi.hw.touchfeature;

import vendor.xiaomi.hw.touchfeature.ITouchFeatureCallback;

@VintfStability
interface ITouchFeature {
    int getModeCurValue(int type, int mode);
    int getModeDefaultValue(int type, int mode);
    int getModeMaxValue(int type, int mode);
    int getModeMinValue(int type, int mode);
    int[] getModeValue(int type, int mode);
    String getTouchEvent();
    int modeReset(int type, int mode);
    int setModeLongValue(int type, int mode, int len, in int[] values);
    int setModeValue(int type, int mode, int value);
    String getModeCurValueString(int type, int mode);
    String getModeWhiteList(int type, in int[] modes);
    int registerCallback(int type, ITouchFeatureCallback callback, String name);
    int unregisterCallback(int type, ITouchFeatureCallback callback, String name);
    String setModePackageName(int type, int mode, String packageName);
}
