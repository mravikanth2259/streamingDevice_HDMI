#include "mock_codec_decoder.hpp"
#include <cstring>
#include <vector>

namespace streaming::drivers::mock {

device::Result MockCodecDecoder::initialize(media::VideoCodec codec,
                                            const media::VideoTrackInfo& track_info) {
    codec_ = codec;
    track_info_ = track_info;
    last_error_ = media::DecodeError::NONE;
    return device::Result::OK;
}

device::Result MockCodecDecoder::shutdown() { return device::Result::OK; }

hal::DecodeResult MockCodecDecoder::decodeFrame(const media::EncodedPacket& packet) {
    hal::DecodeResult result;
    result.status = device::Result::OK;
    result.decode_error = media::DecodeError::NONE;
    result.frame_ready = !packet.data.empty();

    if (result.frame_ready) {
        result.frame.width = track_info_.width;
        result.frame.height = track_info_.height;
        result.frame.stride = track_info_.width * 4;
        result.frame.format = media::PixelFormat::RGBA8888;
        result.frame.timing = packet.timing;
        result.frame.hdr = track_info_.hdr;
        result.frame.size = result.frame.width * result.frame.height * 4;
    }
    return result;
}

device::Result MockCodecDecoder::flush() { return device::Result::OK; }

device::Result MockCodecDecoder::drain(hal::DecodeCallback /*callback*/) {
    return device::Result::OK;
}

device::Result MockCodecDecoder::reset() { return device::Result::OK; }

media::CodecCapabilities MockCodecDecoder::getCapabilities() const {
    media::CodecCapabilities caps;
    caps.codec = codec_;
    caps.max_width = 3840;
    caps.max_height = 2160;
    caps.hardware_accelerated = hw_accel_;
    caps.hdr_support = (codec_ == media::VideoCodec::H265_HEVC || codec_ == media::VideoCodec::AV1);
    caps.drm_support = false;
    return caps;
}

media::DecodeError MockCodecDecoder::getError() const { return last_error_; }

void MockCodecDecoder::setHardwareAcceleration(bool enabled) { hw_accel_ = enabled; }

bool MockCodecDecoder::supports(media::VideoCodec codec) const {
    switch (codec) {
        case media::VideoCodec::H265_HEVC:
        case media::VideoCodec::AV1:
        case media::VideoCodec::VP9:
        case media::VideoCodec::MPEG4_PART2:
        case media::VideoCodec::PRORES:
            return true;
        default:
            return false;
    }
}

} // namespace streaming::drivers::mock
