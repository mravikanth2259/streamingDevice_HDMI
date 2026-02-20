#pragma once

#include "../../hal/audio_hal.hpp"

namespace streaming::drivers::mock {

class MockAudioDriver : public hal::IAudioHal {
public:
    device::Result initialize() override;
    device::Result shutdown() override;
    device::Result setSink(hal::AudioSink sink) override;
    device::Result play(const hal::AudioBuffer& buffer) override;
    device::Result stop() override;
    device::Result setVolume(uint8_t percent) override;
    uint8_t getVolume() const override;
    device::Result setMute(bool mute) override;
    bool isMuted() const override;

private:
    hal::AudioSink sink_{hal::AudioSink::HDMI};
    uint8_t volume_{80};
    bool muted_{false};
};

} // namespace streaming::drivers::mock
