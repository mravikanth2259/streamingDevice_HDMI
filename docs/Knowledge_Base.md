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

### 1.2 Key User Flows

1. **Power on** → Device powers TV via CEC, shows home page
2. **Select app** → Navigate with remote, press OK → App launches and streams
3. **Volume** → Remote volume up/down → CEC sends to TV
4. **Back** → Exit app, return to home
5. **Mobile control** → Pair phone, use app to navigate/play/pause

---

## 2. Architecture (For Test/Product Teams)

### 2.1 Three Main Layers

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

### 2.2 Block Diagram (Simplified)

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
│ Display HAL │        │ Streaming │         │ CEC HAL   │
│ Input HAL   │        │ Service   │         │           │
└─────────────┘        └─────┬─────┘         └───────────┘
     │                        │
     │                        ▼
     │                 ┌────────────┐
     │                 │ Wi-Fi HAL  │
     │                 │ Audio HAL  │
     │                 └────────────┘
     ▼
┌────────────┐
│ TV / HDMI  │
└────────────┘
```

### 2.3 Event Flow (Input → Action)

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

## 3. Services (What Each Does)

| Service | Role | Test-Relevant Behavior |
|---------|------|------------------------|
| **UI Service** | Renders home page, handles focus, navigation | setAppIcons, navigate, select, getFocusedElementId |
| **App Launcher** | Registers apps, launches by ID, session management | registerApp, launch, stop, getCurrentSessionId |
| **Streaming Service** | Starts/stops stream sessions, quality, pause/resume | startSession, stopSession, getState |
| **HDMI-CEC Service** | Sends CEC commands to TV | sendKeyToTv, powerOnTv, standbyTv |
| **Bluetooth Control** | Receives commands from mobile app | setCommandCallback, sendStatus |
| **Config Service** | Persistent settings | getString/setString, getInt/setInt |
| **Telemetry** | Logging | logEvent, logError |
| **Update** | OTA updates | checkForUpdates, downloadAndApply |

---

## 4. HAL Interfaces (For Test/Product)

### 4.1 Display HAL
- **Purpose**: Output video to HDMI
- **Key**: initialize, setDisplayMode, getFramebuffer, present

### 4.2 Input HAL
- **Purpose**: Remote control, IR, touch
- **Key**: setInputCallback, poll, injectKey (mock)

### 4.3 Wi-Fi HAL
- **Purpose**: Network for streaming
- **Key**: connect, disconnect, getState, resolveHost

### 4.4 Bluetooth HAL
- **Purpose**: A2DP audio, GATT control channel
- **Key**: registerControlService, writeCharacteristic, setGattWriteCallback

### 4.5 HDMI-CEC HAL
- **Purpose**: Send commands to TV
- **Key**: sendPowerOn, sendStandby, sendRemoteKey

### 4.6 Storage HAL
- **Purpose**: Save settings, config
- **Key**: read, write, exists, remove

---

## 5. Remote Key and CEC Mapping

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

## 6. Bluetooth Mobile App Protocol

- **Discovery**: BLE scan for "Streaming Device"
- **Pairing**: Secure BLE pairing
- **Control**: Write 1 byte to Command characteristic (see Bluetooth_Control_Protocol.md)
- **Status**: Subscribe to Status characteristic for device state

---

## 7. Configuration

- **App list**: `config/apps.json` defines available streaming apps
- **Settings**: Stored via Config Service (backed by Storage HAL)

---

## 8. Build and Run

```bash
cmake -B build
cmake --build build
./build/streaming_device
```

**Tests:**
```bash
cd build && ctest
```

---

## 9. Diagrams Reference

See **Implementation_Document.md** for:
- Full architecture block diagram
- Layered architecture (Mermaid)
- App launch sequence diagram
- HDMI-CEC mapping flowchart
