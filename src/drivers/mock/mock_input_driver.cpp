#include "mock_input_driver.hpp"

namespace streaming::drivers::mock {

device::Result MockInputDriver::initialize() { return device::Result::OK; }
device::Result MockInputDriver::shutdown() { return device::Result::OK; }

void MockInputDriver::setInputCallback(hal::InputCallback callback) {
    callback_ = std::move(callback);
}

device::Result MockInputDriver::poll(hal::InputEvent& event) {
    if (queue_.empty()) return device::Result::ERROR_TIMEOUT;
    event = queue_.front();
    queue_.pop();
    return device::Result::OK;
}

device::Result MockInputDriver::setEnabled(bool enabled) {
    enabled_ = enabled;
    return device::Result::OK;
}

device::RemoteKey MockInputDriver::getLastKey() const { return last_key_; }

void MockInputDriver::injectKey(device::RemoteKey key, hal::InputEventType type) {
    last_key_ = key;
    hal::InputEvent e;
    e.type = type;
    e.key = key;
    if (callback_) callback_(e);
    else queue_.push(e);
}

} // namespace streaming::drivers::mock
