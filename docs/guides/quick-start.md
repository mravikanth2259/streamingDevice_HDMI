# Quick Start

Get the streaming device firmware running in under 5 minutes.

## Prerequisites

- **C++17 compiler**: GCC 7+, Clang 5+, or MSVC 2017+
- **CMake 3.14+** (optional; Makefile available)
- **Make** (optional; for Makefile build)

## Clone and Build

```bash
git clone https://github.com/mravikanth2259/streamingDevice_HDMI.git
cd streamingDevice_HDMI

# Option A: Make
make all

# Option B: CMake
cmake -B build && cmake --build build
```

## Run

```bash
# Main application
./build/streaming_device

# Tests
make test
# or: ./build/streaming_device_tests
```

## Connect to TV

1. Power off TV and device
2. Connect HDMI cable: **device HDMI OUT** → **TV HDMI input**
3. Connect device power
4. Power on TV, then device
5. Select HDMI input on TV (e.g., HDMI 1)

See [HDMI Setup](hdmi-setup.md) for detailed instructions.

## Next Steps

- [Build & Run](build-and-run.md) – Build options and configuration
- [Architecture Overview](../architecture/overview.md) – System design
- [Project Structure](../development/project-structure.md) – Codebase layout
