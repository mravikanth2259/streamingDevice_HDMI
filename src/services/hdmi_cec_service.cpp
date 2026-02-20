/**
 * @file hdmi_cec_service.cpp
 * @brief HDMI-CEC Service implementation
 */

#include "hdmi_cec_service.hpp"
#include "../hal/hdmi_cec_hal.hpp"
#include "../hal/hdmi_cec_hal.hpp"
#include "../common/logger.hpp"

namespace streaming::services {

class HdmiCecServiceImpl : public IHdmiCecService {
public:
    HdmiCecServiceImpl() : cec_(hal::createHdmiCecHal()) {}

    device::Result initialize() override {
        return cec_->initialize();
    }

    void shutdown() override {
        cec_->shutdown();
    }

    device::Result sendKeyToTv(device::RemoteKey key) override {
        if (!cec_->isTvPresent()) return device::Result::ERROR_NOT_FOUND;
        return cec_->sendRemoteKey(key);
    }

    device::Result powerOnTv() override {
        return cec_->sendPowerOn();
    }

    device::Result standbyTv() override {
        return cec_->sendStandby();
    }

    bool isTvPresent() const override {
        return cec_->isTvPresent();
    }

    device::Result processQueue() override {
        return device::Result::OK;
    }

private:
    std::unique_ptr<hal::IHdmiCecHal> cec_;
};

std::unique_ptr<IHdmiCecService> createHdmiCecService() {
    return std::make_unique<HdmiCecServiceImpl>();
}

} // namespace streaming::services
