# Project Structure

## Directory Layout

```
streamingDevice_HDMI/
├── include/streaming_device/   # Public headers
│   ├── types.hpp
│   └── media_types.hpp
├── src/
│   ├── hal/                    # HAL interfaces + factory
│   ├── drivers/mock/           # Mock implementations
│   ├── services/               # SOA services
│   ├── common/                 # Event bus, logger
│   └── main.cpp
├── config/                     # Configuration files
├── tests/                      # Test sources
├── docs/                       # Documentation source
├── scripts/                    # Build/doc scripts
├── CMakeLists.txt
├── Makefile
└── mkdocs.yml
```

## Naming Conventions

| Type | Convention | Example |
|------|------------|---------|
| HAL interface | `I*Hal` | `IDisplayHal` |
| Service interface | `I*Service` | `IUiService` |
| Implementation | `*ServiceImpl` | `UiServiceImpl` |
| Mock driver | `Mock*Driver` | `MockDisplayDriver` |
