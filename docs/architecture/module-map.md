# Module Map

Auto-generated from the repository structure. Run `python scripts/generate_architecture.py` to regenerate.

## Source Modules

| Module | Path | Purpose |
|--------|------|---------|
| HAL | `src/hal/` | Hardware abstraction interfaces |
| Mock Drivers | `src/drivers/mock/` | Unit-test implementations |
| Services | `src/services/` | SOA business logic |
| Common | `src/common/` | Event bus, logger, exceptions |
| Main | `src/main.cpp` | Application entry |

## HAL Modules

| Interface | File | Purpose |
|-----------|------|---------|
| IDisplayHal | display_hal.hpp | Framebuffer, present |
| IInputHal | input_hal.hpp | Remote/key events |
| IWifiHal | wifi_hal.hpp | Wi-Fi client |
| IBluetoothHal | bluetooth_hal.hpp | A2DP, GATT |
| IHdmiCecHal | hdmi_cec_hal.hpp | CEC commands |
| IStorageHal | storage_hal.hpp | Key-value storage |
| IPowerHal | power_hal.hpp | Sleep, wake |
| IAudioHal | audio_hal.hpp | HDMI/A2DP audio |
| ICodecDecoder | codec_hal.hpp | Video decode |
| IContainerParser | container_hal.hpp | Demux MP4/MOV/MKV |
| IVideoPipeline | video_pipeline_hal.hpp | Color, HDR |
| IDrmHal | drm_hal.hpp | Content protection |

## Services

| Service | Purpose |
|---------|---------|
| IUiService | Home page, navigation |
| IAppLauncherService | Register/launch apps |
| IStreamingService | Session management |
| ICodecService | Codec registration |
| IContainerService | Demux, tracks |
| IStreamPipeline | Demux→decode→output |
| IHdmiCecService | CEC mapping |
| IBluetoothControlService | Mobile control |
| IConfigService | Settings |
| ITelemetryService | Logging |
| IUpdateService | OTA |
