#include "mock_audio_driver.hpp"

namespace streaming::drivers::mock {

device::Result MockAudioDriver::initialize() { return device::Result::OK; }
device::Result MockAudioDriver::shutdown() { return device::Result::OK; }

device::Result MockAudioDriver::setSink(hal::AudioSink sink) {
    sink_ = sink;
    return device::Result::OK;
}

device::Result MockAudioDriver::play(const hal::AudioBuffer&) { return device::Result::OK; }
device::Result MockAudioDriver::stop() { return device::Result::OK; }

device::Result MockAudioDriver::setVolume(uint8_t percent) {
    volume_ = percent;
    return device::Result::OK;
}

uint8_t MockAudioDriver::getVolume() const { return volume_; }

device::Result MockAudioDriver::setMute(bool mute) {
    muted_ = mute;
    return device::Result::OK;
}

bool MockAudioDriver::isMuted() const { return muted_; }

} // namespace streaming::drivers::mock
