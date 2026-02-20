/**
 * @file container_service.cpp
 * @brief ContainerService implementation
 */

#include "container_service.hpp"
#include "../hal/container_hal.hpp"
#include "../common/logger.hpp"
#include <map>

namespace streaming::services {

static media::ContainerFormat detectFormat(const std::string& path) {
    if (path.size() < 4) return media::ContainerFormat::UNKNOWN;
    auto dot_pos = path.find_last_of('.');
    if (dot_pos == std::string::npos) return media::ContainerFormat::UNKNOWN;
    std::string ext = path.substr(dot_pos);
    if (ext == ".mp4") return media::ContainerFormat::MP4;
    if (ext == ".mov") return media::ContainerFormat::MOV;
    if (ext == ".mkv" || ext == ".webm") return media::ContainerFormat::MKV;
    return media::ContainerFormat::UNKNOWN;
}

class ContainerServiceImpl : public IContainerService {
public:
    ContainerServiceImpl() {
        parser_ = hal::createContainerParser();
    }

    device::Result initialize() override { return device::Result::OK; }
    void shutdown() override {}

    device::Result open(const std::string& path_or_uri) override {
        format_ = detectFormat(path_or_uri);
        if (!parser_->supports(format_)) {
            LOG_WARN("ContainerService", "Unsupported format");
            return device::Result::ERROR_NOT_SUPPORTED;
        }
        return parser_->openContainer(path_or_uri);
    }

    device::Result readPacket(media::EncodedPacket& packet_out) override {
        return parser_->readPacket(packet_out);
    }

    device::Result seek(int64_t timestamp_us) override {
        return parser_->seek(timestamp_us);
    }

    std::vector<media::TrackMetadata> getTracks() const override {
        return parser_->getTracks();
    }

    std::vector<media::TrackMetadata> getVideoTracks() const override {
        return parser_->getVideoTracks();
    }

    int64_t getDurationUs() const override {
        return parser_->getDurationUs();
    }

    device::Result close() override {
        return parser_->closeContainer();
    }

    media::ContainerFormat getFormat() const override { return format_; }

private:
    std::unique_ptr<hal::IContainerParser> parser_;
    media::ContainerFormat format_{media::ContainerFormat::UNKNOWN};
};

std::unique_ptr<IContainerService> createContainerService() {
    return std::make_unique<ContainerServiceImpl>();
}

} // namespace streaming::services
