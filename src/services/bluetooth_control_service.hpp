/**
 * @file bluetooth_control_service.hpp
 * @brief Bluetooth Control Service - mobile app control channel
 */

#pragma once

#include <streaming_device/types.hpp>
#include <cstdint>
#include <functional>
#include <memory>
#include <vector>

namespace streaming::services {

/** Control command from mobile app */
enum class ControlCommand : uint8_t {
    NAV_UP,
    NAV_DOWN,
    NAV_LEFT,
    NAV_RIGHT,
    SELECT,
    BACK,
    PLAY,
    PAUSE,
    VOLUME_UP,
    VOLUME_DOWN,
    POWER,
    UNKNOWN
};

/** Command callback */
using ControlCommandCallback = std::function<void(ControlCommand)>;

/**
 * @brief Bluetooth Control Service
 *
 * GATT-based protocol for mobile app to control device.
 * Receives commands, maps to navigation/playback.
 */
class IBluetoothControlService {
public:
    virtual ~IBluetoothControlService() = default;

    /** Initialize GATT service, pairing */
    virtual device::Result initialize() = 0;

    /** Shutdown */
    virtual void shutdown() = 0;

    /** Register callback for incoming commands */
    virtual void setCommandCallback(ControlCommandCallback cb) = 0;

    /** Send status to mobile (e.g., current app, playback state) */
    virtual device::Result sendStatus(const std::vector<uint8_t>& data) = 0;

    /** Check if mobile app is connected */
    virtual bool isConnected() const = 0;
};

std::unique_ptr<IBluetoothControlService> createBluetoothControlService();

} // namespace streaming::services
