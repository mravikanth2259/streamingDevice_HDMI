# Streaming Device Firmware

Portable streaming device firmware that connects to a TV via **HDMI**, streams apps over **Wi-Fi**, and provides a home page launcher with **HDMI-CEC** TV control and **Bluetooth** mobile app support.

---

## Features

| Feature | Description |
|---------|-------------|
| **HDMI** | Video/audio passthrough to TV (1080p/4K) |
| **HDMI-CEC** | Power, volume, and navigation control of TV |
| **Wi-Fi** | Streaming from Netflix, Prime Video, Hulu, Disney+ |
| **Bluetooth** | A2DP audio, GATT control channel for mobile app |
| **Video Codecs** | H.265, AV1, VP9, MPEG-4, ProRes |
| **Containers** | MP4, MOV, MKV |
| **Modular** | HAL, SOA APIs, swappable drivers |

---

## Quick Links

| Section | Description |
|---------|-------------|
| [Quick Start](guides/quick-start.md) | Build and run in minutes |
| [Architecture](architecture/overview.md) | System design and data flows |
| [API Reference](api/hal-interfaces.md) | HAL interfaces and services |
| [Testing](testing/test-strategy.md) | Test strategy and execution |

---

## Project Layout

```
streamingDevice_HDMI/
├── include/          # Public headers (types, media)
├── src/
│   ├── hal/          # Hardware Abstraction Layer
│   ├── drivers/      # Mock and platform drivers
│   ├── services/     # SOA services
│   ├── common/       # Event bus, logger
│   └── main.cpp
├── config/           # App configuration
├── tests/            # Unit and integration tests
└── docs/             # Documentation source
```

---

## Build

```bash
# With Make
make all
make test

# With CMake
cmake -B build && cmake --build build
./build/streaming_device_tests
```

---

## Documentation Structure

This documentation is built with [MkDocs Material](https://squidfunk.github.io/mkdocs-material/) and includes:

- **Interactive navigation** – Sidebar and tabs
- **Full-text search** – Across all pages
- **Diagrams** – Mermaid architecture and sequence diagrams
- **Auto-synced** – Diagrams and structure scripts regenerate from source

### Local Preview

```bash
pip install mkdocs-material
mkdocs serve
# Open http://127.0.0.1:8000
```

### Build Static Site

```bash
mkdocs build
# Output in site/
```
