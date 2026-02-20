/**
 * @file types.hpp
 * @brief Common types and enumerations for Streaming Device firmware
 * @copyright 2025 Streaming Device Project
 */

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

namespace streaming::device {

/** Video resolution presets */
enum class Resolution : uint8_t {
    RES_720P,
    RES_1080P,
    RES_4K
};

/** HDMI connection status */
enum class HdmiStatus : uint8_t {
    DISCONNECTED,
    CONNECTED,
    UNKNOWN
};

/** Display orientation */
enum class DisplayOrientation : uint8_t {
    LANDSCAPE,
    PORTRAIT,
    AUTO
};

/** Wi-Fi connection state */
enum class WifiState : uint8_t {
    DISCONNECTED,
    CONNECTING,
    CONNECTED,
    FAILED
};

/** Bluetooth connection state */
enum class BluetoothState : uint8_t {
    OFF,
    DISCOVERABLE,
    PAIRING,
    CONNECTED,
    DISCONNECTED
};

/** Remote control key codes */
enum class RemoteKey : uint8_t {
    NONE,
    POWER,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    OK,
    BACK,
    VOLUME_UP,
    VOLUME_DOWN,
    MUTE,
    HOME,
    MENU
};

/** CEC logical address */
enum class CecLogicalAddress : uint8_t {
    TV = 0,
    RECORDING_DEVICE_1 = 1,
    RECORDING_DEVICE_2 = 2,
    TUNER_1 = 3,
    PLAYBACK_DEVICE_1 = 4,
    AUDIO_SYSTEM = 5,
    RESERVED = 0xFF
};

/** App launch status */
enum class AppLaunchStatus : uint8_t {
    SUCCESS,
    FAILED,
    TIMEOUT,
    NOT_FOUND,
    AUTH_REQUIRED
};

/** Stream quality preset */
enum class StreamQuality : uint8_t {
    AUTO,
    LOW,
    MEDIUM,
    HIGH,
    ULTRA
};

/** Result/Error codes */
enum class Result : int32_t {
    OK = 0,
    ERROR_GENERIC = -1,
    ERROR_NOT_SUPPORTED = -2,
    ERROR_INVALID_PARAM = -3,
    ERROR_TIMEOUT = -4,
    ERROR_NOT_FOUND = -5,
    ERROR_BUSY = -6,
    ERROR_NO_MEMORY = -7,
    ERROR_IO = -8,
    ERROR_AUTH_FAILED = -9,
    ERROR_NETWORK = -10
};

/** Resolution dimensions */
struct ResolutionInfo {
    uint32_t width;
    uint32_t height;
    uint32_t refresh_rate_hz;
};

/** Streaming app metadata */
struct AppMetadata {
    std::string id;
    std::string name;
    std::string icon_path;
    std::string package_uri;      // App bundle or web URL
    std::string auth_endpoint;    // OAuth/auth URL if applicable
    bool requires_auth{false};
};

} // namespace streaming::device
