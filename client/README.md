
Mail Server Client
==================

This project contains the non-GUI client foundation for the SMTP server in the
sibling `server` folder. The reusable `mail_client_core` library handles SMTP
connection setup, EHLO capability parsing, optional STARTTLS, SMTP AUTH
PLAIN/LOGIN, message validation/composition, and message delivery.

The Qt Quick GUI from the reference app is integrated as `mail_client_gui`. It
uses the same core library, so the compose window and settings panel send
through the real SMTP path instead of a mock UI.

Build
-----

```powershell
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT\scripts\buildsystems\vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=x64-windows-static
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

If Qt 6 is installed and discoverable by CMake, the build also creates
`mail-client-gui`. To skip the GUI target, configure with
`-DMAIL_CLIENT_BUILD_GUI=OFF`.

Manual checks
-------------

With the server running on its default port:

```powershell
.\build\Release\mail-client.exe check --host 127.0.0.1 --port 2525 --tls off
.\build\Release\mail-client.exe auth --host 127.0.0.1 --port 2525 --tls off --username alice@example.com --password secret
.\build\Release\mail-client.exe auth --host 127.0.0.1 --port 2525 --tls off --username bob@example.com --password secret
.\build\Release\mail-client.exe send --host 127.0.0.1 --port 2525 --tls off --username alice@example.com --password secret --to bob@example.com --subject Hello --body "Hi from the client"
.\build\Release\mail-client.exe count --host 127.0.0.1 --port 2525 --tls off --username bob@example.com --password secret
.\build\Release\mail-client.exe fetch --host 127.0.0.1 --port 2525 --tls off --username bob@example.com --password secret --first 1 --last 10
```
