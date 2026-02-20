/**
 * @file media_types.hpp
 * @brief Media types for codecs, containers, and video pipeline
 * @copyright 2025 Streaming Device Project
 */

#pragma once

#include "types.hpp"
#include <cstdint>
#include <string>
#include <vector>

namespace streaming::media {

// =============================================================================
// Codec types
// =============================================================================

enum class VideoCodec : uint8_t {
    UNKNOWN,
    H265_HEVC,
    AV1,
    VP9,
    MPEG4_PART2,
    PRORES
};

enum class AudioCodec : uint8_t {
    UNKNOWN,
    AAC,
    AC3,
    EAC3,
    MP3,
    PCM
};

enum class ContainerFormat : uint8_t {
    UNKNOWN,
    MP4,
    MOV,
    MKV,
    WEBM
};

// =============================================================================
// HDR and color space (SMPTE ST 2086, HDR10/10+)
// =============================================================================

/** Color primaries (ITU-T H.273) */
enum class ColorPrimaries : uint8_t {
    UNSPECIFIED = 0,
    BT709 = 1,
    BT2020 = 9,
    SMPTE_240M = 7,
    P3_D65 = 11
};

/** Transfer characteristics */
enum class TransferCharacteristics : uint8_t {
    UNSPECIFIED = 0,
    BT709 = 1,
    SMPTE_2084 = 16,  /* HDR10 PQ */
    ARIB_STD_B67 = 18 /* HLG */
};

/** Matrix coefficients */
enum class MatrixCoefficients : uint8_t {
    UNSPECIFIED = 0,
    BT709 = 1,
    BT2020_NCL = 9
};

/** Mastering display color volume (SMPTE ST 2086) */
struct MasteringDisplayInfo {
    float display_primaries_x[3]{0, 0, 0};
    float display_primaries_y[3]{0, 0, 0};
    float white_point_x{0};
    float white_point_y{0};
    float max_display_mastering_luminance{0};
    float min_display_mastering_luminance{0};
};

/** Content light level (MaxFALL, MaxCLL) */
struct ContentLightLevel {
    uint16_t max_content_light_level{0};
    uint16_t max_frame_average_light_level{0};
};

/** HDR metadata bundle */
struct HdrMetadata {
    ColorPrimaries color_primaries{ColorPrimaries::UNSPECIFIED};
    TransferCharacteristics transfer{TransferCharacteristics::UNSPECIFIED};
    MatrixCoefficients matrix{MatrixCoefficients::UNSPECIFIED};
    MasteringDisplayInfo mastering_display;
    ContentLightLevel content_light;
    bool is_hdr10{false};
    bool is_hlg{false};
};

// =============================================================================
// Track and stream metadata
// =============================================================================

enum class TrackType : uint8_t {
    VIDEO,
    AUDIO,
    SUBTITLE
};

struct VideoTrackInfo {
    VideoCodec codec{VideoCodec::UNKNOWN};
    uint32_t width{0};
    uint32_t height{0};
    uint32_t frame_rate_num{0};
    uint32_t frame_rate_den{1};
    uint32_t bit_depth{8};
    HdrMetadata hdr;
};

struct AudioTrackInfo {
    AudioCodec codec{AudioCodec::UNKNOWN};
    uint32_t sample_rate{0};
    uint32_t channels{0};
    uint32_t bit_depth{0};
};

struct SubtitleTrackInfo {
    std::string language;
    bool is_forced{false};
};

struct TrackMetadata {
    TrackType type{TrackType::VIDEO};
    uint32_t track_id{0};
    std::string language;
    int64_t duration_us{0};
    int64_t bitrate{0};
    VideoTrackInfo video;
    AudioTrackInfo audio;
    SubtitleTrackInfo subtitle;
};

// =============================================================================
// Frame timing and buffering
// =============================================================================

/** Presentation timestamp (PTS) in microseconds */
using Pts = int64_t;

/** Decode timestamp */
using Dts = int64_t;

struct FrameTiming {
    Pts pts{0};
    Dts dts{0};
    int64_t duration_us{0};
};

enum class PixelFormat : uint8_t {
    UNKNOWN,
    YUV420P,
    NV12,
    P010,   /* 10-bit for HDR */
    RGBA8888,
    BGRA8888
};

/** Decoded video frame */
struct DecodedFrame {
    void* data{nullptr};
    size_t size{0};
    uint32_t width{0};
    uint32_t height{0};
    uint32_t stride{0};
    PixelFormat format{PixelFormat::UNKNOWN};
    FrameTiming timing;
    HdrMetadata hdr;
};

/** Encoded packet from demuxer */
struct EncodedPacket {
    std::vector<uint8_t> data;
    FrameTiming timing;
    uint32_t track_id{0};
    bool is_keyframe{false};
};

// =============================================================================
// Decoder capabilities
// =============================================================================

struct CodecCapabilities {
    VideoCodec codec{VideoCodec::UNKNOWN};
    uint32_t max_width{0};
    uint32_t max_height{0};
    bool hardware_accelerated{false};
    bool hdr_support{false};
    bool drm_support{false};
};

// =============================================================================
// Decode errors
// =============================================================================

enum class DecodeError : int32_t {
    NONE = 0,
    CORRUPT_FRAME = -1,
    UNSUPPORTED = -2,
    BUFFER_UNDERRUN = -3,
    FORMAT_MISMATCH = -4,
    DRM_FAILED = -5,
    FLUSH_FAILED = -6,
    RESET_FAILED = -7
};

} // namespace streaming::media
