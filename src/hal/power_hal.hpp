/**
 * @file power_hal.hpp
 * @brief Hardware Abstraction Layer for Power Management
 * @copyright 2025 Streaming Device Project
 * 
 * Sleep/wake, wake-on-IR/remote, battery management (if applicable).
 */

#pragma once

#include <streaming_device/types.hpp>
#include <cstdint>
#include <functional>
#include <memory>

namespace streaming::hal {

/** Power state */
enum class PowerState : uint8_t {
    ACTIVE,
    STANDBY,
    SUSPENDED,
    SHUTDOWN
};

/** Wake source */
enum class WakeSource : uint8_t {
    NONE,
    REMOTE_IR,
    CEC,
    RTC,
    POWER_BUTTON
};

/** Wake callback */
using WakeCallback = std::function<void(WakeSource)>;

/**
 * @brief Power HAL Interface
 * 
 * Manages sleep, wake-on-remote, and optional battery state.
 */
class IPowerHal {
public:
    virtual ~IPowerHal() = default;

    /** Initialize power management */
    virtual device::Result initialize() = 0;

    /** Shutdown power subsystem */
    virtual device::Result shutdown() = 0;

    /** Enter standby/sleep mode */
    virtual device::Result enterStandby() = 0;

    /** Wake from standby */
    virtual device::Result wake() = 0;

    /** Get current power state */
    virtual PowerState getState() const = 0;

    /** Enable wake-on-remote (IR/CEC) */
    virtual device::Result enableWakeOnRemote(bool enable) = 0;

    /** Register wake callback */
    virtual void setWakeCallback(WakeCallback callback) = 0;

    /** Get battery level 0-100 (if applicable, else returns 100) */
    virtual uint8_t getBatteryLevel() const = 0;

    /** Check if on battery (vs AC) */
    virtual bool isOnBattery() const = 0;

    /** Set display backlight brightness 0-100 */
    virtual device::Result setBacklight(uint8_t percent) = 0;
};

/** Factory for creating platform-specific power HAL */
std::unique_ptr<IPowerHal> createPowerHal();

} // namespace streaming::hal
