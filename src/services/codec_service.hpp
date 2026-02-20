/**
 * @file codec_service.hpp
 * @brief CodecService - register codecs, query capabilities, select decoder
 */

#pragma once

#include <streaming_device/types.hpp>
#include <streaming_device/media_types.hpp>
#include "hal/codec_hal.hpp"
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace streaming::services {

/** Codec registration info */
struct CodecRegistration {
    media::VideoCodec codec;
    std::string name;
    bool hardware_preferred{false};
    uint32_t priority{0};  /* Higher = preferred when multiple support same codec */
};

/**
 * @brief CodecService Interface
 *
 * - Register codecs (plug-in architecture)
 * - Query capabilities
 * - Select codec for a given stream
 * - Initialize/deinitialize decoders
 * - Manage hardware acceleration flags
 */
class ICodecService {
public:
    virtual ~ICodecService() = default;

    virtual device::Result initialize() = 0;
    virtual void shutdown() = 0;

    /** Register a decoder factory for a codec */
    virtual device::Result registerCodec(media::VideoCodec codec,
                                         std::function<std::unique_ptr<hal::ICodecDecoder>()> factory,
                                         const CodecRegistration& info) = 0;

    /** Get all supported codecs */
    virtual std::vector<CodecRegistration> getRegisteredCodecs() const = 0;

    /** Get capabilities for a codec */
    virtual media::CodecCapabilities getCapabilities(media::VideoCodec codec) const = 0;

    /** Select best decoder for track (consider HW accel, priority) */
    virtual std::unique_ptr<hal::ICodecDecoder> createDecoder(
        const media::VideoTrackInfo& track,
        bool prefer_hardware = true) = 0;

    /** Check if codec is supported */
    virtual bool isSupported(media::VideoCodec codec) const = 0;

    /** Set global hardware acceleration preference */
    virtual void setHardwareAccelerationPreferred(bool preferred) = 0;
};

std::unique_ptr<ICodecService> createCodecService();

} // namespace streaming::services
