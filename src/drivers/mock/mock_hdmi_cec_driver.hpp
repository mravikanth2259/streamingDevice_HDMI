#pragma once

#include "../../hal/hdmi_cec_hal.hpp"

namespace streaming::drivers::mock {

class MockHdmiCecDriver : public hal::IHdmiCecHal {
public:
    device::Result initialize() override;
    device::Result shutdown() override;
    device::Result sendMessage(const hal::CecMessage& msg) override;
    device::Result sendPowerOn() override;
    device::Result sendStandby() override;
    device::Result sendUserControl(hal::CecOpcode key) override;
    device::Result sendRemoteKey(device::RemoteKey key) override;
    void setMessageCallback(hal::CecMessageCallback callback) override;
    device::CecLogicalAddress getLogicalAddress() const override;
    bool isTvPresent() const override;

    hal::CecOpcode getLastSent() const { return last_sent_; }

private:
    bool tv_present_{false};
    hal::CecOpcode last_sent_{hal::CecOpcode::STANDBY};
    hal::CecMessageCallback msg_cb_;
};

} // namespace streaming::drivers::mock
