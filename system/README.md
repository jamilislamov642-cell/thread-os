# THREAD system services

The system services layer is responsible for the runtime platform that supports the desktop and applications.

## Responsibilities

- user login and permission handling
- workspace layout persistence
- process supervision
- service initialization
- logging and diagnostics
- mount and device management
- update orchestration

## Session model

The workspace layer stores:

- open apps
- active projects
- recently opened files
- prior window arrangement
- last working session metadata

This allows THREAD to offer a restore prompt after reboot.
