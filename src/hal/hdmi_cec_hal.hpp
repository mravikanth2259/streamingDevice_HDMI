/**
 * @file hdmi_cec_hal.hpp
 * @brief Hardware Abstraction Layer for HDMI-CEC
 * @copyright 2025 Streaming Device Project
 * 
 * Abstracts HDMI-CEC commands for TV control: Power, Volume, Navigation.
 */

#pragma once

#include <streaming_device/types.hpp>
#include <cstdint>
#include <functional>
#include <memory>
#include <vector>

namespace streaming::hal {

/** CEC Opcode (subset of full CEC spec) */
enum class CecOpcode : uint8_t {
    IMAGE_VIEW_ON = 0x04,
    STANDBY = 0x36,
    GIVE_DEVICE_POWER_STATUS = 0x8F,
    REPORT_POWER_STATUS = 0x90,
    USER_CONTROL_PRESSED = 0x44,
    USER_CONTROL_RELEASED = 0x45,
    VOLUME_UP = 0x41,
    VOLUME_DOWN = 0x42,
    MUTE = 0x43,
    SELECT = 0x00,
    UP = 0x01,
    DOWN = 0x02,
    LEFT = 0x03,
    RIGHT = 0x04,
    ROOT_MENU = 0x09,
    BACK = 0x0D
};

/** CEC message */
struct CecMessage {
    device::CecLogicalAddress source{device::CecLogicalAddress::PLAYBACK_DEVICE_1};
    device::CecLogicalAddress destination{device::CecLogicalAddress::TV};
    CecOpcode opcode{CecOpcode::STANDBY};
    std::vector<uint8_t> operands;
};

/** CEC message callback */
using CecMessageCallback = std::function<void(const CecMessage&)>;

/**
 * @brief HDMI-CEC HAL Interface
 * 
 * Translates remote control and device state into CEC commands.
 * Supports Power On/Off, Volume, Navigation (Up/Down/Left/Right/OK/Back).
 */
class IHdmiCecHal {
public:
    virtual ~IHdmiCecHal() = default;

    /** Initialize CEC hardware */
    virtual device::Result initialize() = 0;

    /** Shutdown CEC */
    virtual device::Result shutdown() = 0;

    /** Send CEC message to TV */
    virtual device::Result sendMessage(const CecMessage& msg) = 0;

    /** Send power on to TV */
    virtual device::Result sendPowerOn() = 0;

    /** Send standby to TV */
    virtual device::Result sendStandby() = 0;

    /** Send user control (volume up/down, navigation, etc.) */
    virtual device::Result sendUserControl(CecOpcode key) = 0;

    /** Map RemoteKey to CEC opcode and send */
    virtual device::Result sendRemoteKey(device::RemoteKey key) = 0;

    /** Register callback for incoming CEC messages */
    virtual void setMessageCallback(CecMessageCallback callback) = 0;

    /** Get logical address of this device */
    virtual device::CecLogicalAddress getLogicalAddress() const = 0;

    /** Check if CEC is enabled and TV is present */
    virtual bool isTvPresent() const = 0;
};

/** Factory for creating platform-specific HDMI-CEC HAL */
std::unique_ptr<IHdmiCecHal> createHdmiCecHal();

} // namespace streaming::hal
