/**
 * @file streaming_service.cpp
 * @brief Streaming Service - integrates CodecService, ContainerService, StreamPipeline
 */

#include "streaming_service.hpp"
#include "stream_pipeline_service.hpp"
#include <streaming_device/types.hpp>
#include "../common/logger.hpp"

namespace streaming::services {

static StreamState pipelineToStreamState(PipelineState ps) {
    switch (ps) {
        case PipelineState::IDLE:
        case PipelineState::ERROR: return StreamState::IDLE;
        case PipelineState::OPENING:
        case PipelineState::BUFFERING: return StreamState::BUFFERING;
        case PipelineState::PLAYING: return StreamState::PLAYING;
        case PipelineState::PAUSED: return StreamState::PAUSED;
        case PipelineState::SEEKING: return StreamState::BUFFERING;
        default: return StreamState::IDLE;
    }
}

class StreamingServiceImpl : public IStreamingService {
public:
    device::Result initialize() override {
        pipeline_ = createStreamPipeline();
        pipeline_->initialize();
        pipeline_->setStatusCallback([this](PipelineState ps, const std::string& msg) {
            state_ = pipelineToStreamState(ps);
            if (status_cb_) status_cb_(state_, msg);
        });
        return device::Result::OK;
    }

    void shutdown() override {
        pipeline_->shutdown();
    }

    device::Result startSession(const std::string& app_id,
                                const std::string& content_id,
                                const std::string& session_id) override {
        LOG_INFO("Streaming", "Start session: ", app_id, content_id, session_id);

        std::string uri = content_id.empty()
            ? "https://example.com/stream_" + app_id + ".mp4"  /* stub URL */
            : content_id;

        if (pipeline_->open(uri) != device::Result::OK) {
            state_ = StreamState::ERROR;
            return device::Result::ERROR_NETWORK;
        }
        return pipeline_->play();
    }

    device::Result stopSession() override {
        pipeline_->stop();
        state_ = StreamState::IDLE;
        if (status_cb_) status_cb_(state_, "");
        return device::Result::OK;
    }

    device::Result pause() override {
        return pipeline_->pause();
    }

    device::Result resume() override {
        return pipeline_->play();
    }

    device::Result setQuality(device::StreamQuality) override { return device::Result::OK; }

    StreamState getState() const override { return state_; }

    void setStatusCallback(StreamStatusCallback cb) override { status_cb_ = std::move(cb); }

    uint8_t getBufferProgress() const override {
        return (state_ == StreamState::BUFFERING) ? 50u : 100u;
    }

private:
    std::unique_ptr<IStreamPipeline> pipeline_;
    StreamState state_{StreamState::IDLE};
    StreamStatusCallback status_cb_;
};

std::unique_ptr<IStreamingService> createStreamingService() {
    return std::make_unique<StreamingServiceImpl>();
}

} // namespace streaming::services
