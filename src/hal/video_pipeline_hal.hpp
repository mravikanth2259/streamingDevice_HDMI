/**
 * @file video_pipeline_hal.hpp
 * @brief Video pipeline HAL - color conversion, HDR, compositor feed
 * @copyright 2025 Streaming Device Project
 *
 * Feed decoded frames to HDMI/framebuffer compositor with proper
 * color space conversion, HDR metadata, and upscaling.
 */

#pragma once

#include <streaming_device/types.hpp>
#include <streaming_device/media_types.hpp>
#include <functional>
#include <memory>

namespace streaming::hal {

/** Frame presentation callback */
using FramePresentCallback = std::function<void(device::Result)>;

/**
 * @brief Video Pipeline Interface
 *
 * Orchestrates: decoded frame → color conversion → HDR handling → compositor.
 */
class IVideoPipeline {
public:
    virtual ~IVideoPipeline() = default;

    /** Initialize pipeline with output dimensions */
    virtual device::Result initialize(uint32_t output_width,
                                     uint32_t output_height) = 0;

    /** Shutdown */
    virtual device::Result shutdown() = 0;

    /** Submit decoded frame for color conversion and presentation */
    virtual device::Result submitFrame(const media::DecodedFrame& frame,
                                      FramePresentCallback on_present) = 0;

    /** Set output color space (e.g., for HDR passthrough) */
    virtual device::Result setOutputColorSpace(
        media::ColorPrimaries primaries,
        media::TransferCharacteristics transfer) = 0;

    /** Set HDR metadata for HDMI output */
    virtual device::Result setHdrMetadata(const media::HdrMetadata& metadata) = 0;

    /** Enable/disable upscaling */
    virtual device::Result setUpscalingEnabled(bool enabled) = 0;

    /** Reset pipeline state */
    virtual device::Result reset() = 0;
};

} // namespace streaming::hal
