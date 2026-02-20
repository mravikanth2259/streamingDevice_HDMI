# Build & Run

## Build Options

| Method | Command | Notes |
|--------|---------|------|
| **Make** | `make all` | No CMake; single command |
| **CMake** | `cmake -B build && cmake --build build` | Standard out-of-tree build |

## CMake Options

```bash
cmake -B build \
  -DUSE_MOCK_HAL=ON \   # Use mock drivers (default)
  -DBUILD_TESTS=ON      # Build test binary (default)
cmake --build build
```

## Targets

| Target | Description |
|--------|-------------|
| `streaming_device` | Main application |
| `streaming_device_tests` | Unit and integration tests |

## Running

```bash
# Main app (runs until Ctrl+C)
./build/streaming_device

# Tests
./build/streaming_device_tests
# or: make test
```

## Configuration

- **Mock HAL**: Enabled by default; no real hardware required
- **App list**: Edit `config/apps.json` for available streaming apps
