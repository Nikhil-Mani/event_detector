# Pantry Door Monitor

An ESP32-based IoT system for real-time pantry door state detection using ultrasonic sensors and statistical classification.

## Overview

This project monitors pantry door activity by analyzing sonar distance measurements from an HC-SR04 ultrasonic sensor. A Flask backend processes raw sensor data, applies moving average smoothing, and classifies door states (open/closed) which are stored in SQLite for historical tracking and analysis.

## Hardware

- **Microcontroller**: ESP32
- **Ultrasonic Sensor**: HC-SR04
- **Communication**: WiFi (ESP32 to Flask backend)

## System Architecture

```
ESP32 + HC-SR04  →  WiFi  →  Flask Backend  →  SQLite Database
   (sensor)         (HTTP)    (data processing)   (persistence)
```

The ESP32 runs FreeRTOS tasks to manage sensor polling and HTTP communication. Distance measurements from the HC-SR04 are collected at regular intervals and sent to the Flask backend via HTTP requests. The backend applies moving average filtering to smooth noisy sensor data, then uses statistical thresholds to classify door states. Classification results and raw measurements are logged to SQLite.

## Algorithm

The door state detection uses a moving average approach:

1. **Data collection**: HC-SR04 outputs distance measurements
2. **Smoothing**: Moving average filter reduces sensor noise
3. **Classification**: Distance thresholds determine open/closed states (calibrated during setup)
4. **Persistence**: States and raw data logged to SQLite

This statistical approach avoids over-fitting to individual measurements while capturing genuine state changes.

## Project Status

This is an actively developed project. Current focus areas include:
- Refining door state detection algorithm performance
- Analyzing stored sensor data for classification accuracy
- Testing sensor reliability across different pantry conditions

## File Structure

```
├── esp32_firmware/          # ESP32 code (C++)
├── flask_backend/
│   ├── app.py              # Flask server and classification logic
│   └── requirements.txt
├── data/
│   └── pantry.db           # SQLite database
└── README.md
```

## Future Work

- Web dashboard for visualizing door state history
- Machine learning-based classification refinement
- Multiple sensor support for expanded monitoring
- Edge detection for door movement events

## License

MIT