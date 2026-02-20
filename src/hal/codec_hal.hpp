/**
 * @file codec_hal.hpp
 * @brief Decoding HAL - abstract decoder interface for video codecs
 * @copyright 2025 Streaming Device Project
 *
 * Common decode API: initialize, decode frame, flush, drain, reset,
 * get capabilities, error reporting.
 */

#pragma once

#include <streaming_device/types.hpp>
#include <streaming_device/media_types.hpp>
#include <cstdint>
#include <functional>
#include <memory>
#include <vector>

namespace streaming::hal {

/** Decode result */
struct DecodeResult {
    device::Result status{device::Result::OK};
    media::DecodeError decode_error{media::DecodeError::NONE};
    bool frame_ready{false};
    media::DecodedFrame frame;
};

/** Decode callback for async/streaming decode */
using DecodeCallback = std::function<void(const DecodeResult&)>;

/**
 * @brief Codec Decoder Interface
 *
 * Abstracts H.265, AV1, VP9, MPEG-4 Part 2, ProRes decoders.
 * Supports hardware acceleration via platform-specific implementations.
 */
class ICodecDecoder {
public:
    virtual ~ICodecDecoder() = default;

    /** Initialize decoder for given codec and format */
    virtual device::Result initialize(media::VideoCodec codec,
                                      const media::VideoTrackInfo& track_info) = 0;

    /** Shutdown and release resources */
    virtual device::Result shutdown() = 0;

    /** Decode one encoded packet into decoded frame */
    virtual DecodeResult decodeFrame(const media::EncodedPacket& packet) = 0;

    /** Flush internal buffers, discard pending data */
    virtual device::Result flush() = 0;

    /** Drain remaining frames from decoder (end-of-stream) */
    virtual device::Result drain(DecodeCallback callback) = 0;

    /** Reset decoder to initial state */
    virtual device::Result reset() = 0;

    /** Get decoder capabilities */
    virtual media::CodecCapabilities getCapabilities() const = 0;

    /** Get last decode error */
    virtual media::DecodeError getError() const = 0;

    /** Set hardware acceleration enabled/disabled */
    virtual void setHardwareAcceleration(bool enabled) = 0;

    /** Check if decoder supports given codec */
    virtual bool supports(media::VideoCodec codec) const = 0;
};

/** Factory signature for platform-specific decoders */
using CodecDecoderFactory = std::unique_ptr<ICodecDecoder> (*)();

} // namespace streaming::hal
