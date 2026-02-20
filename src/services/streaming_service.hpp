/**
 * @file streaming_service.hpp
 * @brief Media Streaming Service - content ingestion, decoding, output
 */

#pragma once

#include <streaming_device/types.hpp>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>

namespace streaming::services {

/** Stream session state */
enum class StreamState : uint8_t {
    IDLE,
    CONNECTING,
    BUFFERING,
    PLAYING,
    PAUSED,
    ERROR
};

/** Stream status callback */
using StreamStatusCallback = std::function<void(StreamState, const std::string&)>;

/**
 * @brief Streaming Service Interface
 *
 * Modular components: ingestion, DRM (where applicable), decode, render, HDMI.
 */
class IStreamingService {
public:
    virtual ~IStreamingService() = default;

    /** Initialize (TLS, codecs, pipeline) */
    virtual device::Result initialize() = 0;

    /** Shutdown */
    virtual void shutdown() = 0;

    /** Start streaming session (app-specific pipeline) */
    virtual device::Result startSession(const std::string& app_id,
                                        const std::string& content_id,
                                        const std::string& session_id) = 0;

    /** Stop current session */
    virtual device::Result stopSession() = 0;

    /** Pause/Resume */
    virtual device::Result pause() = 0;
    virtual device::Result resume() = 0;

    /** Set quality */
    virtual device::Result setQuality(device::StreamQuality quality) = 0;

    /** Get current state */
    virtual StreamState getState() const = 0;

    /** Register status callback */
    virtual void setStatusCallback(StreamStatusCallback cb) = 0;

    /** Get buffering progress 0-100 */
    virtual uint8_t getBufferProgress() const = 0;
};

std::unique_ptr<IStreamingService> createStreamingService();

} // namespace streaming::services
