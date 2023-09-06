/*
 * Copyright (C) 2023 Paranoid Android
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package com.xiaomi.settings.display;

import android.os.IHwBinder.DeathRecipient;
import android.util.Log;

import android.os.IBinder;
import android.os.ServiceManager;
import vendor.xiaomi.hardware.displayfeature_aidl.IDisplayFeature;

public class DfWrapper {

    private static final String TAG = "XiaomiPartsDisplayFeatureWrapper";

    private static IDisplayFeature mDisplayFeature;

    private static DeathRecipient mDeathRecipient = (cookie) -> {
        Log.d(TAG, "serviceDied");
        mDisplayFeature = null;
    };

    public static IDisplayFeature getDisplayFeature() {
        if (mDisplayFeature == null) {
            Log.d(TAG, "getDisplayFeature: mDisplayFeature=null");
            try {
                String name = "default";
                String fqName = IDisplayFeature.DESCRIPTOR + "/" + name;
                IBinder binder = android.os.Binder.allowBlocking(ServiceManager.waitForDeclaredService(fqName));
                mDisplayFeature = IDisplayFeature.Stub.asInterface(binder);
                mDisplayFeature.asBinder().linkToDeath((android.os.IBinder.DeathRecipient) mDeathRecipient, 0);
            } catch (Exception e) {
                Log.e(TAG, "getDisplayFeature failed!", e);
            }
        }
        return mDisplayFeature;
    }

    public static void setDisplayFeature(DfParams params) {
        final IDisplayFeature displayFeature = getDisplayFeature();
        if (displayFeature == null) {
            Log.e(TAG, "setDisplayFeatureParams: displayFeature is null!");
            return;
        }
        Log.d(TAG, "setDisplayFeatureParams: " + params);
        try {
            displayFeature.setFeature(0, params.mode, params.value, params.cookie);
        } catch (Exception e) {
            Log.e(TAG, "setDisplayFeatureParams failed!", e);
        }
    }

    public static class DfParams {
        /* displayfeature parameters */
        final int mode, value, cookie;

        public DfParams(int mode, int value, int cookie) {
            this.mode = mode;
            this.value = value;
            this.cookie = cookie;
        }

        public String toString() {
            return "DisplayFeatureParams(" + mode + ", " + value + ", " + cookie + ")";
        }
    }
}