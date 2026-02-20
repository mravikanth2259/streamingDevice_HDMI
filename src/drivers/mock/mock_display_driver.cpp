#include "mock_display_driver.hpp"
#include <cstring>

namespace streaming::drivers::mock {

device::Result MockDisplayDriver::initialize() {
    mode_ = {1920, 1080, 60, hal::PixelFormat::RGBA8888, false};
    framebuffer_.resize(1920 * 1080 * 4, 0);
    connected_ = true;
    return device::Result::OK;
}

device::Result MockDisplayDriver::shutdown() {
    framebuffer_.clear();
    return device::Result::OK;
}

hal::DisplayMode MockDisplayDriver::getDisplayMode() const { return mode_; }

device::Result MockDisplayDriver::setDisplayMode(const hal::DisplayMode& mode) {
    mode_ = mode;
    framebuffer_.resize(mode.width * mode.height * 4, 0);
    return device::Result::OK;
}

hal::FramebufferInfo MockDisplayDriver::getFramebuffer() {
    hal::FramebufferInfo info;
    info.base_address = framebuffer_.data();
    info.width = mode_.width;
    info.height = mode_.height;
    info.stride = mode_.width * 4;
    info.format = mode_.format;
    info.size_bytes = static_cast<uint32_t>(framebuffer_.size());
    return info;
}

device::Result MockDisplayDriver::present(hal::PresentCallback on_complete) {
    if (on_complete) on_complete();
    return device::Result::OK;
}

bool MockDisplayDriver::isConnected() const { return connected_; }

std::vector<hal::DisplayMode> MockDisplayDriver::getSupportedModes() const {
    return {
        {1280, 720, 60, hal::PixelFormat::RGBA8888, false},
        {1920, 1080, 60, hal::PixelFormat::RGBA8888, false},
        {3840, 2160, 60, hal::PixelFormat::RGBA8888, false}
    };
}

device::Result MockDisplayDriver::clear() {
    memset(framebuffer_.data(), 0, framebuffer_.size());
    return device::Result::OK;
}

} // namespace streaming::drivers::mock
