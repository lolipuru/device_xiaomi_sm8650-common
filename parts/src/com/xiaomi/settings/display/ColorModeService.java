/*
 * Copyright (C) 2023 Paranoid Android
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package com.xiaomi.settings.display;

import static android.provider.Settings.System.DISPLAY_COLOR_MODE;
import static com.xiaomi.settings.display.DfWrapper.DfParams;

import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.database.ContentObserver;
import android.hardware.display.AmbientDisplayConfiguration;
import android.os.Handler;
import android.os.IBinder;
import android.os.SystemProperties;
import android.os.UserHandle;
import android.provider.Settings;
import android.util.Log;

import java.util.Map;

import com.xiaomi.settings.display.DfWrapper.DfParams;

public class ColorModeService extends Service {
    private static final String TAG = "XiaomiPartsColorModeService";
    private static final boolean DEBUG = true;

    private static final int DEFAULT_COLOR_MODE = SystemProperties.getInt(
            "persist.sys.sf.native_mode", 0);

    private static final DfParams STANDARD_PARAMS = new DfParams(2, 2, 255);

    /* color mode -> displayfeature (mode, value, cookie) */
    private static final Map<Integer, DfParams> COLOR_MAP = Map.of(
        258 /* vivid */, new DfParams(0, 2, 255),
        256 /* saturated */, new DfParams(1, 2, 255),
        257 /* standard */, STANDARD_PARAMS,
        269 /* original */, new DfParams(26, 1, 0),
        268 /* p3 */, new DfParams(26, 2, 0),
        267 /* srgb */, new DfParams(26, 3, 0)
    );
    /* original/p3/srgb */
    private static final int EXPERT_MODE = 26;
    private static final DfParams EXPERT_PARAMS = new DfParams(26, 0, 10);

    private Handler mHandler = new Handler();
    private AmbientDisplayConfiguration mAmbientConfig;
    private boolean mIsDozing;

    private final ContentObserver mSettingObserver = new ContentObserver(mHandler) {
        @Override
        public void onChange(boolean selfChange) {
            Log.e(TAG, "SettingObserver: onChange");
            setCurrentColorMode();
        }
    };

    private final BroadcastReceiver mScreenStateReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            Log.e(TAG, "onReceive: " + intent.getAction());
            switch (intent.getAction()) {
                case Intent.ACTION_SCREEN_ON:
                    if (mIsDozing) {
                        mIsDozing = false;
                        mHandler.removeCallbacksAndMessages(null);
                        mHandler.postDelayed(() -> {
                            Log.e(TAG, "Was in AOD, restore color mode");
                            setCurrentColorMode();
                        }, 100);
                    }
                    break;
                case Intent.ACTION_SCREEN_OFF:
                    if (!mAmbientConfig.alwaysOnEnabled(UserHandle.USER_CURRENT)) {
                        Log.e(TAG, "AOD is not enabled");
                        mIsDozing = false;
                        break;
                    }
                    mIsDozing = true;
                    mHandler.removeCallbacksAndMessages(null);
                    Log.e(TAG, "Entered AOD, set color mode to standard");
                    DfWrapper.setDisplayFeature(STANDARD_PARAMS);
                    break;
            }
        }
    };

    @Override
    public void onCreate() {
        super.onCreate();
        Log.e(TAG, "onCreate");
        mAmbientConfig = new AmbientDisplayConfiguration(this);
        getContentResolver().registerContentObserver(Settings.System.getUriFor(DISPLAY_COLOR_MODE),
                    false, mSettingObserver, UserHandle.USER_CURRENT);
        IntentFilter screenStateFilter = new IntentFilter(Intent.ACTION_SCREEN_ON);
        screenStateFilter.addAction(Intent.ACTION_SCREEN_OFF);
        registerReceiver(mScreenStateReceiver, screenStateFilter);
        setCurrentColorMode();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.e(TAG, "onStartCommand");
        return START_STICKY;
    }

    @Override
    public void onDestroy() {
        Log.e(TAG, "onDestroy");
        getContentResolver().unregisterContentObserver(mSettingObserver);
        unregisterReceiver(mScreenStateReceiver);
        super.onDestroy();
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    private void setCurrentColorMode() {
        if (mIsDozing) {
            Log.e(TAG, "setCurrentColorMode: skip in AOD");
            return;
        }
        final int colorMode = Settings.System.getIntForUser(getContentResolver(),
                DISPLAY_COLOR_MODE, DEFAULT_COLOR_MODE, UserHandle.USER_CURRENT);
        if (!COLOR_MAP.containsKey(colorMode)) {
            Log.e(TAG, "setCurrentColorMode: " + colorMode + " is not in colorMap!");
            return;
        }
        final DfParams params = COLOR_MAP.get(colorMode);
        Log.e(TAG, "setCurrentColorMode: " + colorMode + ", params=" + params);
        if (params.mode == EXPERT_MODE) {
            DfWrapper.setDisplayFeature(EXPERT_PARAMS);
        }
        DfWrapper.setDisplayFeature(params);
    }
}