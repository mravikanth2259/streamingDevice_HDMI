#include "mock_hdmi_cec_driver.hpp"
#include <map>

namespace streaming::drivers::mock {

static hal::CecUserControlCode remoteKeyToCec(device::RemoteKey key) {
    switch (key) {
        case device::RemoteKey::VOLUME_UP: return hal::CecUserControlCode::VOLUME_UP;
        case device::RemoteKey::VOLUME_DOWN: return hal::CecUserControlCode::VOLUME_DOWN;
        case device::RemoteKey::MUTE: return hal::CecUserControlCode::MUTE;
        case device::RemoteKey::OK: return hal::CecUserControlCode::SELECT;
        case device::RemoteKey::UP: return hal::CecUserControlCode::UP;
        case device::RemoteKey::DOWN: return hal::CecUserControlCode::DOWN;
        case device::RemoteKey::LEFT: return hal::CecUserControlCode::LEFT;
        case device::RemoteKey::RIGHT: return hal::CecUserControlCode::RIGHT;
        case device::RemoteKey::BACK: return hal::CecUserControlCode::BACK;
        default: return hal::CecUserControlCode::SELECT;
    }
}

device::Result MockHdmiCecDriver::initialize() {
    tv_present_ = true;
    return device::Result::OK;
}

device::Result MockHdmiCecDriver::shutdown() { return device::Result::OK; }

device::Result MockHdmiCecDriver::sendMessage(const hal::CecMessage&) { return device::Result::OK; }

device::Result MockHdmiCecDriver::sendPowerOn() {
    last_sent_opcode_ = hal::CecOpcode::IMAGE_VIEW_ON;
    return device::Result::OK;
}

device::Result MockHdmiCecDriver::sendStandby() {
    last_sent_opcode_ = hal::CecOpcode::STANDBY;
    return device::Result::OK;
}

device::Result MockHdmiCecDriver::sendUserControl(hal::CecUserControlCode key) {
    last_sent_user_control_ = key;
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
