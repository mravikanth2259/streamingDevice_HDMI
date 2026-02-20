/**
 * @file input_hal.hpp
 * @brief Hardware Abstraction Layer for Input (Remote, CEC, Touch, etc.)
 * @copyright 2025 Streaming Device Project
 */

#pragma once

#include <streaming_device/types.hpp>
#include <functional>
#include <memory>

namespace streaming::hal {

/** Input event types */
enum class InputEventType : uint8_t {
    KEY_DOWN,
    KEY_UP,
    KEY_REPEAT,
    TOUCH_DOWN,
    TOUCH_UP,
    TOUCH_MOVE
};

/** Input event */
struct InputEvent {
    InputEventType type{InputEventType::KEY_DOWN};
    device::RemoteKey key{device::RemoteKey::NONE};
    uint32_t touch_x{0};
    uint32_t touch_y{0};
    uint64_t timestamp_us{0};
};

/** Callback for input events */
using InputCallback = std::function<void(const InputEvent&)>;

/**
 * @brief Input HAL Interface
 * 
 * Abstracts remote control, IR, CEC key events, and optional touch/joystick.
 */
class IInputHal {
public:
    virtual ~IInputHal() = default;

    /** Initialize input subsystem */
    virtual device::Result initialize() = 0;

    /** Shutdown input */
    virtual device::Result shutdown() = 0;

    /** Register callback for input events */
    virtual void setInputCallback(InputCallback callback) = 0;

    /** Poll for input (if using polled mode) */
    virtual device::Result poll(InputEvent& event) = 0;

    /** Enable/disable input capture */
    virtual device::Result setEnabled(bool enabled) = 0;

    /** Get last key state (for repeat detection) */
    virtual device::RemoteKey getLastKey() const = 0;
};

/** Factory for creating platform-specific input HAL */
std::unique_ptr<IInputHal> createInputHal();

} // namespace streaming::hal
