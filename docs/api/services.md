# Services

Service-oriented layer. Interfaces in `src/services/*.hpp`.

## Service Dependencies

```mermaid
graph TD
    Main[main.cpp] --> UI[IUiService]
    Main --> App[IAppLauncherService]
    Main --> Stream[IStreamingService]
    Main --> CEC[IHdmiCecService]
    Main --> Config[IConfigService]
    
    Stream --> Pipeline[IStreamPipeline]
    Pipeline --> Codec[ICodecService]
    Pipeline --> Container[IContainerService]
    
    UI --> Display[Display HAL]
    UI --> Input[Input HAL]
    CEC --> CECHAL[HDMI-CEC HAL]
    Config --> Storage[Storage HAL]
```

## Service Interfaces

| Service | Purpose |
|---------|---------|
| **IUiService** | Home page, app icons, navigation |
| **IAppLauncherService** | Register apps, launch by ID |
| **IStreamingService** | Start/stop sessions, pause/resume |
| **ICodecService** | Register decoders, create for track |
| **IContainerService** | Open file, read packets, get tracks |
| **IStreamPipeline** | Demux → decode → output |
| **IHdmiCecService** | Map keys to CEC |
| **IBluetoothControlService** | Mobile app commands |
| **IConfigService** | Persistent settings |
| **ITelemetryService** | Logging |
| **IUpdateService** | OTA updates |

## Creation

```cpp
auto ui = services::createUiService();
auto launcher = services::createAppLauncherService();
// ...
```
