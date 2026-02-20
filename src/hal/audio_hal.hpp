/**
 * @file audio_hal.hpp
 * @brief Hardware Abstraction Layer for Audio output
 * @copyright 2025 Streaming Device Project
 * 
 * Abstracts HDMI audio passthrough, Bluetooth A2DP output.
 */

#pragma once

#include <streaming_device/types.hpp>
#include <cstdint>
#include <functional>
#include <memory>
#include <vector>

namespace streaming::hal {

/** Audio output sink */
enum class AudioSink : uint8_t {
    HDMI,
    BLUETOOTH_A2DP
};

/** Audio format */
enum class AudioFormat : uint8_t {
    PCM_16BIT_STEREO,
    PCM_24BIT_STEREO,
    AAC,
    AC3
};

/** Audio buffer descriptor */
struct AudioBuffer {
    const uint8_t* data{nullptr};
    size_t size{0};
    uint32_t sample_rate_hz{48000};
    AudioFormat format{AudioFormat::PCM_16BIT_STEREO};
};

/**
 * @brief Audio HAL Interface
 * 
 * Routes audio to HDMI or Bluetooth A2DP. Passthrough for streaming.
 */
class IAudioHal {
public:
    virtual ~IAudioHal() = default;

    /** Initialize audio subsystem */
    virtual device::Result initialize() = 0;

    /** Shutdown audio */
    virtual device::Result shutdown() = 0;

    /** Set output sink (HDMI or Bluetooth) */
    virtual device::Result setSink(AudioSink sink) = 0;

    /** Play audio buffer */
    virtual device::Result play(const AudioBuffer& buffer) = 0;

    /** Stop playback */
    virtual device::Result stop() = 0;

    /** Set volume 0-100 */
    virtual device::Result setVolume(uint8_t percent) = 0;

    /** Get current volume */
    virtual uint8_t getVolume() const = 0;

    /** Set mute */
    virtual device::Result setMute(bool mute) = 0;

    /** Check if muted */
    virtual bool isMuted() const = 0;
};

/** Factory for creating platform-specific audio HAL */
std::unique_ptr<IAudioHal> createAudioHal();

} // namespace streaming::hal
