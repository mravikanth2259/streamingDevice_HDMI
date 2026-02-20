/**
 * @file container_service.hpp
 * @brief ContainerService - select demuxer, provide track metadata, deliver packets
 */

#pragma once

#include <streaming_device/types.hpp>
#include <streaming_device/media_types.hpp>
#include "hal/container_hal.hpp"
#include <memory>
#include <string>
#include <vector>

namespace streaming::services {

/**
 * @brief ContainerService Interface
 *
 * - Select and initialize appropriate demuxer for container format
 * - Provide track metadata
 * - Deliver packetized payloads to decoders
 */
class IContainerService {
public:
    virtual ~IContainerService() = default;

    virtual device::Result initialize() = 0;
    virtual void shutdown() = 0;

    /** Open container, auto-detect format, initialize demuxer */
    virtual device::Result open(const std::string& path_or_uri) = 0;

    /** Read next packet (video/audio/subtitle) */
    virtual device::Result readPacket(media::EncodedPacket& packet_out) = 0;

    /** Seek to timestamp (microseconds) */
    virtual device::Result seek(int64_t timestamp_us) = 0;

    /** Get all tracks */
    virtual std::vector<media::TrackMetadata> getTracks() const = 0;

    /** Get video tracks only */
    virtual std::vector<media::TrackMetadata> getVideoTracks() const = 0;

    /** Get container duration */
    virtual int64_t getDurationUs() const = 0;

    /** Close container */
    virtual device::Result close() = 0;

    /** Get detected container format */
    virtual media::ContainerFormat getFormat() const = 0;
};

std::unique_ptr<IContainerService> createContainerService();

} // namespace streaming::services
