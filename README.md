# Airride Control System

A complete hardware and software solution for controlling air suspension systems in vehicles using a 4-way solenoid valve configuration.

## Project Overview

This project provides an integrated control system for car air suspension management, featuring:
- **Touchscreen UI** with intuitive settings and real-time monitoring
- **Dedicated controller** managing solenoid valves and pressure sensors
- **Real-time communication** between UI and controller via serial protocol

## Project Structure

```
AirrideScreen/          - ESP32-based touchscreen UI with TFT display
├── lib/                - UI components and screen implementations
│   ├── MainScreen/     - Primary operation interface
│   ├── SettingsScreen/ - Configuration screens (4 pages)
│   ├── CalibrationScreen/ - Screen calibration
│   └── ScreenManager/  - Screen navigation and management
└── src/
    └── main.cpp        - UI application entry point

UsbSerial/              - Arduino R4 Minima-based controller
├── lib/                - Core control logic
│   ├── StateMachine/   - Vehicle state management
│   ├── SolenoidManager/ - Solenoid valve control
│   ├── PressureSensorManager/ - Sensor input handling
│   └── LogHandler/     - Data logging via serial
└── src/
    └── main.cpp        - Controller application entry point

SharedLibraries/        - Common utilities shared between projects
├── Communication/      - Serial protocol implementation
└── ISerial/           - Serial interface abstraction
```

## Hardware Architecture

**UI Controller (AirrideScreen)**
- ESP32-2432S028R

**System Controller (UsbSerial)**
- Microcontroller: Arduino R4 Minima
- Solenoid Valves: 4x relay-controlled valves
- Pressure Sensors: Multiple input channels for suspension monitoring

See `images/Fritzing/` for detailed wiring diagrams. (Diagrams are for the new implementation with canbus)

## Features

### Main Screen
- Real-time vehicle status display with suspension visualization
- Visual feedback for suspension pressure

### Ride Settings
- Independent front and rear suspension height adjustment
- Configurable max pressure limits
- Auto-level functionality based on pressure (Machine Learning to be added)

## Installation & Setup

This project uses **PlatformIO** for build and dependency management.

### Prerequisites
- PlatformIO IDE or VS Code with PlatformIO extension
- Arduino IDE (optional, for uploads)

### Building

```bash
# AirrideScreen (ESP32)
cd AirrideScreen
pio run -e esp32doit-devkit-v1

# UsbSerial (Arduino R4 Minima)  
cd UsbSerial
pio run -e uno_r4_minima
```

### Uploading

```bash
# Upload to device
pio run -t upload
```

## Communication Protocol

The UI and controller communicate via a custom serial protocol. See `SharedLibraries/Communication/` for protocol implementation details.

## Contributing

This project is actively maintained and open to improvements. Areas for improvements include:
- Canbus
- Machine Learning
- Bluetooth

## Status

Core functionality is complete and stable. Ongoing feature additions and refinements are in progress.

---

**Built with:** PlatformIO, C++, TFT_eSPI, Arduino Framework
