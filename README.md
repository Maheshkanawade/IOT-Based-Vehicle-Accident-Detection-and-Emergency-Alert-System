# 🚗 IoT Based Vehicle Accident Detection & Emergency Alert System

<div align="center">

![Status](https://img.shields.io/badge/Status-Completed-brightgreen?style=for-the-badge)
![Platform](https://img.shields.io/badge/Platform-Arduino_Uno-00979D?style=for-the-badge&logo=arduino&logoColor=white)
![Language](https://img.shields.io/badge/Language-Embedded_C-A8B9CC?style=for-the-badge&logo=c&logoColor=black)
![Conference](https://img.shields.io/badge/Published-PRECCON_2025-blue?style=for-the-badge)

> ⚡ An interrupt-driven embedded system that detects real vehicle accidents and automatically sends GPS location via SMS — within seconds.

</div>

---

## 📟 Serial Monitor Preview

```bash
---- Accident Detection System @ 9600 baud ----

[INIT]   >> Booting system...
[OK]     >> MPU6050 IMU initialized
[OK]     >> NEO-6M GPS module ready
[OK]     >> SIM800L GSM module connected
[READY]  >> Monitoring for impact events...

[DATA]   >> Accel X: 0.98g | Y: 0.02g | Z: 0.01g  → Normal
[DATA]   >> Accel X: 0.97g | Y: 0.01g | Z: 0.00g  → Normal
[ALERT]  >> Impact detected! Magnitude: 3.45g
[INFO]   >> Verifying event... not a speed bump
[WARN]   >> Accident confirmed!
[INFO]   >> Fetching GPS coordinates...
[OK]     >> Location: 19.9975° N, 73.7898° E
[OK]     >> Sending SMS alert...
[SENT]   >> Emergency SMS delivered ✅
[INFO]   >> Cancellation window: 10 sec (press button to cancel)
[DONE]   >> Alert cycle complete. Resuming monitoring...
```

---

## 📌 Overview

This system is a **real-time accident detection and emergency alert device** built on Arduino Uno. It continuously monitors vehicle motion using the MPU6050 IMU sensor. When an abnormal impact is detected, it:

1. Validates it as a real crash (not a speed bump or sharp turn)
2. Fetches live GPS coordinates via the NEO-6M module
3. Sends an **SMS with location** to an emergency contact via SIM800L GSM
4. Provides a **cancellation window** to prevent false alarms

> 📄 Research paper presented at **PRECCON 2025**

---

## ✨ Features

- ✅ **Interrupt-driven firmware** — low-latency impact detection
- ✅ **Smart crash validation** — distinguishes accidents from routine jolts
- ✅ **Automatic SMS alert** with live GPS coordinates
- ✅ **False alarm prevention** via user-controlled cancellation window
- ✅ **Fully standalone** — no smartphone required
- ✅ **Low power design** — suitable for always-on vehicle deployment

---

## 🔧 Hardware Components

| Component | Module | Purpose |
|---|---|---|
| Microcontroller | Arduino Uno (ATmega328P) | Main processing unit |
| IMU Sensor | MPU6050 (Accelerometer + Gyroscope) | Crash impact detection |
| GPS Module | NEO-6M | Real-time location tracking |
| GSM Module | SIM800L | SMS alert transmission |
| Power Supply | 12V → 5V/4V regulated | Powers all modules |
| Cancel Button | Tactile push button | False alarm cancellation |

---

## 🔌 Circuit Connections

> 📷 *Circuit diagram image will be added here*

### MPU6050 → Arduino Uno
| MPU6050 Pin | Arduino Pin |
|---|---|
| VCC | 3.3V |
| GND | GND |
| SDA | A4 |
| SCL | A5 |
| INT | D2 (Interrupt) |

### NEO-6M GPS → Arduino Uno
| NEO-6M Pin | Arduino Pin |
|---|---|
| VCC | 5V |
| GND | GND |
| TX | D4 (SoftwareSerial RX) |
| RX | D3 (SoftwareSerial TX) |

### SIM800L GSM → Arduino Uno
| SIM800L Pin | Arduino Pin |
|---|---|
| VCC | 4V (external regulated) |
| GND | GND |
| TX | D7 (SoftwareSerial RX) |
| RX | D6 (SoftwareSerial TX) |

### Cancel Button
| Button Pin | Arduino Pin |
|---|---|
| One leg | D8 |
| Other leg | GND |

> ⚠️ **Note:** SIM800L requires a stable **4V / 2A** power supply. Do **not** power it from Arduino's 5V pin — it will reset or fail.

---

## 📸 Project Photos

> 📷 *Hardware setup photos will be added here*

---

## ⚙️ How It Works

```
┌─────────────────────────────────────────────────────┐
│                   SYSTEM FLOW                       │
├─────────────────────────────────────────────────────┤
│                                                     │
│  MPU6050 ──► Interrupt fires on impact              │
│                      │                              │
│                       ▼                             │
│          Read accelerometer magnitude               │
│                      │                              │
│           ┌──────────▼──────────┐                   │
│           │  Magnitude > 2.5g?  │                   │
│           └──────────┬──────────┘                   │
│                NO    │    YES                       │
│               ◄──────┘     │                        │
│            Resume           ▼                       │
│          monitoring   Validate event                │
│                       (not bump/turn?)              │
│                             │                       │
│                    ┌────────▼────────┐              │
│                    │ Accident confirmed│             │
│                    └────────┬────────┘              │
│                             │                       │
│                    Fetch GPS coordinates            │
│                             │                       │
│                    Send SMS via SIM800L             │
│                             │                       │
│                    Start cancellation timer         │
│                    (button press = cancel)          │
│                             │                       │
│                    Resume monitoring                │
└─────────────────────────────────────────────────────┘
```

---

## 🛠️ Software & Libraries

| Library | Purpose |
|---|---|
| `Wire.h` | I2C communication with MPU6050 |
| `MPU6050.h` | IMU sensor data reading |
| `TinyGPS++.h` | Parsing NMEA data from NEO-6M |
| `SoftwareSerial.h` | UART for GPS and GSM modules |

---

## 🚀 Setup & How to Run

### 1. Prerequisites
- Arduino IDE installed ([Download here](https://www.arduino.cc/en/software))
- Install required libraries via **Sketch → Include Library → Manage Libraries**:
  - `TinyGPS++`
  - `MPU6050` (search and install)

### 2. Hardware Setup
- Wire all components as per the **Circuit Connections** table above
- Insert a working SIM card (with SMS balance) into SIM800L
- Power SIM800L with a separate **4V / 2A** regulated supply

### 3. Configure Emergency Contact
Open the `.ino` file and update this line with the emergency contact number:
```c
String emergencyNumber = "+91XXXXXXXXXX"; // Replace with actual number
```

### 4. Upload & Test
- Connect Arduino Uno to your PC via USB
- Select **Board:** Arduino Uno and correct **COM Port**
- Click **Upload**
- Open Serial Monitor at **9600 baud** to see live logs
- Tap the MPU6050 sharply to simulate an impact and test the alert

---

## 📊 Impact Threshold Tuning

The crash detection threshold can be adjusted in the code:

```c
#define IMPACT_THRESHOLD 2.5  // in g-force units
                              // Increase to reduce sensitivity
                              // Decrease to increase sensitivity
```

| Scenario | Typical g-force |
|---|---|
| Normal driving | < 0.5g |
| Speed bump | 0.8g – 1.5g |
| Sharp turn | 0.5g – 1.2g |
| Real accident | > 2.5g |

---

## ⚠️ Known Issues & Notes

- [ ] Code has minor bugs from last-minute changes at submission — fix in progress
- [ ] SIM800L is sensitive to power supply; use a stable 4V/2A source
- [ ] GPS cold start may take 1–2 minutes to get a fix (normal behavior)
- [ ] Works best in open areas for GPS accuracy

---

## 🔮 Future Improvements

- [ ] Add OLED display for real-time status
- [ ] Switch to STM32 for better processing power
- [ ] Add WhatsApp alert via Twilio API (ESP32 version)
- [ ] Cloud dashboard for fleet monitoring
- [ ] Li-Po battery backup for post-crash power

---

## 👨‍💻 Author

**Mahesh Kanawade**
Embedded Systems & IoT Engineer

[![LinkedIn](https://img.shields.io/badge/LinkedIn-0077B5?style=flat-square&logo=linkedin&logoColor=white)](https://linkedin.com/in/mahesh-kanawade)
[![GitHub](https://img.shields.io/badge/GitHub-181717?style=flat-square&logo=github&logoColor=white)](https://github.com/Maheshkanawade)
[![Email](https://img.shields.io/badge/Email-D14836?style=flat-square&logo=gmail&logoColor=white)](mailto:maheshkanawade2004@gmail.com)

---

<div align="center">

*⚡ Built with Embedded C on real hardware | Field-tested | PRECCON 2025*

</div>
