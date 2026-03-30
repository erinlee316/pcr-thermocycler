# Arduino PCR Thermocycler

A low-cost PCR thermocycler built with Arduino, with Python data analysis to validate thermal accuracy across 30 cycles.

## What is a PCR Thermocycler?
PCR (Polymerase Chain Reaction) is a technique used to amplify DNA. It requires precise temperature cycling through three stages:
- **Denaturation** — 95°C (separates DNA strands)
- **Annealing** — 55°C (primers bind to DNA)
- **Extension** — 72°C (DNA is copied)

## Hardware
- Arduino microcontroller
- DS18B20 temperature sensor (Dallas Temperature library)
- TCS34725 color sensor (Adafruit library)
- 2-channel relay module for heating/cooling control

## Data
Temperature readings were logged from the Arduino every 0.5 seconds across 30 complete PCR cycles (~5,000 seconds total). All temperatures are in °C.

| Column | Description |
|---|---|
| `time` | Elapsed time in seconds |
| `cycle` | Cycle number (1–30) |
| `stage` | PCR stage (denature, anneal, extension) |
| `phase` | Ramp or hold |
| `temperature` | Temperature in °C |

## Analysis (`pcr.ipynb`)
The Jupyter notebook analyzes how accurately the device holds target temperatures:

1. **Thermal Profile** — plots the full 30-cycle temperature curve
2. **Deviation Analysis** — measures how far each cycle's mean hold temperature deviates from target (tolerance: ±1.5°C)
3. **Z-Score Analysis** — flags statistically anomalous cycles (threshold: 2 standard deviations)

### Key Findings
- Device holds temperature within ±1.5°C across all 30 cycles and all 3 stages
- All cycles passed the 2 standard deviation threshold — no consistent drift
- Extension stage had the highest deviation (expected due to longest hold time at 60s)
- Mean z-score across all stages < 1 — thermocycler performs within 1 std dev on average

## How to Run
1. Install dependencies:
```bash
pip install numpy pandas matplotlib jupyter
```
2. Open the notebook:
```bash
jupyter notebook pcr.ipynb
```

## Files
```
├── pcr.ipynb                                    # Data analysis notebook
├── arduino_pcr_data.csv                         # Raw temperature data from Arduino
├── Final_PCR_machine/
│   └── Final_PCR_machine.ino                    # Main Arduino sketch
├── Arduino-Temperature-Control-Library-master 2/
│   ├── DallasTemperature.cpp
│   └── DallasTemperature.h                      # DS18B20 temperature sensor library
├── Adafruit_TCS34725-master-6/
│   ├── Adafruit_TCS34725.cpp
│   └── Adafruit_TCS34725.h                      # Color sensor library
├── _2_channel_relay/
│   └── _2_channel_relay.ino                     # Relay module test sketch
└── README.md
```
