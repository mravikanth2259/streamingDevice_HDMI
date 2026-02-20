#pragma once

#include "../../hal/video_pipeline_hal.hpp"

namespace streaming::drivers::mock {

class MockVideoPipeline : public hal::IVideoPipeline {
public:
    device::Result initialize(uint32_t output_width, uint32_t output_height) override;
    device::Result shutdown() override;
    device::Result submitFrame(const media::DecodedFrame& frame,
                               hal::FramePresentCallback on_present) override;
    device::Result setOutputColorSpace(media::ColorPrimaries primaries,
                                      media::TransferCharacteristics transfer) override;
    device::Result setHdrMetadata(const media::HdrMetadata& metadata) override;
    device::Result setUpscalingEnabled(bool enabled) override;
    device::Result reset() override;

private:
    uint32_t out_w_{0}, out_h_{0};
    bool upscaling_{true};
};

} // namespace streaming::drivers::mock
