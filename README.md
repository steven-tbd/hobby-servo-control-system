# Hobby Servo Control System

A multi-modal system for controlling large arrays of hobby servos in 2-DOF (Degrees of Freedom) configurations using TouchDesigner and an ESP32 microcontroller.

This repository contains the project files for a scaleable framework designed to serve as the foundation for complex kinetic art installations, enabling the precise, choreographed movement of many individual 2-axis mechanisms.

![Servo Configuration](link/to/your/servo_configuration_image.png)

## Features

*   **2-DOF Mechanism Control:** The entire system, from the C++ code to the TouchDesigner network, is architected to control servos in X/Y pairs, creating 2-DOF movement.
*   **Multi-Modal Control:** The system can be controlled via three distinct input methods within TouchDesigner:
    *   **Real-time Gesture Control:** Using a Leap Motion sensor for direct, intuitive manipulation.
    *   **Manual Slider Control:** A simple UI for direct control over the X and Y axes of all servos.
    *   **Pre-Programmed Automation:** Using TouchDesigner's Animation COMP for precise, repeatable keyframe sequences.
        * (Update coming soon. See https://github.com/steven-tbd/robotic-arm-control-system for example of using Animation Comp to control servo movements)        
*   **Wireless-Capable & Robust Communication:** Utilizes an ESP32 to receive control data via OSC over UDP, a professional and flexible protocol for interactive installations.
*   **Scalable Architecture:** Employs a PCA9685 driver board to control large arrays of servos (12 servos / 6 2-DOF mechanisms in the prototype).

---

## I/O Diagram

```version-2
INPUT:  [Leap Motion Camera (Sensor)] ---> (USB HID) ---> [Computer/TouchDesigner (Processor/VPL)]
OUTPUT: [Computer/TouchDesigner (Processor/VPL)] ---> (WiFi/Network UDP) ---> [ESP32/C++ (Microcontroller Board/Arduino IDE)] ---> (I2C) ---> [PCA9685 (Motor Control Board)] ---> (PWM) ---> [12x Hobby Servos (Actuator)]
```

> - This workflow can switch between inputs from TouchDesigner: Leap Motion camera, manual, and pre-defined movement.
> - Motor movements are created using the Animation Comp in TouchDesigner.
> - For installations, the networked protocol connection should be switched to a tethered Serial link for reliability.

---

## Repository Contents

*   **ESP32:**
    *   `touchdesigner-udp-com.ino`: The C++ code for the ESP32. It connects to WiFi, listens for incoming OSC messages, and controls the X/Y servo pairs.
*   **TouchDesigner:**
    *   `motor-control-system_005v2-1.toe`: The multi-modal control network with switchable inputs for Leap Motion, Manual sliders, Programmed Movements, and a Reset position.

---

## Setup & Usage

### Hardware Requirements

*   ESP32 Development Board
*   PCA9685 16-Channel Servo Driver
*   EMAX ES08MA II Servos (or similar hobby servos)
*   Leap Motion Controller
*   External 5V power supply for the servos

### Software Requirements

*   TouchDesigner
*   Arduino IDE with the ESP32 board manager installed.
*   Required Arduino Libraries: `Adafruit_PWMServoDriver`, `OSC`

### Instructions

1.  **Configure WiFi:** Open `touchdesigner-udp-com.ino` and replace the placeholder `ssid` and `password` with your WiFi credentials.
2.  **Upload Code:** Upload the sketch to your ESP32 board.
3.  **Run TouchDesigner:** Open `motor-control-system_005v2-1.toe`. The network is configured to send OSC data to the ESP32's IP address (update the IP address in the `OSC Out CHOP`).
4.  **Interact:** Use the buttons at the top of the Touchdesigner network switch between inputs running through the switch_inputMaster Chop to the OSC Out chop.


### Project Links & Demos
*   **Project Write-up:** [https://stevenmbenton.com/hobby-servo-control-system/](https://stevenmbenton.com/hobby-servo-control-system/)
*   **YouTube Demo:** [Gesture-Controlled 2-axis mechanisms](https://youtu.be/UZ0vq4jCJZ0)
