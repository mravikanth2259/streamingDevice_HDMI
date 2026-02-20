# Architecture Overview

The streaming device firmware uses a **three-layer architecture**: Application → Services (SOA) → HAL (Hardware Abstraction Layer) → Drivers.

## Layer Diagram

```mermaid
graph TB
    subgraph Application
        Main[main.cpp]
    end
    
    subgraph Services["Service Layer (SOA)"]
        UI[UI Service]
        App[App Launcher]
        Stream[Streaming]
        CEC[HDMI-CEC]
        Codec[Codec]
        Container[Container]
        Pipeline[Stream Pipeline]
    end
    
    subgraph HAL["Hardware Abstraction Layer"]
        Display[Display HAL]
        Input[Input HAL]
        WiFi[Wi-Fi HAL]
        BT[Bluetooth HAL]
        CECHAL[CEC HAL]
        Storage[Storage HAL]
        CodecHAL[Codec HAL]
        ContainerHAL[Container HAL]
    end
    
    subgraph Drivers["Drivers"]
        Mock[Mock Drivers]
        Platform[Platform Drivers]
    end
    
    Main --> UI
    Main --> App
    Main --> Stream
    Main --> CEC
    Stream --> Pipeline
    Pipeline --> Codec
    Pipeline --> Container
    UI --> Display
    UI --> Input
    CEC --> CECHAL
    Codec --> CodecHAL
    Container --> ContainerHAL
    Display --> Mock
    Input --> Mock
    CECHAL --> Mock
```

## Design Principles

1. **HAL** – All hardware access via abstract interfaces; platform drivers implement them.
2. **SOA** – Services communicate via event bus; clear boundaries.
3. **Testability** – Mock drivers allow unit tests without hardware.
4. **Extensibility** – New apps, codecs, and platforms via config and factories.

## Related

- [System Diagram](system-diagram.md)
- [Data Flows](data-flows.md)
- [Module Map](module-map.md)
