/**
 * @file stream_pipeline_service.hpp
 * @brief StreamPipeline - orchestrate demux, decode, color convert, HDR, PTS sync
 */

#pragma once

#include <streaming_device/types.hpp>
#include <streaming_device/media_types.hpp>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>

namespace streaming::services {

/** Pipeline state */
enum class PipelineState : uint8_t {
    IDLE,
    OPENING,
    PLAYING,
    PAUSED,
    SEEKING,
    BUFFERING,
    ERROR
};

/** Pipeline status callback */
using PipelineStatusCallback = std::function<void(PipelineState, const std::string&)>;

/** Telemetry callback (decode errors, buffer underruns, metrics) */
using PipelineTelemetryCallback = std::function<void(const std::string& event,
                                                     const std::string& details)>;

/**
 * @brief StreamPipeline Interface
 *
 * Orchestrates: demuxing → decoding → color conversion → HDR handling → PTS sync
 */
class IStreamPipeline {
public:
    virtual ~IStreamPipeline() = default;

    virtual device::Result initialize() = 0;
    virtual void shutdown() = 0;

    /** Open stream and prepare pipeline */
    virtual device::Result open(const std::string& path_or_uri) = 0;

    /** Start playback */
    virtual device::Result play() = 0;

    /** Pause */
    virtual device::Result pause() = 0;

    /** Seek to timestamp (microseconds) */
    virtual device::Result seek(int64_t timestamp_us) = 0;

    /** Stop and close */
    virtual device::Result stop() = 0;

    /** Get current state */
    virtual PipelineState getState() const = 0;

    /** Get current PTS (microseconds) */
    virtual int64_t getCurrentPts() const = 0;

    /** Set status callback */
    virtual void setStatusCallback(PipelineStatusCallback cb) = 0;

    /** Set telemetry callback */
    virtual void setTelemetryCallback(PipelineTelemetryCallback cb) = 0;
};

std::unique_ptr<IStreamPipeline> createStreamPipeline();

} // namespace streaming::services
