/*
 * Copyright (C) 2026 EPAM Systems
 * SPDX-License-Identifier: Apache-2.0
 */

#include "V2_1/SubHal.h"

#include <unistd.h>

#include <string>
#include <vector>

namespace android {
namespace hardware {
namespace sensors {
namespace V2_1 {
namespace subhal {
namespace xenvm {

using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::hardware::hidl_handle;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::sensors::V1_0::OperationMode;
using ::android::hardware::sensors::V1_0::RateLevel;
using ::android::hardware::sensors::V1_0::Result;
using ::android::hardware::sensors::V1_0::SharedMemInfo;
using ::android::hardware::sensors::V2_1::Event;
using ::android::hardware::sensors::V2_1::SensorInfo;
using ::android::hardware::sensors::V2_1::implementation::IHalProxyCallback;
using ::android::hardware::sensors::V2_1::implementation::ISensorsSubHal;

class SensorsPolicySubHal final : public ISensorsSubHal {
  public:
    Return<void> getSensorsList_2_1(getSensorsList_2_1_cb callback) override {
        callback(std::vector<SensorInfo>{});
        return Void();
    }

    Return<Result> setOperationMode(OperationMode mode) override {
        if (mode == OperationMode::DATA_INJECTION) {
            return Result::BAD_VALUE;
        }
        return mode == OperationMode::NORMAL ? Result::OK : Result::BAD_VALUE;
    }

    Return<Result> activate(int32_t /* sensorHandle */, bool /* enabled */) override {
        return Result::BAD_VALUE;
    }

    Return<Result> batch(int32_t /* sensorHandle */, int64_t /* samplingPeriodNs */,
                         int64_t /* maxReportLatencyNs */) override {
        return Result::BAD_VALUE;
    }

    Return<Result> flush(int32_t /* sensorHandle */) override { return Result::BAD_VALUE; }

    Return<Result> injectSensorData_2_1(const Event& /* event */) override {
        return Result::BAD_VALUE;
    }

    Return<void> registerDirectChannel(const SharedMemInfo& /* mem */,
                                       registerDirectChannel_cb callback) override {
        callback(Result::INVALID_OPERATION, -1);
        return Void();
    }

    Return<Result> unregisterDirectChannel(int32_t /* channelHandle */) override {
        return Result::INVALID_OPERATION;
    }

    Return<void> configDirectReport(int32_t /* sensorHandle */, int32_t /* channelHandle */,
                                    RateLevel /* rate */, configDirectReport_cb callback) override {
        callback(Result::INVALID_OPERATION, 0);
        return Void();
    }

    Return<void> debug(const hidl_handle& fd, const hidl_vec<hidl_string>& /* args */) override {
        if (fd.getNativeHandle() != nullptr && fd->numFds > 0) {
            dprintf(fd->data[0],
                    "Xenvm sensors policy sub-HAL: DATA_INJECTION is not supported.\n");
        }
        return Void();
    }

    const std::string getName() override { return "Xenvm-SensorsPolicySubHal"; }

    Return<Result> initialize(const sp<IHalProxyCallback>& /* halProxyCallback */) override {
        return Result::OK;
    }
};

}  // namespace xenvm
}  // namespace subhal
}  // namespace V2_1
}  // namespace sensors
}  // namespace hardware
}  // namespace android

extern "C" __attribute__((visibility("default")))
android::hardware::sensors::V2_1::implementation::ISensorsSubHal* sensorsHalGetSubHal_2_1(
        uint32_t* version) {
    static android::hardware::sensors::V2_1::subhal::xenvm::SensorsPolicySubHal subHal;
    *version = SUB_HAL_2_1_VERSION;
    return &subHal;
}
