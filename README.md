# TinyML Gesture Recognition on NodeMCU (ESP8266)

Real-time gesture classification project using a NodeMCU (ESP8266), an MPU6050 accelerometer, and an RGB LED, powered by a Machine Learning model trained with Edge Impulse.

## 🚀 Features
- Real-time Sensor Polling: Samples accelerometer data (~59 Hz) dynamically from the MPU6050[cite: 2].
- Edge Impulse Inference: Runs an embedded EON-compiled TinyML model directly on the microcontroller.
- Hardware Feedback: Maps recognized gestures to an RGB LED output in real-time (`circle`, `up_down`, `left_right`, `idle`)[cite: 2].

---

## 🛠️ Hardware Requirements
- NodeMCU (ESP8266) development board[cite: 2]
- MPU6050 Accelerometer / Gyroscope module (I2C)[cite: 2]
- RGB LED (Common Cathode or Anode) with current-limiting resistors (e.g., 220Ω)[cite: 2]
- Connecting wires and breadboard

---

## 📌 Pin Configuration

| Component | NodeMCU Pin | Description |
| :--- | :--- | :--- |
| MPU6050 (SDA) | `D2` (GPIO 4) | I2C Data |
| MPU6050 (SCL) | `D1` (GPIO 5) | I2C Clock |
| RGB LED (Red) | D5` | Red channel output[cite: 2] |
| RGB LED (Green) | `D6` | Green channel output[cite: 2] |
| RGB LED (Blue) | `D7` | Blue channel output[cite: 2] |

---

## 💻 Software & Libraries
- Arduino IDE[cite: 2]
- Adafruit MPU6050 Library[cite: 2]
- Adafruit Unified Sensor Library[cite: 2]
- Edge Impulse Arduino SDK (Custom trained model export)[cite: 2]

---

## ⚙️ How to Run
1. Clone or download this repository.
2. Open the `.ino` sketch in the Arduino IDE[cite: 2].
3. Install the required dependencies via the Arduino Library Manager (`Adafruit MPU6050`).
4. Import your exported Edge Impulse Arduino library as a `.zip` file (`Sketch > Include Library > Add .ZIP Library`).
5. Select your board (NodeMCU 1.0 (ESP-12E Module))[cite: 7] and correct COM port.
6. Upload the code and open the Serial Monitor at `115200` baud[cite: 2].
