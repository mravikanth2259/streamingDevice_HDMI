#pragma once

#include "../../hal/power_hal.hpp"

namespace streaming::drivers::mock {

class MockPowerDriver : public hal::IPowerHal {
public:
    device::Result initialize() override;
    device::Result shutdown() override;
    device::Result enterStandby() override;
    device::Result wake() override;
    hal::PowerState getState() const override;
    device::Result enableWakeOnRemote(bool enable) override;
    void setWakeCallback(hal::WakeCallback callback) override;
    uint8_t getBatteryLevel() const override;
    bool isOnBattery() const override;
    device::Result setBacklight(uint8_t percent) override;

private:
    hal::PowerState state_{hal::PowerState::ACTIVE};
    hal::WakeCallback wake_cb_;
    uint8_t backlight_{100};
};

} // namespace streaming::drivers::mock
