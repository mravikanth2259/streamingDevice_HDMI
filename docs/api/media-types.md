# Media Types

Defined in `include/streaming_device/media_types.hpp`.

## Video Codecs

| Codec | Enum | Notes |
|-------|------|-------|
| H.265/HEVC | `VideoCodec::H265_HEVC` | 4K, HDR |
| AV1 | `VideoCodec::AV1` | Royalty-free |
| VP9 | `VideoCodec::VP9` | Web streaming |
| MPEG-4 Part 2 | `VideoCodec::MPEG4_PART2` | Legacy |
| ProRes | `VideoCodec::PRORES` | Editing |

## Container Formats

| Format | Enum |
|--------|------|
| MP4 | `ContainerFormat::MP4` |
| MOV | `ContainerFormat::MOV` |
| MKV | `ContainerFormat::MKV` |

## Key Structs

| Struct | Purpose |
|--------|---------|
| `TrackMetadata` | Track type, duration, codec info |
| `VideoTrackInfo` | Width, height, frame rate, HDR |
| `EncodedPacket` | Data from demuxer |
| `DecodedFrame` | Output from decoder |
| `HdrMetadata` | Color primaries, mastering display |
