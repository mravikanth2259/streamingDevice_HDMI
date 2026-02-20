# Codec and Container API Reference

## Overview

Hardware-accelerated or software decoders for H.265 (HEVC), AV1, VP9, MPEG-4 Part 2, and ProRes. Container support for .mp4, .mov, .mkv. Modular HAL and SOA architecture.

---

## Codec HAL (ICodecDecoder)

**File**: `src/hal/codec_hal.hpp`

### Interface

| Method | Description |
|--------|-------------|
| `initialize(codec, track_info)` | Initialize decoder for codec and format |
| `shutdown()` | Release resources |
| `decodeFrame(packet)` | Decode one encoded packet → DecodeResult |
| `flush()` | Flush internal buffers |
| `drain(callback)` | Drain remaining frames (EOS) |
| `reset()` | Reset to initial state |
| `getCapabilities()` | CodecCapabilities |
| `getError()` | Last DecodeError |
| `setHardwareAcceleration(enabled)` | HW accel toggle |
| `supports(codec)` | Check codec support |

### DecodeResult

```cpp
struct DecodeResult {
    device::Result status;
    media::DecodeError decode_error;
    bool frame_ready;
    media::DecodedFrame frame;
};
```

### Supported Codecs

- **H.265 (HEVC)**: 4K/HDR, SMPTE ST 2086, HDR10/10+
- **AV1**: Royalty-free, streaming, color space
- **VP9**: 4K, web-derived streams
- **MPEG-4 Part 2**: Legacy, baseline/advanced, error resilience
- **ProRes**: High-quality editing; licensing required

---

## Container HAL (IContainerParser)

**File**: `src/hal/container_hal.hpp`

### Interface

| Method | Description |
|--------|-------------|
| `openContainer(path_or_uri)` | Open .mp4, .mov, .mkv |
| `readPacket(packet_out)` | Read next video/audio/subtitle packet |
| `seek(timestamp_us)` | Seek to PTS |
| `seekToByte(offset)` | Seek by byte (progressive) |
| `getTracks()` | All track metadata |
| `getVideoTracks()` | Video only |
| `getAudioTracks()` | Audio only |
| `getSubtitleTracks()` | Subtitles |
| `getDurationUs()` | Container duration |
| `closeContainer()` | Close and release |

### Track Metadata

```cpp
struct TrackMetadata {
    TrackType type;           // VIDEO, AUDIO, SUBTITLE
    uint32_t track_id;
    std::string language;
    int64_t duration_us;
    VideoTrackInfo video;     // codec, width, height, frame_rate, HDR
    AudioTrackInfo audio;     // codec, sample_rate, channels
    SubtitleTrackInfo subtitle;
};
```

---

## HDR Metadata

**File**: `include/streaming_device/media_types.hpp`

```cpp
struct HdrMetadata {
    ColorPrimaries color_primaries;
    TransferCharacteristics transfer;
    MatrixCoefficients matrix;
    MasteringDisplayInfo mastering_display;  // SMPTE ST 2086
    ContentLightLevel content_light;         // MaxCLL, MaxFALL
    bool is_hdr10;
    bool is_hlg;
};
```

---

## Video Pipeline HAL (IVideoPipeline)

**File**: `src/hal/video_pipeline_hal.hpp`

- `submitFrame(frame, on_present)` – Color convert, HDR, present to compositor
- `setOutputColorSpace(primaries, transfer)`
- `setHdrMetadata(metadata)` – HDMI HDR passthrough
- `setUpscalingEnabled(enabled)`

---

## DRM HAL (IDrmHal)

**File**: `src/hal/drm_hal.hpp`

Hooks for content protection (Widevine, PlayReady, FairPlay):

- `requestKeys(request, response)` – Secure key retrieval
- `releaseSession(session_id)`
- `isContentProtected(init_data)`

---

## CodecService

**File**: `src/services/codec_service.hpp`

- `registerCodec(codec, factory, info)` – Plug-in codecs
- `createDecoder(track, prefer_hardware)` – Select best decoder
- `getCapabilities(codec)`
- `isSupported(codec)`

---

## ContainerService

**File**: `src/services/container_service.hpp`

- `open(path_or_uri)` – Auto-detect format, init demuxer
- `readPacket(packet_out)`
- `seek(timestamp_us)`
- `getTracks()`, `getDurationUs()`, `close()`

---

## StreamPipeline

**File**: `src/services/stream_pipeline_service.hpp`

Orchestrates: demux → decode → color convert → HDR → PTS sync.

- `open(path_or_uri)`, `play()`, `pause()`, `seek()`, `stop()`
- `setStatusCallback()`, `setTelemetryCallback()` – Errors, buffer underruns

---

## Licensing

- **ProRes**: Apple ProRes license required for decode.
- **HEVC/AV1/VP9**: Check platform and patent pool requirements.
- **Hardware decoders**: SoC-specific; consult vendor docs.
