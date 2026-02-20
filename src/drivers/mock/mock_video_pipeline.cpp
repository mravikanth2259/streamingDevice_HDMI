#include "mock_video_pipeline.hpp"

namespace streaming::drivers::mock {

device::Result MockVideoPipeline::initialize(uint32_t output_width, uint32_t output_height) {
    out_w_ = output_width;
    out_h_ = output_height;
    return device::Result::OK;
}

device::Result MockVideoPipeline::shutdown() { return device::Result::OK; }

device::Result MockVideoPipeline::submitFrame(const media::DecodedFrame& /*frame*/,
                                            hal::FramePresentCallback on_present) {
    if (on_present) on_present(device::Result::OK);
    return device::Result::OK;
}

device::Result MockVideoPipeline::setOutputColorSpace(media::ColorPrimaries /*primaries*/,
                                                    media::TransferCharacteristics /*transfer*/) {
    return device::Result::OK;
}

device::Result MockVideoPipeline::setHdrMetadata(const media::HdrMetadata& /*metadata*/) {
    return device::Result::OK;
}

device::Result MockVideoPipeline::setUpscalingEnabled(bool enabled) {
    upscaling_ = enabled;
    return device::Result::OK;
}

device::Result MockVideoPipeline::reset() { return device::Result::OK; }

} // namespace streaming::drivers::mock
