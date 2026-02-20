# Streaming Device Firmware

Portable streaming device firmware in C++ that connects to a TV via HDMI (HDMI-CEC), streams apps over Wi-Fi, and provides a home page launcher UI. Includes HAL, SOA services, mock drivers, and mobile app Bluetooth control protocol.

## Features

- **HDMI**: Video/audio passthrough to TV (1080p/4K)
- **HDMI-CEC**: Remote control (Power, Volume, Navigation) to TV
- **Wi-Fi**: Streaming from Netflix, Prime Video, Hulu, Disney+, etc.
- **Bluetooth**: A2DP audio sink, GATT control channel for mobile app
- **UI**: Home page with app icons, navigation
- **Video Codecs**: H.265 (HEVC), AV1, VP9, MPEG-4 Part 2, ProRes (HAL + mocks)
- **Containers**: .mp4, .mov, .mkv (demux, tracks, seeking)
- **HDR**: SMPTE ST 2086, HDR10/10+, color space metadata
- **Modular**: HAL, SOA APIs, swappable drivers for testing and portability

## Project Structure

```
streamingDevice_HDMI/
├── include/
│   └── streaming_device/
│       └── types.hpp
├── src/
│   ├── hal/              # Hardware Abstraction Layer
│   │   ├── display_hal.hpp
│   │   ├── input_hal.hpp
│   │   ├── wifi_hal.hpp
│   │   ├── bluetooth_hal.hpp
│   │   ├── hdmi_cec_hal.hpp
│   │   ├── storage_hal.hpp
│   │   ├── power_hal.hpp
│   │   ├── audio_hal.hpp
│   │   └── hal_factory.cpp
│   ├── drivers/
│   │   └── mock/         # Mock drivers for unit testing
│   ├── services/         # SOA services
│   │   ├── ui_service
│   │   ├── app_launcher_service
│   │   ├── streaming_service
│   │   ├── hdmi_cec_service
│   │   ├── bluetooth_control_service
│   │   ├── config_service
│   │   ├── telemetry_service
│   │   └── update_service
│   ├── common/           # Event bus, logger, exceptions
│   └── main.cpp
├── config/
│   └── apps.json        # Example app configuration
├── docs/
│   ├── Implementation_Document.md
│   ├── Test_Suite_Document.md
│   ├── Knowledge_Base.md
│   └── Bluetooth_Control_Protocol.md
├── tests/
└── CMakeLists.txt
```

## Build

### Prerequisites
- CMake 3.14+
- C++17 compiler (GCC, Clang, MSVC)

### Build Steps

```bash
# Configure
cmake -B build

# Build
cmake --build build

# Run
./build/streaming_device
```

### Run Tests

```bash
cd build
ctest --output-on-failure
# Or directly:
./streaming_device_tests
```

## Configuration

- **Use mock HAL** (default): Mock drivers for development and CI. Set `USE_MOCK_HAL=ON`.
- **App list**: Edit `config/apps.json` to add/remove streaming apps.

## Documentation

### Interactive Docs (MkDocs)

Build and serve interactive documentation with search and navigation:

```bash
pip install mkdocs mkdocs-material
mkdocs serve   # http://127.0.0.1:8000
mkdocs build  # Output in site/
```

**Deployed**: [GitHub Pages](https://mravikanth2259.github.io/streamingDevice_HDMI/) (when enabled)

### Reference Documents

| Document | Description |
|----------|-------------|
| [Detailed_Design_Document.md](docs/Detailed_Design_Document.md) | Full design: system context, HAL, security, compliance |
| [Implementation_Document.md](docs/Implementation_Document.md) | Architecture, block diagrams, sequence diagrams |
| [Test_Suite_Document.md](docs/Test_Suite_Document.md) | Happy path, rainy day, integration test cases |
| [Knowledge_Base.md](docs/Knowledge_Base.md) | Reference for test and product teams |
| [Product_Requirements_Document.md](docs/Product_Requirements_Document.md) | PRD with user stories |
| [Bluetooth_Control_Protocol.md](docs/Bluetooth_Control_Protocol.md) | Mobile app GATT protocol |
| [Codec_Container_API.md](docs/Codec_Container_API.md) | Codec/container HAL, HDR, DRM, licensing |

### Auto-Generated

- **Architecture diagram**: `python scripts/generate_architecture.py` – extracts modules from `src/`
- **C++ API** (optional): `doxygen Doxyfile` – generates HTML from headers

## API Overview

### HAL Interfaces
- `IDisplayHal`: Framebuffer, present, resolution
- `IInputHal`: Remote/key events
- `IWifiHal`: Connect, scan, DNS
- `IBluetoothHal`: A2DP, GATT control
- `IHdmiCecHal`: CEC commands to TV
- `IStorageHal`: Key-value persistence
- `IPowerHal`: Sleep, wake
- `IAudioHal`: HDMI/A2DP audio

### Services
- `IUiService`: Home page, navigation
- `IAppLauncherService`: Register/launch apps
- `IStreamingService`: Session management (uses StreamPipeline)
- `ICodecService`: Codec registration, decoder selection
- `IContainerService`: Demux for MP4/MOV/MKV
- `IStreamPipeline`: Demux → decode → HDR → PTS sync
- `IHdmiCecService`: CEC mapping
- `IBluetoothControlService`: Mobile control
- `IConfigService`: Settings
- `ITelemetryService`, `IUpdateService`

## License

Proprietary - Streaming Device Project 2025
