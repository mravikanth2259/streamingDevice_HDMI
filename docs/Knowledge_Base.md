# Streaming Device Firmware - Knowledge Base

**Audience**: Test teams, Product teams, Developers  
**Purpose**: Reference for understanding implementation, architecture, and behavior

---

## 1. Product Overview

### 1.1 What Is It?

A **portable streaming device** that:
- Connects to a TV via **HDMI** (video/audio passthrough)
- Uses **HDMI-CEC** to control the TV (power, volume, navigation)
- Streams content from apps (Netflix, Prime, Hulu, Disney+, etc.) over **Wi-Fi**
- Provides a **home page UI** with app icons for launching
- Supports **Bluetooth** for audio (A2DP) and **mobile app control**
- Decodes **H.265, AV1, VP9, MPEG-4, ProRes** in MP4/MOV/MKV containers

### 1.2 Key User Flows

1. **Power on** → Device powers TV via CEC, shows home page
2. **Select app** → Navigate with remote, press OK → App launches and streams
3. **Volume** → Remote volume up/down → CEC sends to TV
4. **Back** → Exit app, return to home
5. **Mobile control** → Pair phone, use app to navigate/play/pause

---

## 2. Physical Setup: Connecting to TV via HDMI

### 2.1 Visual Connection Diagram

```
    TV                                    STREAMING DEVICE
    ══                                    ════════════════

┌─────────────────────────┐              ┌─────────────────────────┐
│                         │              │                         │
│   ┌─────────────────┐   │              │   ┌─────────────────┐  │
│   │ HDMI 1  HDMI 2   │   │    HDMI      │   │   HDMI OUT       │  │
│   │   ●       ●      │◀──┼──────CABLE───┼──▶│   ●              │  │
│   │                 │   │    (Type A)   │   │                  │  │
│   └─────────────────┘   │              │   └─────────────────┘  │
│         TV INPUT        │              │   Power: USB / DC        │
│                         │              │                         │
└─────────────────────────┘              └─────────────────────────┘

    ▲                                          ▲
    │                                          │
 Connect HDMI cable                     Connect power
 to any HDMI input                      cable
```

### 2.2 Step-by-Step Setup Instructions

| # | Step | Instructions |
|---|------|--------------|
| 1 | **Power off** | Turn off both the TV and the streaming device. |
| 2 | **Find TV HDMI port** | Locate an HDMI input on the TV (side or back). Label: HDMI 1, HDMI 2, etc. |
| 3 | **Find device HDMI port** | On the streaming device, find the **HDMI OUT** port. Do not use USB or other ports. |
| 4 | **Connect HDMI cable** | Insert one end into the TV's HDMI input. Insert the other into the device's HDMI OUT. Push firmly until fully seated. |
| 5 | **Connect power** | Plug the streaming device power supply (USB or DC) into a power outlet. |
| 6 | **Power on TV** | Turn on the TV first. |
| 7 | **Power on device** | Turn on the streaming device. The TV may auto-detect it. |
| 8 | **Select HDMI input** | Use the TV remote to select the HDMI input you used (e.g., "HDMI 1"). |
| 9 | **Enable CEC (recommended)** | In TV settings, enable HDMI-CEC (Anynet+, Bravia Sync, Simplink, etc.) so the remote can control the TV. |
| 10 | **Connect Wi-Fi** | Use device settings to connect to your home Wi-Fi for streaming. |

### 2.3 HDMI Cable Reference

| Spec | Description |
|------|-------------|
| **Connector** | HDMI Type A (standard 19-pin) |
| **Resolution** | HDMI 1.4: up to 1080p; HDMI 2.0+: 4K, HDR |
| **Length** | Standard cables: up to ~5 m; longer runs: use active or fiber HDMI |
| **Direction** | Use the port labeled HDMI **OUT** on the device; **IN** on the TV |

### 2.4 Troubleshooting Connection

| Issue | Possible Cause | Solution |
|-------|----------------|---------|
| No picture | Wrong HDMI input | Select correct HDMI input on TV |
| No picture | Cable not seated | Unplug and firmly reconnect both ends |
| No picture | HDCP mismatch | Try another HDMI port or cable |
| No CEC control | CEC disabled | Enable CEC in TV settings |
| Black screen | Resolution mismatch | Check device output resolution (1080p/4K) |

---

## 3. Architecture (For Test/Product Teams)

### 3.1 Three Main Layers

```
┌──────────────────────────────────────┐
│  SERVICES (UI, App Launcher, etc.)    │  ← Business logic
├──────────────────────────────────────┤
│  HAL (Hardware Abstraction)           │  ← Interfaces to hardware
├──────────────────────────────────────┤
│  DRIVERS (Mock or Real hardware)      │  ← Actual hardware access
└──────────────────────────────────────┘
```

**Why this split?**
- Services can be tested without real hardware (using mocks)
- Replacing hardware (e.g., different SoC) only requires new drivers behind the same HAL

### 3.2 System Block Diagram

```
                    ┌─────────────────┐
                    │   Main Loop     │
                    └────────┬────────┘
                             │
     ┌───────────────────────┼───────────────────────┐
     │                       │                       │
     ▼                       ▼                       ▼
┌─────────┐           ┌─────────────┐          ┌─────────┐
│   UI    │           │ App Launcher│          │ HDMI-CEC│
│ Service │           │   Service   │          │ Service │
└────┬────┘           └──────┬──────┘          └────┬────┘
     │                       │                      │
     │    ┌──────────────────┼──────────────────────┤
     │    │                  │                      │
     ▼    ▼                  ▼                      ▼
┌─────────────┐        ┌────────────┐         ┌───────────┐
│ Display HAL │        │ Streaming  │         │ CEC HAL   │
│ Input HAL   │        │ Service    │         │           │
└─────────────┘        └─────┬──────┘         └───────────┘
     │                        │
     │                        ▼
     │                 ┌────────────┐
     │                 │ Codec Svc  │
     │                 │ Container  │
     │                 │ Pipeline   │
     │                 └─────┬──────┘
     │                       │
     │                 ┌─────┴─────┐
     │                 │ Wi-Fi HAL │
     │                 │ Audio HAL │
     │                 └───────────┘
     ▼
┌────────────┐
│ TV / HDMI  │
└────────────┘
```

### 3.3 Media Pipeline Flow

```
  Network/File
       │
       ▼
┌──────────────┐     EncodedPacket      ┌──────────────┐
│  Container   │ ───────────────────▶  │    Codec     │
│  Service     │  (H.265, AV1, etc.)   │   Service    │
│  (Demux)     │                       │   (Decode)   │
└──────────────┘                       └──────┬───────┘
       │                                      │ DecodedFrame
       │ getTracks()                          ▼
       │                              ┌──────────────┐
       │                              │   Video      │
       │                              │   Pipeline   │
       │                              │ (Color, HDR) │
       │                              └──────┬───────┘
       │                                     │
       │                                     ▼
       │                              ┌──────────────┐
       │                              │  Display HAL │
       │                              │  → HDMI OUT  │
       │                              └──────────────┘
```

### 3.4 Interface Dependency Diagram

```
┌─────────────────────────────────────────────────────────────────┐
│                     WHO CALLS WHOM                                │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│  main.cpp                                                        │
│     │                                                            │
│     ├──▶ IUiService          ──▶ IDisplayHal, IInputHal          │
│     ├──▶ IAppLauncherService ──▶ EventBus                         │
│     ├──▶ IStreamingService   ──▶ IStreamPipeline                  │
│     ├──▶ IHdmiCecService     ──▶ IHdmiCecHal                     │
│     └──▶ IConfigService      ──▶ IStorageHal                     │
│                                                                  │
│  IStreamPipeline                                                 │
│     ├──▶ ICodecService       ──▶ ICodecDecoder                    │
│     ├──▶ IContainerService   ──▶ IContainerParser                 │
│     └──▶ IVideoPipeline     ──▶ (optional) compositor            │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

### 3.5 Event Flow (Input → Action)

```
Remote Key Press
      │
      ▼
Input HAL (receives key)
      │
      ▼
Main loop routes to:
  - UI Service (navigation: up/down/left/right, select, back)
  - HDMI-CEC Service (forward to TV: volume, power, navigation)
  - App Launcher (select → launch app)
  - Streaming Service (launch → start stream)
```

---

## 4. Services (What Each Does)

| Service | Role | Test-Relevant Behavior |
|---------|------|------------------------|
| **UI Service** | Renders home page, handles focus, navigation | setAppIcons, navigate, select, getFocusedElementId |
| **App Launcher** | Registers apps, launches by ID, session management | registerApp, launch, stop, getCurrentSessionId |
| **Streaming Service** | Starts/stops stream sessions, quality, pause/resume | startSession, stopSession, getState |
| **Codec Service** | Registers decoders, selects best for track | registerCodec, createDecoder, getCapabilities |
| **Container Service** | Demuxes MP4/MOV/MKV, provides tracks | open, readPacket, getTracks, seek |
| **Stream Pipeline** | Demux → decode → color → HDR → output | open, play, pause, seek, stop |
| **HDMI-CEC Service** | Sends CEC commands to TV | sendKeyToTv, powerOnTv, standbyTv |
| **Bluetooth Control** | Receives commands from mobile app | setCommandCallback, sendStatus |
| **Config Service** | Persistent settings | getString/setString, getInt/setInt |
| **Telemetry** | Logging | logEvent, logError |
| **Update** | OTA updates | checkForUpdates, downloadAndApply |

---

## 5. HAL Interfaces (For Test/Product)

### 5.1 Display HAL
- **Purpose**: Output video to HDMI
- **Key**: initialize, setDisplayMode, getFramebuffer, present

### 5.2 Input HAL
- **Purpose**: Remote control, IR, touch
- **Key**: setInputCallback, poll, injectKey (mock)

### 5.3 Wi-Fi HAL
- **Purpose**: Network for streaming
- **Key**: connect, disconnect, getState, resolveHost

### 5.4 Bluetooth HAL
- **Purpose**: A2DP audio, GATT control channel
- **Key**: registerControlService, writeCharacteristic, setGattWriteCallback

### 5.5 HDMI-CEC HAL
- **Purpose**: Send commands to TV
- **Key**: sendPowerOn, sendStandby, sendRemoteKey

### 5.6 Storage HAL
- **Purpose**: Save settings, config
- **Key**: read, write, exists, remove

### 5.7 Codec HAL
- **Purpose**: Decode H.265, AV1, VP9, MPEG-4, ProRes
- **Key**: initialize, decodeFrame, flush, reset, getCapabilities

### 5.8 Container HAL
- **Purpose**: Demux MP4, MOV, MKV
- **Key**: openContainer, readPacket, seek, getTracks

---

## 6. Remote Key and CEC Mapping

| Remote Key | Device Action | CEC to TV |
|------------|---------------|-----------|
| UP | Move focus up | Send CEC Up |
| DOWN | Move focus down | Send CEC Down |
| LEFT | Move focus left | Send CEC Left |
| RIGHT | Move focus right | Send CEC Right |
| OK | Select app / Confirm | Send CEC Select |
| BACK | Exit app / Go back | Send CEC Back |
| VOLUME_UP | - | Send CEC Volume Up |
| VOLUME_DOWN | - | Send CEC Volume Down |
| POWER | - | Send CEC Standby (or Power On) |

---

## 7. Bluetooth Mobile App Protocol

- **Discovery**: BLE scan for "Streaming Device"
- **Pairing**: Secure BLE pairing
- **Control**: Write 1 byte to Command characteristic (see Bluetooth_Control_Protocol.md)
- **Status**: Subscribe to Status characteristic for device state

---

## 8. Configuration

- **App list**: `config/apps.json` defines available streaming apps
- **Settings**: Stored via Config Service (backed by Storage HAL)

---

## 9. Build and Run

```bash
# With Make
make all
make test
make run

# With CMake
cmake -B build
cmake --build build
./build/streaming_device
```

**Tests:**
```bash
make test
# or: cd build && ctest
```

---

## 10. Document Cross-Reference

| Document | Contents |
|----------|----------|
| **Implementation_Document.md** | Full architecture, interface diagrams, HDMI setup, media pipeline |
| **Test_Suite_Document.md** | Test cases, test architecture, execution |
| **Codec_Container_API.md** | Codec and container API reference |
| **Bluetooth_Control_Protocol.md** | Mobile app GATT protocol |
