# Streaming Device UI Prototype

Interactive HTML5 TV-style launcher for the portable HDMI streaming device.

## Features

- **Black background** with high-contrast white focus borders and glow
- **Horizontal icon row** at bottom: Netflix, Prime Video, Hulu, Disney+, YouTube, Settings
- **River-style scroll** – icons flow as you navigate Left/Right
- **Focus**: white border, glow, 8% scale-up, smooth transitions
- **Settings modal** – mock panel with Video, Network, Bluetooth, Accessibility, About
- **Network icons** (top-right): Wi-Fi and Bluetooth with status badges and tooltips
- **Accessibility**: `prefers-reduced-motion`, aria-labels, semantic HTML
- **Keyboard**: Left/Right navigate, Enter select, S or Settings opens panel, Escape closes

## Quick Start

### Option 1: Local HTTP server (recommended)

`layout.json` is loaded via fetch; use a server to avoid CORS:

```bash
# Python 3
python3 -m http.server 8080

# Node (npx)
npx serve .

# Then open http://localhost:8080
```

### Option 2: Open file directly

Open `index.html` in a browser. If `layout.json` fails to load, the app falls back to built-in defaults.

## File Structure

```
ui-prototype/
├── index.html        # Main document
├── styles.css        # Theme, layout, focus, modal
├── app.js            # Logic, key handling, river scroll
├── README.md
├── data/
│   └── layout.json   # Icon order, labels, network status
└── assets/icons/
    ├── netflix.svg
    ├── prime_video.svg
    ├── hulu.svg
    ├── disney_plus.svg
    ├── youtube.svg
    ├── settings.svg
    ├── wifi.svg
    └── bluetooth.svg
```

## Controls

| Key | Action |
|-----|--------|
| **Left** | Move focus left (wrap) |
| **Right** | Move focus right (wrap) |
| **Enter** | Launch app (logs to console) or open Settings |
| **S** | Open Settings panel |
| **Escape** | Close Settings panel |

## layout.json

```json
{
  "initialFocusIndex": 0,
  "icons": [
    { "id": "netflix", "label": "Netflix", "asset": "assets/icons/netflix.svg" },
    { "id": "settings", "label": "Settings", "asset": "...", "action": "open_settings" }
  ],
  "networkIcons": [
    { "id": "wifi", "status": "connected" },
    { "id": "bluetooth", "status": "disconnected" }
  ]
}
```

## Resolution

- Default: 1080p (1920×1080)
- 4K: Media query scales icons to 320px at 3840px width

## Related

- [TV_UI_Design_Spec.md](../docs/TV_UI_Design_Spec.md) – Full design specification
