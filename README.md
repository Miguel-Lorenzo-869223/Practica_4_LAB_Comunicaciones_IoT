# Practica_3_LAB_Comunicaciones_IoT

# Practical Exercise 3: Bluetooth Low Energy (BLE) GATT Architecture & Bluetooth Classic SPP

> **IoT Communications Laboratory**  
> MSc in Electronic Engineering — University of Zaragoza (EINA/UNIZAR)  
> **Author:** Miguel A. Lorenzo  
> **Date:** October 2026  

---

## 📌 Project Overview

This repository contains the complete embedded C++ software implementation for **Practical Exercise 3** of the IoT Communications Laboratory. The main objective is to explore, implement, and analyze wireless short-range communication protocols using both **Bluetooth Low Energy (BLE)** and **Bluetooth Classic (BR/EDR)**. 

The lab is divided into five progressive practical exercises developed across two hardware platforms:
1. **Arduino Nano 33 BLE:** Used for GATT server implementations, peripheral attribute management, built-in RGB LED control, and IMU sensor telemetry (notifications and control commands).
2. **ESP32 Dev Module:** Used for custom UUID advertising and establishing a bidirectional Bluetooth Classic Serial Port Profile (SPP / RFCOMM) transparent chat bridge.

---

## 🛠️ Hardware Setup & Key Peripherals

- **Arduino Nano 33 BLE (nRF52840):**
  - **BLE Radio Controller:** Integrated nRF52840 2.4 GHz SoC running `ArduinoBLE`.
  - **Inertial Sensor:** Onboard ST LSM9DS1 / LSM6DS3 IMU connected via internal SPI/I2C bus.
  - **Digital Actuators:** Onboard active-LOW RGB LED (`LEDR`, `LEDG`, `LEDB`).
- **ESP32 Development Board:**
  - **Dual-Mode Bluetooth Transceiver:** Integrated 2.4 GHz Wi-Fi / Bluetooth 4.2 BR/EDR & BLE controller.
- **Diagnostic Tools & Interfaces:**
  - **UART Interface:** Serial communication at **115200 baud** for real-time debugging.
  - **Mobile Smartphone:** Android/iOS device equipped with BLE scanner and SPP terminal software.

---

## 📱 Required Applications & Software Environment

- **Development IDE:** Arduino IDE (v2.x or later).
- **Core Board Packages:**
  - `Arduino Mbed OS Nano Boards` (for Arduino Nano 33 BLE).
  - `esp32` by Espressif Systems (for ESP32 Dev Module).
- **Required C++ Libraries:**
  - `ArduinoBLE` (GATT and GAP stack for Arduino).
  - `Arduino_LSM9DS1` or `Arduino_LSM6DS3` (Inertial sensor driver).
  - `BluetoothSerial` (ESP32 native RFCOMM/SPP driver).
- **Mobile Applications:**
  - **[nRF Connect for Mobile](https://play.google.com/store/apps/details?id=no.nordicsemi.android.mcp&hl=es_419&gl=US):** For BLE scanning, GATT database discovery, characteristic reads/writes, and notification tracking.
  - **Serial Bluetooth Terminal:** For Bluetooth Classic SPP chat testing.

---

## 🏗️ Exercises & System Architectures

### 1. Exercise 3.1 — Custom Name/ASCII-Encoded UUID BLE Advertising
- **Platform:** ESP32 / Arduino Nano 33 BLE
- **Description:** Configures the GAP layer to perform continuous advertising on physical channels 37, 38, and 39. Generates a custom 128-bit "Vanity UUID" by embedding the ASCII string **"MIGUEL"** (`0x4D 0x49 0x47 0x55 0x45 0x4C`) into the highest byte offset (`4d494755-454c-0000-0000-000000000001`).
- **Key Features:** Length-Type-Value (LTV) frame structuring, Little-Endian UUID byte ordering over the air, and Scan Response payload activation (`setScanResponse(true)`).

### 2. Exercise 3.2 — Peripheral GATT Server for RGB LED Control
- **Platform:** Arduino Nano 33 BLE
- **Description:** Establishes a GATT Server hosting a custom service (`19B10000-E008-4E6F-9A37-7D23D95F3698`) and a 1-byte control characteristic (`BLERead | BLEWrite`).
- **Key Features:** Listens for incoming ATT Write requests (`written()`) from a central device to drive the onboard active-LOW RGB LED (e.g., `0x01` = Red, `0x02` = Green, `0x03` = Blue, `0x00` = Off).

### 3. Exercise 3.3 — Real-Time IMU Telemetry via BLE Notifications
- **Platform:** Arduino Nano 33 BLE
- **Description:** Implements an automated telemetry node exposing 3-axis acceleration data ($X, Y, Z$) over a 12-byte payload characteristic (`BLERead | BLENotify`).
- **Key Features:** Converts internal 32-bit floats into a raw byte array (`byte[12]`). Utilizes asynchronous BLE notifications push updates to connected centrals at 10 Hz without requiring explicit Client Read polling.

### 4. Exercise 3.4 — Remote Sensor Enable/Disable via GATT Write Control
- **Platform:** Arduino Nano 33 BLE
- **Description:** Expands Exercise 3.3 by adding a second, 1-byte write-only characteristic (`enableChar`, `BLEWrite`) to the sensor service.
- **Key Features:** Acts as a remote hardware toggle (`0x01` = Enable, `0x00` = Disable). Disabling the sensor suspends IMU bus sampling and cancels notification updates, conserving power and RF bandwidth.

### 5. Exercise 3.5 — Bluetooth Classic (BR/EDR) SPP Bidirectional Chat
- **Platform:** ESP32 Dev Module
- **Description:** Implements a transparent UART-to-RFCOMM serial bridge using the Serial Port Profile (SPP).
- **Key Features:** Emulates standard Bluetooth serial connection (`BluetoothSerial`). Messages typed into the PC Serial Monitor are forwarded to the smartphone app, and responses from the smartphone app are output back to the PC console in real time.

---

## 📂 Project Structure

```text
.
├── LB_IOT_P3_1_BLE_Advertising/
│   └── LB_IOT_P3_1_BLE_Advertising.ino       # Exercise 1: Custom ASCII-UUID BLE Advertising
├── LB_IOT_P3_2_GATT_LED_Control/
│   └── LB_IOT_P3_2_GATT_LED_Control.ino      # Exercise 2: GATT Server RGB LED Actuator Control
├── LB_IOT_P3_3_IMU_Telemetry_Notify/
│   └── LB_IOT_P3_3_IMU_Telemetry_Notify.ino # Exercise 3: Accelerometer Telemetry via Notifications
├── LB_IOT_P3_4_IMU_Control_Write/
│   └── LB_IOT_P3_4_IMU_Control_Write.ino     # Exercise 4: Remote Sensor Activation via GATT Write
├── LB_IOT_P3_5_BT_Classic_SPP_Chat/
│   └── LB_IOT_P3_5_BT_Classic_SPP_Chat.ino   # Exercise 5: ESP32 Bluetooth Classic SPP Terminal Bridge
└── README.md                                 # Repository documentation
```

---

## 📚 Reference Material & Documentation

- **Lab Manual Support Links:**
  - [ESP32 BLE Overview & Setup in Arduino IDE](https://randomnerdtutorials.com/esp32-bluetooth-low-energy-ble-arduino-ide/)
  - [Arduino Nano 33 BLE Device-to-Device Communication](https://docs.arduino.cc/tutorials/nano-33-ble-sense/ble-device-to-device/)
  - [Understanding iBeacon & Advertising Frame Structure (Wikipedia)](https://en.wikipedia.org/wiki/IBeacon)
  - [Nordic Semiconductor nRF Connect for Android](https://play.google.com/store/apps/details?id=no.nordicsemi.android.mcp&hl=es_419&gl=US)
  - [Arduino Nano 33 BLE Sense Bluetooth Official Guide](https://docs.arduino.cc/tutorials/nano-33-ble-sense/bluetooth/)
  - [Espressif Arduino-ESP32 BluetoothSerial Library](https://github.com/espressif/arduino-esp32/tree/master/libraries/BluetoothSerial)

- **Official Specifications & API References:**
  - [Bluetooth Core Specification (Bluetooth SIG)](https://www.bluetooth.com/specifications/specs/)
  - [ArduinoBLE Official Library Documentation](https://www.arduino.cc/reference/en/libraries/arduinoble/)

---

## 🚀 How to Build & Run

### Running BLE Exercises (Exercises 3.1 – 3.4)
1. Open the desired sketch folder (`LB_IOT_P3_X`) in **Arduino IDE**.
2. Select **Arduino Nano 33 BLE** (or **ESP32 Dev Module** for 3.1) under `Tools > Board`.
3. Connect the microcontroller via USB and select its COM port.
4. Upload the code and open the **Serial Monitor** at **115200 baud**.
5. Launch **nRF Connect** on your smartphone, scan for devices (`Nano33_RGB_GATT` or `Nano33_Acelerometro`), connect, and explore the GATT table.

### Running Bluetooth Classic SPP Chat (Exercise 3.5)
1. Open `LB_IOT_P3_5_BT_Classic_SPP_Chat.ino` in **Arduino IDE**.
2. Select **ESP32 Dev Module** under `Tools > Board`.
3. Flash the code to the ESP32 and open the Serial Monitor at **115200 baud**.
4. On your mobile phone, open **System Bluetooth Settings**, discover, and pair with `ESP32_Chat_SPP`.
5. Open **Serial Bluetooth Terminal** app, connect to `ESP32_Chat_SPP` under *Devices > Bluetooth Classic*, and begin chatting between the smartphone terminal and the PC console.
```
