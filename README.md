# Door Monitor

An ESP32-based IoT system for real-time door state detection using ultrasonic sensors and statistical classification.

## Overview

This project monitors door activity by analyzing sonar distance measurements from an HC-SR04 ultrasonic sensor. A Flask backend processes raw sensor data, applies moving average smoothing, and classifies door states (open/closed) which are stored in SQLite for historical tracking and analysis.

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
Additionally, a machine learning model was trained on simulated sensor data with random event switching, and accuracy was compared to the actual model.

## Algorithm

The door state detection uses a moving average approach:

1. **Data collection**: HC-SR04 outputs distance measurements
2. **Smoothing**: Moving average filter reduces sensor noise
3. **Classification**: Adaptive thresholding algorithm determines open/closed states 
4. **Persistence**: States and raw data logged to SQLite

This statistical approach avoids over-fitting to individual measurements while capturing genuine state changes.

### 📊 Adaptive Threshold Model

Instead of relying on a fixed distance cutoff, the system dynamically learns door states from observed data.

The backend maintains a rolling window of recent sonar measurements

Computes mean and standard deviation

Learns two confidence ranges:

Closed range: µ ± kσ around the short-distance readings

Open range: µ ± kσ around longer-distance readings

Classifies transitions only when readings cross statistical bounds

This makes the system robust against:

sensor noise

mounting variability

environmental drift (temperature, humidity, angle)
## Simulation

To actively develop the project despite not having access to the hardware, a simulation of sonar data sending was necessary. It works based off of EDA which determined that the average noise had a range of 7.

## Project Status

This is an actively developed project. Current focus areas include:
- Refining door state detection algorithm performance
- Analyzing stored sensor data for classification accuracy
- Testing sensor reliability across different conditions

## File Structure

```
├── esp32_firmware/          # ESP32 code (C++)
├── dashboard/
│   ├── app.py              # Flask server and classification logic
│   └── data_handling.py    # Data handling
│   └── door.py             # Door class and threshold algorithm
│   └── sim.py              # Simulating sensor data
├── data/
│   └── sonar.db           # SQLite database
└── README.md
```

## Future Work

- Web dashboard for visualizing door state history
- Edge detection for door movement events

## License

MIT