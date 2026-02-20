# Adding a New App

1. **Add to config**: Edit `config/apps.json`:

```json
{
  "id": "newapp",
  "name": "New App",
  "icon_path": "/icons/newapp.png",
  "package_uri": "https://newapp.com",
  "auth_endpoint": "",
  "requires_auth": false
}
```

2. **Register at runtime**: In `main.cpp` or during init:

```cpp
app_launcher->registerApp({
    "newapp", "New App", "/icons/newapp.png",
    "https://newapp.com", "", false
});
```

3. **Streaming pipeline**: The streaming service uses the `package_uri` or `content_id` to open content. Ensure the streaming backend supports the app's URL scheme.
