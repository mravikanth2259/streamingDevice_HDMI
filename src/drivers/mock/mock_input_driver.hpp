#pragma once

#include "../../hal/input_hal.hpp"
#include <queue>

namespace streaming::drivers::mock {

class MockInputDriver : public hal::IInputHal {
public:
    device::Result initialize() override;
    device::Result shutdown() override;
    void setInputCallback(hal::InputCallback callback) override;
    device::Result poll(hal::InputEvent& event) override;
    device::Result setEnabled(bool enabled) override;
    device::RemoteKey getLastKey() const override;

    void injectKey(device::RemoteKey key, hal::InputEventType type);

private:
    hal::InputCallback callback_;
    std::queue<hal::InputEvent> queue_;
    device::RemoteKey last_key_{device::RemoteKey::NONE};
    bool enabled_{true};
};

} // namespace streaming::drivers::mock
