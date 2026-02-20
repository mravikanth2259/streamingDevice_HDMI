# API Contract Draft (IDL-Style): Core Services

**Purpose:** Minimal interface definitions for UI, App Launcher, Streaming, HDMI-CEC, Bluetooth, Config. Aligned to [Detailed_Design_Document.md](../Detailed_Design_Document.md).

---

## IUiService

```
interface IUiService {
  Result render();
  Result setAppIcons(vector<string> ids, vector<string> labels);
  Result navigate(RemoteKey direction);  // UP, DOWN, LEFT, RIGHT
  Result select();                      // Returns OK; triggers app launch callback
  string getFocusedElementId();
  Result setFocusCallback(FocusCallback cb);
}
```

---

## IAppLauncherService

```
interface IAppLauncherService {
  Result registerApp(AppMetadata metadata);
  Result launch(string app_id, AppLaunchCallback cb);
  Result stop(string session_id);
  vector<AppMetadata> getApps();
  bool isAppActive();
  string getActiveSessionId();
}
```

---

## IStreamingService

```
interface IStreamingService {
  Result startSession(string app_id, string content_uri, StreamingSessionCallback cb);
  Result pause();
  Result resume();
  Result seek(int64_t timestamp_us);
  Result stopSession();
  StreamingSessionState getState();
  Result setStatusCallback(StatusCallback cb);
  Result setTelemetryCallback(TelemetryCallback cb);
}
```

---

## IHdmiCecService

```
interface IHdmiCecService {
  Result powerOnTv();
  Result standbyTv();
  Result sendKeyToTv(RemoteKey key);   // VOLUME_UP, VOLUME_DOWN, MUTE, etc.
  bool isTvPresent();
}
```

---

## IBluetoothControlService

```
interface IBluetoothControlService {
  Result initialize();
  Result setCommandCallback(CommandCallback cb);   // Receives byte 0x01-0x0B
  Result sendStatus(StatusData data);
  bool isConnected();
}
```

---

## IConfigService

```
interface IConfigService {
  Result setString(string key, string value);
  Result getString(string key, string& out);
  Result setInt(string key, int32_t value);
  Result getInt(string key, int32_t& out);
  Result setBool(string key, bool value);
  Result getBool(string key, bool& out);
  Result remove(string key);
}
```

---

## Result

```
enum Result {
  OK = 0,
  ERROR_GENERIC = -1,
  ERROR_NOT_SUPPORTED = -2,
  ERROR_INVALID_PARAM = -3,
  ERROR_TIMEOUT = -4,
  ERROR_NOT_FOUND = -5,
  ERROR_BUSY = -6,
  ERROR_NO_MEMORY = -7,
  ERROR_IO = -8,
  ERROR_AUTH_FAILED = -9,
  ERROR_NETWORK = -10
}
```

---

## Data Types (see types.hpp, media_types.hpp)

- **AppMetadata**: id, name, icon_path, package_uri, auth_endpoint, requires_auth
- **RemoteKey**: NONE, POWER, UP, DOWN, LEFT, RIGHT, OK, BACK, VOLUME_UP, VOLUME_DOWN, MUTE, HOME, MENU
- **StreamingSessionState**: IDLE, PLAYING, PAUSED, SEEKING, ERROR

---

*Draft version: 1.0. Formal IDL (e.g., Protobuf, Cap'n Proto) to be derived from this.*
