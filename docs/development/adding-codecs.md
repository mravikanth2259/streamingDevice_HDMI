# Adding a Codec

1. **Implement HAL**: Create a class implementing `ICodecDecoder` in `src/drivers/` or `src/hal/`.

2. **Register with CodecService**:

```cpp
codec_svc->registerCodec(
    media::VideoCodec::NEW_CODEC,
    []() { return std::make_unique<NewCodecDecoder>(); },
    {media::VideoCodec::NEW_CODEC, "NewCodec", false, 0}
);
```

3. **Add enum**: If adding a new codec type, update `media_types.hpp`:

```cpp
enum class VideoCodec : uint8_t {
    // ...
    NEW_CODEC
};
```

4. **Update factory**: Add to `hal_factory` or codec registration in `CodecService::initialize()`.
