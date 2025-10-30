#include <aidl/xiaomi/system/hypsys/common/BnHypSysSsi.h>
#include <aidl/xiaomi/system/hypsys/common/BnHypSysSsiIntl.h>
#include <aidl/xiaomi/system/hypsys/common/IHypSysSsi.h>
#include <aidl/xiaomi/system/hypsys/common/IHypSysSsiIntl.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>
#include <memory>
#include <string>
#include <vector>

using ::aidl::xiaomi::system::hypsys::common::BnHypSysSsi;
using ::aidl::xiaomi::system::hypsys::common::BnHypSysSsiIntl;
using ::aidl::xiaomi::system::hypsys::common::IHypSysSsi;
using ::aidl::xiaomi::system::hypsys::common::IHypSysSsiIntl;
using ::ndk::ScopedAStatus;

struct HypSysSsiService : public BnHypSysSsi {
    ScopedAStatus reportSimpleEvent(int32_t eventId, const std::string& payload) override {
        return ScopedAStatus::ok();
    }
    ScopedAStatus reportEventV2(const std::string& category, const std::string& action,
                                const std::string& label, bool flag) override {
        return ScopedAStatus::ok();
    }
    ScopedAStatus reportNativeFeatureEvent(const std::string& feature, const std::string& action,
                                           const std::string& details) override {
        return ScopedAStatus::ok();
    }
    ScopedAStatus appCaptureLog(const std::string& tag, const std::string& level,
                                const std::string& message, const std::string& extras) override {
        return ScopedAStatus::ok();
    }
    ScopedAStatus reportEventsV2(const std::string& category,
                                 const std::vector<std::string>& events, const std::string& label,
                                 bool flag) override {
        return ScopedAStatus::ok();
    }
    ScopedAStatus reportOneTrackEvent(const std::string& track, const std::string& key,
                                      const std::string& value, int32_t flags,
                                      int32_t* _aidl_return) override {
        *_aidl_return = 0;
        return ScopedAStatus::ok();
    }
    ScopedAStatus reportOneTrackEvents(const std::string& track, const std::string& key,
                                       std::vector<std::string>* values, int32_t flags,
                                       int32_t* _aidl_return) override {
        // values is inout: you may modify it before returning
        *_aidl_return = 0;
        return ScopedAStatus::ok();
    }
};

struct HypSysSsiIntlService : public BnHypSysSsiIntl {
    ScopedAStatus transferMessage(const std::string& feature, const std::string& action) override {
        return ScopedAStatus::ok();
    }
};

int main(int, char**) {
    ABinderProcess_setThreadPoolMaxThreadCount(0);

    auto ssi = ndk::SharedRefBase::make<HypSysSsiService>();
    auto ssi_intl = ndk::SharedRefBase::make<HypSysSsiIntlService>();

    const std::string ssi_instance = std::string(IHypSysSsi::descriptor) + "/default";
    const std::string ssi_intl_instance = std::string(IHypSysSsiIntl::descriptor) + "/default";

    if (AServiceManager_addService(ssi->asBinder().get(), ssi_instance.c_str()) != STATUS_OK) {
        return 1;
    }
    if (AServiceManager_addService(ssi_intl->asBinder().get(), ssi_intl_instance.c_str()) !=
        STATUS_OK) {
        return 1;
    }

    ABinderProcess_joinThreadPool();
    return 0;
}
