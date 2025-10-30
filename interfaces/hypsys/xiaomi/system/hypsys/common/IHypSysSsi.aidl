//
// SPDX-FileCopyrightText: The LineageOS Project
// SPDX-License-Identifier: Apache-2.0
//

package xiaomi.system.hypsys.common;

@VintfStability
interface IHypSysSsi {
    void reportSimpleEvent(int eventId, String payload);
    void reportEventV2(String category, String action, String label, boolean bool);
    void reportNativeFeatureEvent(String feature, String action, String details);
    void appCaptureLog(String tag, String level, String message, String extras);
    void reportEventsV2(String category, in List<String> events, String label, boolean bool);
    int reportOneTrackEvent(String track, String key, String value, int flags);
    int reportOneTrackEvents(String track, String key, inout List<String> values, int flags);
}
