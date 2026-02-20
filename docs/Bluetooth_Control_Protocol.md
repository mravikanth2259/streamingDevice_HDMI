# Bluetooth Control Protocol for Mobile App

## Overview

The streaming device exposes a GATT-based control channel for companion mobile apps. The mobile app can discover, pair, and control the device (navigation, playback, volume, power) over Bluetooth Low Energy (BLE).

## GATT Service Definition

### Control Service
- **UUID**: `12345678-1234-5678-1234-56789abcdef0`
- **Purpose**: Device control and status

### Characteristics

| Characteristic | UUID | Properties | Description |
|----------------|------|------------|-------------|
| Command | `12345678-1234-5678-1234-56789abcdef1` | Write | Mobile app writes commands here |
| Status  | `12345678-1234-5678-1234-56789abcdef2` | Read, Notify | Device sends status updates |

## Command Protocol (Mobile → Device)

The mobile app writes a single byte to the Command characteristic:

| Byte | Command | Action |
|------|---------|--------|
| 0x01 | NAV_UP | Move focus up |
| 0x02 | NAV_DOWN | Move focus down |
| 0x03 | NAV_LEFT | Move focus left |
| 0x04 | NAV_RIGHT | Move focus right |
| 0x05 | SELECT | Select/OK |
| 0x06 | BACK | Back |
| 0x07 | PLAY | Play |
| 0x08 | PAUSE | Pause |
| 0x09 | VOLUME_UP | Volume up |
| 0x0A | VOLUME_DOWN | Volume down |
| 0x0B | POWER | Power toggle |

## Status Protocol (Device → Mobile)

Device sends status updates via the Status characteristic (notify):

- **Byte 0**: State flags (bit0: playing, bit1: app active, bit2: wifi connected)
- **Byte 1**: Current volume 0-100
- **Bytes 2+**: Optional JSON or custom payload (e.g., current app ID)

## Pairing Flow

1. Device enables BLE discoverable mode
2. Mobile app scans for device (advertised name: "Streaming Device")
3. Mobile app initiates connection and pairing (BLE secure pairing)
4. Mobile app discovers GATT services and subscribes to Status notifications
5. Mobile app can now write commands to Command characteristic

## Security Requirements

- Use BLE secure pairing (passkey display or numeric comparison)
- Authenticate session; reject unpaired writes
- Optional: encrypt sensitive status data

## Example Mobile App Flow (Pseudocode)

```
1. BLE scan for "Streaming Device"
2. Connect to device
3. Pair (secure)
4. Discover service 12345678-1234-5678-1234-56789abcdef0
5. Subscribe to Status characteristic notifications
6. Write 0x05 to Command to simulate "Select"
7. Receive status notification with current state
```

## Latency

Target: &lt; 100ms from write to device action for responsive UX.
