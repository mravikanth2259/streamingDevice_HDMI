#pragma once

#include "../../hal/container_hal.hpp"
#include <queue>
#include <vector>

namespace streaming::drivers::mock {

/** Mock container parser for MP4, MOV, MKV - unit testing */
class MockContainerParser : public hal::IContainerParser {
public:
    device::Result openContainer(const std::string& path_or_uri) override;
    device::Result readPacket(media::EncodedPacket& packet_out) override;
    device::Result seek(int64_t timestamp_us) override;
    device::Result seekToByte(uint64_t offset) override;
    std::vector<media::TrackMetadata> getTracks() const override;
    std::vector<media::TrackMetadata> getVideoTracks() const override;
    std::vector<media::TrackMetadata> getAudioTracks() const override;
    std::vector<media::TrackMetadata> getSubtitleTracks() const override;
    int64_t getDurationUs() const override;
    device::Result closeContainer() override;
    bool supports(media::ContainerFormat format) const override;

    /** Test helper: inject packets */
    void injectPacket(const media::EncodedPacket& packet);

private:
    media::ContainerFormat format_{media::ContainerFormat::UNKNOWN};
    std::vector<media::TrackMetadata> tracks_;
    std::queue<media::EncodedPacket> packet_queue_;
    int64_t duration_us_{0};
    int64_t seek_pts_{0};
    bool open_{false};
};

} // namespace streaming::drivers::mock
