/**
 * @file stream_pipeline_service.cpp
 * @brief StreamPipeline - demux → decode → color convert → HDR → PTS sync
 */

#include "stream_pipeline_service.hpp"
#include "codec_service.hpp"
#include "container_service.hpp"
#include "../hal/video_pipeline_hal.hpp"
#include "../common/logger.hpp"
#include <algorithm>

namespace streaming::services {

class StreamPipelineServiceImpl : public IStreamPipeline {
public:
    StreamPipelineServiceImpl()
        : codec_svc_(createCodecService())
        , container_svc_(createContainerService())
    {}

    device::Result initialize() override {
        codec_svc_->initialize();
        container_svc_->initialize();
        return device::Result::OK;
    }

    void shutdown() override {
        stop();
        container_svc_->shutdown();
        codec_svc_->shutdown();
    }

    device::Result open(const std::string& path_or_uri) override {
        if (state_ != PipelineState::IDLE && state_ != PipelineState::ERROR)
            return device::Result::ERROR_BUSY;

        state_ = PipelineState::OPENING;
        if (status_cb_) status_cb_(state_, "Opening...");

        if (container_svc_->open(path_or_uri) != device::Result::OK) {
            state_ = PipelineState::ERROR;
            if (status_cb_) status_cb_(state_, "Open failed");
            return device::Result::ERROR_IO;
        }

        auto tracks = container_svc_->getVideoTracks();
        if (tracks.empty()) {
            state_ = PipelineState::ERROR;
            if (status_cb_) status_cb_(state_, "No video track");
            return device::Result::ERROR_NOT_FOUND;
        }

        video_track_ = tracks[0];
        decoder_ = codec_svc_->createDecoder(video_track_.video, true);
        if (!decoder_) {
            state_ = PipelineState::ERROR;
            if (status_cb_) status_cb_(state_, "No decoder for codec");
            return device::Result::ERROR_NOT_SUPPORTED;
        }

        state_ = PipelineState::PAUSED;
        if (status_cb_) status_cb_(state_, "Ready");
        return device::Result::OK;
    }

    device::Result play() override {
        /* Only allow play from PAUSED - open() must be called first to initialize
         * container, decoder, and video track. Calling play() from IDLE would skip
         * initialization and cause null pointer dereferences. */
        if (state_ != PipelineState::PAUSED)
            return device::Result::ERROR_BUSY;
        state_ = PipelineState::PLAYING;
        if (status_cb_) status_cb_(state_, "Playing");
        return device::Result::OK;
    }

    device::Result pause() override {
        state_ = PipelineState::PAUSED;
        if (status_cb_) status_cb_(state_, "Paused");
        return device::Result::OK;
    }

    device::Result seek(int64_t timestamp_us) override {
        state_ = PipelineState::SEEKING;
        if (status_cb_) status_cb_(state_, "Seeking...");
        if (container_svc_->seek(timestamp_us) != device::Result::OK) {
            state_ = PipelineState::ERROR;
            return device::Result::ERROR_IO;
        }
        if (decoder_) decoder_->flush();
        state_ = PipelineState::PLAYING;
        current_pts_ = timestamp_us;
        if (status_cb_) status_cb_(state_, "Playing");
        return device::Result::OK;
    }

    device::Result stop() override {
        if (decoder_) decoder_->reset();
        decoder_.reset();
        container_svc_->close();
        state_ = PipelineState::IDLE;
        current_pts_ = 0;
        return device::Result::OK;
    }

    PipelineState getState() const override { return state_; }
    int64_t getCurrentPts() const override { return current_pts_; }

    void setStatusCallback(PipelineStatusCallback cb) override { status_cb_ = std::move(cb); }
    void setTelemetryCallback(PipelineTelemetryCallback cb) override { telemetry_cb_ = std::move(cb); }

private:
    std::unique_ptr<ICodecService> codec_svc_;
    std::unique_ptr<IContainerService> container_svc_;
    std::unique_ptr<hal::ICodecDecoder> decoder_;
    media::TrackMetadata video_track_;
    PipelineState state_{PipelineState::IDLE};
    int64_t current_pts_{0};
    PipelineStatusCallback status_cb_;
    PipelineTelemetryCallback telemetry_cb_;
};

std::unique_ptr<IStreamPipeline> createStreamPipeline() {
    return std::make_unique<StreamPipelineServiceImpl>();
}

} // namespace streaming::services
