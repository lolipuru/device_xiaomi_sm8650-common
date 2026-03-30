//
// SPDX-FileCopyrightText: The LineageOS Project
// SPDX-License-Identifier: Apache-2.0
//

package vendor.xiaomi.hardware.displayfeature_aidl;

import vendor.xiaomi.hardware.displayfeature_aidl.IDisplayFeatureCallback;

@VintfStability
interface IDisplayFeature {
    void notifyBrightness(int brightness);
    void registerCallback(int displayId, IDisplayFeatureCallback callback);
    void sendMessage(int what, int arg1, String payload);
    void sendPanelCommand(String command);
    void sendPostProcCommand(int command, int value);
    void sendRefreshCommand();
    void setFeature(int displayId, int mode, int value, int cookie);
    void setFunction(int displayId, int mode, int value, int cookie);
    void sendGamePkgName(int displayId, int mode, int state, String packageName);
}
