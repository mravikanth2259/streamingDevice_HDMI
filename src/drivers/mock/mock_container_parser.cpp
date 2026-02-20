#include "mock_container_parser.hpp"

namespace streaming::drivers::mock {

static media::ContainerFormat detectFormat(const std::string& path) {
    if (path.find(".mp4") != std::string::npos) return media::ContainerFormat::MP4;
    if (path.find(".mov") != std::string::npos) return media::ContainerFormat::MOV;
    if (path.find(".mkv") != std::string::npos || path.find(".webm") != std::string::npos)
        return media::ContainerFormat::MKV;
    return media::ContainerFormat::MP4;  /* default for testing */
}

device::Result MockContainerParser::openContainer(const std::string& path_or_uri) {
    format_ = detectFormat(path_or_uri);
    open_ = true;
    duration_us_ = 120000000;  /* 2 minutes */

    media::TrackMetadata video;
    video.type = media::TrackType::VIDEO;
    video.track_id = 1;
    video.duration_us = duration_us_;
    video.video.codec = media::VideoCodec::H265_HEVC;
    video.video.width = 1920;
    video.video.height = 1080;
    video.video.frame_rate_num = 24;
    video.video.frame_rate_den = 1;
    tracks_.push_back(video);

    media::TrackMetadata audio;
    audio.type = media::TrackType::AUDIO;
    audio.track_id = 2;
    audio.duration_us = duration_us_;
    audio.audio.codec = media::AudioCodec::AAC;
    audio.audio.sample_rate = 48000;
    audio.audio.channels = 2;
    tracks_.push_back(audio);

    return device::Result::OK;
}

device::Result MockContainerParser::readPacket(media::EncodedPacket& packet_out) {
    if (!open_ || packet_queue_.empty()) return device::Result::ERROR_TIMEOUT;
    packet_out = packet_queue_.front();
    packet_queue_.pop();
    return device::Result::OK;
}

device::Result MockContainerParser::seek(int64_t timestamp_us) {
    seek_pts_ = timestamp_us;
    while (!packet_queue_.empty()) packet_queue_.pop();
    return device::Result::OK;
}

device::Result MockContainerParser::seekToByte(uint64_t /*offset*/) {
    return device::Result::OK;
}

std::vector<media::TrackMetadata> MockContainerParser::getTracks() const {
    return tracks_;
}

std::vector<media::TrackMetadata> MockContainerParser::getVideoTracks() const {
    std::vector<media::TrackMetadata> out;
    for (const auto& t : tracks_)
        if (t.type == media::TrackType::VIDEO) out.push_back(t);
    return out;
}

std::vector<media::TrackMetadata> MockContainerParser::getAudioTracks() const {
    std::vector<media::TrackMetadata> out;
    for (const auto& t : tracks_)
        if (t.type == media::TrackType::AUDIO) out.push_back(t);
    return out;
}

std::vector<media::TrackMetadata> MockContainerParser::getSubtitleTracks() const {
    std::vector<media::TrackMetadata> out;
    for (const auto& t : tracks_)
        if (t.type == media::TrackType::SUBTITLE) out.push_back(t);
    return out;
}

int64_t MockContainerParser::getDurationUs() const { return duration_us_; }

device::Result MockContainerParser::closeContainer() {
    open_ = false;
    while (!packet_queue_.empty()) packet_queue_.pop();
    return device::Result::OK;
}

bool MockContainerParser::supports(media::ContainerFormat format) const {
    return format == media::ContainerFormat::MP4 ||
           format == media::ContainerFormat::MOV ||
           format == media::ContainerFormat::MKV;
}

void MockContainerParser::injectPacket(const media::EncodedPacket& packet) {
    packet_queue_.push(packet);
}

} // namespace streaming::drivers::mock
