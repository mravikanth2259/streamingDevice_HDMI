/**
 * @file display_hal.hpp
 * @brief Hardware Abstraction Layer for Display/Video output
 * @copyright 2025 Streaming Device Project
 * 
 * Abstracts framebuffer rendering, HDMI output, and video compositor.
 * Platform-specific implementations sit behind this interface.
 */

#pragma once

#include <streaming_device/types.hpp>
#include <cstdint>
#include <functional>
#include <memory>

namespace streaming::hal {

/** Pixel format for framebuffer */
enum class PixelFormat : uint8_t {
    RGB888,
    RGBA8888,
    ARGB8888,
    BGRA8888,
    YUV420
};

/** Display mode information */
struct DisplayMode {
    uint32_t width;
    uint32_t height;
    uint32_t refresh_rate_hz;
    PixelFormat format;
    bool interlaced{false};
};

/** Framebuffer descriptor */
struct FramebufferInfo {
    void* base_address{nullptr};
    uint32_t width{0};
    uint32_t height{0};
    uint32_t stride{0};
    PixelFormat format{PixelFormat::RGBA8888};
    uint32_t size_bytes{0};
};

/** Callback for vsync/present completion */
using PresentCallback = std::function<void()>;

/**
 * @brief Display HAL Interface
 * 
 * Minimal API for HDMI display output: initialize, present frames,
 * configure resolution. Platform drivers implement this interface.
 */
class IDisplayHal {
public:
    virtual ~IDisplayHal() = default;

    /** Initialize display and HDMI output */
    virtual device::Result initialize() = 0;

    /** Shutdown and release resources */
    virtual device::Result shutdown() = 0;

    /** Get current display mode */
    virtual DisplayMode getDisplayMode() const = 0;

    /** Set display resolution and refresh rate */
    virtual device::Result setDisplayMode(const DisplayMode& mode) = 0;

    /** Get framebuffer for rendering */
    virtual FramebufferInfo getFramebuffer() = 0;

    /** Present current framebuffer to HDMI output (vsync) */
    virtual device::Result present(PresentCallback on_complete = nullptr) = 0;

    /** Check if display is connected (HDMI hot-plug) */
    virtual bool isConnected() const = 0;

    /** Get supported display modes */
    virtual std::vector<DisplayMode> getSupportedModes() const = 0;

    /** Clear framebuffer to black */
    virtual device::Result clear() = 0;
};

/** Factory for creating platform-specific display HAL */
std::unique_ptr<IDisplayHal> createDisplayHal();

} // namespace streaming::hal
