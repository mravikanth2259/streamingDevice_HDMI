# Test Cases

See [Test_Suite_Document](../Test_Suite_Document.md) for full tables. Summary:

## Happy Path

- Display init, present, clear
- Input inject, poll, callback
- App Launcher register, launch, stop
- UI set icons, navigate, select
- HDMI-CEC power, volume, standby
- Codec create decoder, decode frame
- Container open, get tracks, read packet
- Stream Pipeline open, play, seek, stop

## Rainy Day

- Invalid parameters
- Missing resources
- Play without open (ERROR_BUSY)
- Path without extension (UNKNOWN format)
