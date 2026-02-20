#include "mock_power_driver.hpp"

namespace streaming::drivers::mock {

device::Result MockPowerDriver::initialize() { return device::Result::OK; }
device::Result MockPowerDriver::shutdown() { return device::Result::OK; }

device::Result MockPowerDriver::enterStandby() {
    state_ = hal::PowerState::STANDBY;
    return device::Result::OK;
}

device::Result MockPowerDriver::wake() {
    state_ = hal::PowerState::ACTIVE;
    if (wake_cb_) wake_cb_(hal::WakeSource::REMOTE_IR);
    return device::Result::OK;
}

hal::PowerState MockPowerDriver::getState() const { return state_; }

device::Result MockPowerDriver::enableWakeOnRemote(bool) { return device::Result::OK; }

void MockPowerDriver::setWakeCallback(hal::WakeCallback cb) { wake_cb_ = std::move(cb); }

uint8_t MockPowerDriver::getBatteryLevel() const { return 100; }
bool MockPowerDriver::isOnBattery() const { return false; }

device::Result MockPowerDriver::setBacklight(uint8_t percent) {
    backlight_ = percent;
    return device::Result::OK;
}

} // namespace streaming::drivers::mock
