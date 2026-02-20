/**
 * @file codec_service.cpp
 * @brief CodecService implementation
 */

#include "codec_service.hpp"
#include "../hal/codec_hal.hpp"
#include "../drivers/mock/mock_codec_decoder.hpp"
#include "../common/logger.hpp"
#include <algorithm>
#include <map>

namespace streaming::services {

class CodecServiceImpl : public ICodecService {
public:
    device::Result initialize() override {
        registerCodec(media::VideoCodec::H265_HEVC,
            []() { return std::make_unique<drivers::mock::MockCodecDecoder>(); },
            {media::VideoCodec::H265_HEVC, "HEVC", false, 0});
        registerCodec(media::VideoCodec::AV1,
            []() { return std::make_unique<drivers::mock::MockCodecDecoder>(); },
            {media::VideoCodec::AV1, "AV1", false, 0});
        registerCodec(media::VideoCodec::VP9,
            []() { return std::make_unique<drivers::mock::MockCodecDecoder>(); },
            {media::VideoCodec::VP9, "VP9", false, 0});
        registerCodec(media::VideoCodec::MPEG4_PART2,
            []() { return std::make_unique<drivers::mock::MockCodecDecoder>(); },
            {media::VideoCodec::MPEG4_PART2, "MPEG-4", false, 0});
        registerCodec(media::VideoCodec::PRORES,
            []() { return std::make_unique<drivers::mock::MockCodecDecoder>(); },
            {media::VideoCodec::PRORES, "ProRes", false, 0});
        return device::Result::OK;
    }
    void shutdown() override { factories_.clear(); }

    device::Result registerCodec(media::VideoCodec codec,
                                 std::function<std::unique_ptr<hal::ICodecDecoder>()> factory,
                                 const CodecRegistration& info) override {
        if (!factory) return device::Result::ERROR_INVALID_PARAM;
        factories_[codec].push_back({info, factory});
        LOG_INFO("CodecService", "Registered codec: ", info.name);
        return device::Result::OK;
    }

    std::vector<CodecRegistration> getRegisteredCodecs() const override {
        std::vector<CodecRegistration> result;
        for (const auto& p : factories_) {
            for (const auto& reg : p.second) {
                result.push_back(reg.first);
            }
        }
        return result;
    }

    media::CodecCapabilities getCapabilities(media::VideoCodec codec) const override {
        auto it = factories_.find(codec);
        if (it == factories_.end()) return {};
        if (it->second.empty()) return {};
        auto decoder = it->second.front().second();
        auto caps = decoder->getCapabilities();
        return caps;
    }

    std::unique_ptr<hal::ICodecDecoder> createDecoder(
        const media::VideoTrackInfo& track,
        bool prefer_hardware) override {
        auto it = factories_.find(track.codec);
        if (it == factories_.end()) return nullptr;

        auto& list = it->second;
        std::vector<std::pair<CodecRegistration, std::function<std::unique_ptr<hal::ICodecDecoder>()>>> sorted = list;
        std::sort(sorted.begin(), sorted.end(), [&](const auto& a, const auto& b) {
            if (prefer_hardware && (a.first.hardware_preferred != b.first.hardware_preferred))
                return a.first.hardware_preferred;
            return a.first.priority > b.first.priority;
        });

        for (const auto& reg : sorted) {
            auto dec = reg.second();
            if (dec && dec->supports(track.codec)) {
                dec->setHardwareAcceleration(prefer_hardware);
                if (dec->initialize(track.codec, track) == device::Result::OK)
                    return dec;
            }
        }
        return nullptr;
    }

    bool isSupported(media::VideoCodec codec) const override {
        return factories_.count(codec) && !factories_.at(codec).empty();
    }

    void setHardwareAccelerationPreferred(bool preferred) override {
        prefer_hw_ = preferred;
    }

private:
    std::map<media::VideoCodec, std::vector<std::pair<CodecRegistration,
        std::function<std::unique_ptr<hal::ICodecDecoder>()>>>> factories_;
    bool prefer_hw_{true};
};

std::unique_ptr<ICodecService> createCodecService() {
    return std::make_unique<CodecServiceImpl>();
}

} // namespace streaming::services
