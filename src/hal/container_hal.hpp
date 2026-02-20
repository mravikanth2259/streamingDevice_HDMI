/**
 * @file container_hal.hpp
 * @brief Container parser HAL - abstract demuxer for MP4, MOV, MKV
 * @copyright 2025 Streaming Device Project
 *
 * Extract video/audio/subtitle tracks, track metadata, streaming timestamps.
 * Support interleaving, seeking, progressive/bitstream decoding.
 */

#pragma once

#include <streaming_device/types.hpp>
#include <streaming_device/media_types.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace streaming::hal {

/**
 * @brief Container Parser Interface
 *
 * Abstracts demuxers for .mp4, .mov, .mkv containers.
 */
class IContainerParser {
public:
    virtual ~IContainerParser() = default;

    /** Open container from file path or stream URI */
    virtual device::Result openContainer(const std::string& path_or_uri) = 0;

    /** Read next packet (video, audio, or subtitle) */
    virtual device::Result readPacket(media::EncodedPacket& packet_out) = 0;

    /** Seek to timestamp (microseconds) */
    virtual device::Result seek(int64_t timestamp_us) = 0;

    /** Seek to byte offset (for progressive/streaming) */
    virtual device::Result seekToByte(uint64_t offset) = 0;

    /** Get all track metadata */
    virtual std::vector<media::TrackMetadata> getTracks() const = 0;

    /** Get video tracks only */
    virtual std::vector<media::TrackMetadata> getVideoTracks() const = 0;

    /** Get audio tracks only */
    virtual std::vector<media::TrackMetadata> getAudioTracks() const = 0;

    /** Get subtitle tracks */
    virtual std::vector<media::TrackMetadata> getSubtitleTracks() const = 0;

    /** Get container duration in microseconds */
    virtual int64_t getDurationUs() const = 0;

    /** Close container and release resources */
    virtual device::Result closeContainer() = 0;

    /** Check if container format is supported */
    virtual bool supports(media::ContainerFormat format) const = 0;
};

std::unique_ptr<IContainerParser> createContainerParser();

} // namespace streaming::hal
