#include "mock_hdmi_cec_driver.hpp"
#include <map>

namespace streaming::drivers::mock {

static hal::CecOpcode remoteKeyToCec(device::RemoteKey key) {
    switch (key) {
        case device::RemoteKey::POWER: return hal::CecOpcode::IMAGE_VIEW_ON;
        case device::RemoteKey::VOLUME_UP: return hal::CecOpcode::VOLUME_UP;
        case device::RemoteKey::VOLUME_DOWN: return hal::CecOpcode::VOLUME_DOWN;
        case device::RemoteKey::MUTE: return hal::CecOpcode::MUTE;
        case device::RemoteKey::OK: return hal::CecOpcode::SELECT;
        case device::RemoteKey::UP: return hal::CecOpcode::UP;
        case device::RemoteKey::DOWN: return hal::CecOpcode::DOWN;
        case device::RemoteKey::LEFT: return hal::CecOpcode::LEFT;
        case device::RemoteKey::RIGHT: return hal::CecOpcode::RIGHT;
        case device::RemoteKey::BACK: return hal::CecOpcode::BACK;
        default: return hal::CecOpcode::SELECT;
    }
}

device::Result MockHdmiCecDriver::initialize() {
    tv_present_ = true;
    return device::Result::OK;
}

device::Result MockHdmiCecDriver::shutdown() { return device::Result::OK; }

device::Result MockHdmiCecDriver::sendMessage(const hal::CecMessage&) { return device::Result::OK; }

device::Result MockHdmiCecDriver::sendPowerOn() {
    last_sent_ = hal::CecOpcode::IMAGE_VIEW_ON;
    return device::Result::OK;
}

device::Result MockHdmiCecDriver::sendStandby() {
    last_sent_ = hal::CecOpcode::STANDBY;
    return device::Result::OK;
}

device::Result MockHdmiCecDriver::sendUserControl(hal::CecOpcode key) {
    last_sent_ = key;
    return device::Result::OK;
}

device::Result MockHdmiCecDriver::sendRemoteKey(device::RemoteKey key) {
    return sendUserControl(remoteKeyToCec(key));
}

void MockHdmiCecDriver::setMessageCallback(hal::CecMessageCallback cb) { msg_cb_ = std::move(cb); }

device::CecLogicalAddress MockHdmiCecDriver::getLogicalAddress() const {
    return device::CecLogicalAddress::PLAYBACK_DEVICE_1;
}

bool MockHdmiCecDriver::isTvPresent() const { return tv_present_; }

} // namespace streaming::drivers::mock
