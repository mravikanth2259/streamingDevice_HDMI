#pragma once

#include "../../hal/codec_hal.hpp"
#include <map>

namespace streaming::drivers::mock {

/** Mock decoder supporting all codecs for unit testing */
class MockCodecDecoder : public hal::ICodecDecoder {
public:
    device::Result initialize(media::VideoCodec codec,
                              const media::VideoTrackInfo& track_info) override;
    device::Result shutdown() override;
    hal::DecodeResult decodeFrame(const media::EncodedPacket& packet) override;
    device::Result flush() override;
    device::Result drain(hal::DecodeCallback callback) override;
    device::Result reset() override;
    media::CodecCapabilities getCapabilities() const override;
    media::DecodeError getError() const override;
    void setHardwareAcceleration(bool enabled) override;
    bool supports(media::VideoCodec codec) const override;

private:
    media::VideoCodec codec_{media::VideoCodec::UNKNOWN};
    media::VideoTrackInfo track_info_;
    media::DecodeError last_error_{media::DecodeError::NONE};
    bool hw_accel_{false};
};

} // namespace streaming::drivers::mock
