#pragma once

#include "../../hal/display_hal.hpp"
#include <vector>

namespace streaming::drivers::mock {

class MockDisplayDriver : public hal::IDisplayHal {
public:
    device::Result initialize() override;
    device::Result shutdown() override;
    hal::DisplayMode getDisplayMode() const override;
    device::Result setDisplayMode(const hal::DisplayMode& mode) override;
    hal::FramebufferInfo getFramebuffer() override;
    device::Result present(hal::PresentCallback on_complete) override;
    bool isConnected() const override;
    std::vector<hal::DisplayMode> getSupportedModes() const override;
    device::Result clear() override;

private:
    hal::DisplayMode mode_;
    std::vector<uint8_t> framebuffer_;
    bool connected_{false};
};

} // namespace streaming::drivers::mock
