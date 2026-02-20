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
    device::Result sendUserControl(hal::CecUserControlCode key) override;
    device::Result sendRemoteKey(device::RemoteKey key) override;
    void setMessageCallback(hal::CecMessageCallback callback) override;
    device::CecLogicalAddress getLogicalAddress() const override;
    bool isTvPresent() const override;

    /** Last sent CEC opcode (from sendPowerOn/sendStandby) */
    hal::CecOpcode getLastSentOpcode() const { return last_sent_opcode_; }
    /** Last sent user control code (from sendUserControl) */
    hal::CecUserControlCode getLastSentUserControl() const { return last_sent_user_control_; }

private:
    bool tv_present_{false};
    hal::CecOpcode last_sent_opcode_{hal::CecOpcode::STANDBY};
    hal::CecUserControlCode last_sent_user_control_{hal::CecUserControlCode::SELECT};
    hal::CecMessageCallback msg_cb_;
};

} // namespace streaming::drivers::mock
