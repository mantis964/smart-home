
# Smart Home Environment Monitoring System

Embedded System using STM32F407, MQ-2 Gas Sensor, LDR, DHT22, LCD (HD44780), HC-05 Bluetooth, and Passive Buzzer

## Overview

This project implements a compact home-environment monitoring system using the STM32F407 Discovery board. It measures gas concentration, temperature, humidity, and ambient light, displays the results on a 16x2 LCD, and transmits sensor data wirelessly to a mobile device via Bluetooth (HC-05). An alarm system is included to alert the user when thresholds are exceeded.

The project integrates:

* MQ-2 gas sensor (LPG, Smoke, CO)
* DHT22 temperature and humidity sensor
* LDR for light detection
* HD44780-compatible 16x2 LCD in 4-bit mode
* HC-05 Bluetooth Classic module (UART TX/RX)
* Passive buzzer for alarms
* Manual reset button for alarm acknowledgment

This system can act as a safety and environmental monitoring node within a smart home.

---

## Hardware Components

* STM32F407 Discovery Board
* MQ-2 gas sensor (with level shifter for 5V to 3.3V)
* LDR module (D0 digital output)
* DHT22 sensor
* 16x2 LCD (JHD204A or compatible)
* HC-05 Bluetooth module
* Passive buzzer
* Reset push-button
* Jumper wires and breadboard
* 3.3V and 5V regulated supply (from STM32 board)

---

## Pin Connections

### LCD (4-bit mode)

| LCD Pin | STM32 Pin                  |
| ------- | -------------------------- |
| RS      | PB0                        |
| EN      | PB1                        |
| D4      | PB2                        |
| D5      | PB10                       |
| D6      | PB12                       |
| D7      | PB14                       |
| RW      | GND                        |
| VSS     | GND                        |
| VDD     | 5V                         |
| V0      | Potentiometer for contrast |

### Sensors

| Sensor            | STM32 Pin | Notes                              |
| ----------------- | --------- | ---------------------------------- |
| MQ-2 (Analog Out) | PA1       | Goes through level shifter         |
| LDR (D0 Digital)  | PA0       | Pulled-up input                    |
| DHT22 Data        | PB9       | Uses TIM2-based microsecond timing |

### Bluetooth HC-05

| HC-05 Pin | STM32 Pin        |
| --------- | ---------------- |
| TX        | PC11 (USART3_RX) |
| RX        | PC10 (USART3_TX) |
| VCC       | 5V               |
| GND       | GND              |
| KEY       | Not used         |

### Buzzer and Reset Button

| Component    | STM32 Pin              |
| ------------ | ---------------------- |
| Buzzer       | PC5 (Output)           |
| LED          | PC5 (Output)           |
| Reset Button | PA2 (Input, Pull-down) |

---

## Software Features

### 1. Sensor Data Acquisition

* MQ-2 gas sensor is read via ADC1. Calculations convert ADC voltage to Rs/Ro ratio and then approximate ppm for LPG, Smoke, and CO.
* LDR is read as a digital high/low input to detect ambient light.
* DHT22 uses a timing-based custom driver with TIM2 for microsecond delays and is sampled every 2 seconds.

### 2. LCD Display

Two formatted lines are displayed:

* Line 1: Temperature, Humidity, Light state
* Line 2: LPG and CO ppm values
  The layout is optimized to fit within the 16x2 screen.

### 3. Bluetooth Transmission

A formatted packet is sent every 500 ms via UART (HC-05):

```
TEMP=25.3,HUM=48.2,LDR=BRIGHT,LPG=120,SMOKE=60,CO=10,ALARM=0
```

This can be viewed on:

* Serial Bluetooth Terminal (Android)
* Any device supporting Bluetooth SPP (Classic)

### 4. Alarm System

Alarm triggers when any threshold is exceeded:

* Temperature > 35 °C
* LPG > 200 ppm
* Smoke > 300 ppm
* CO > 50 ppm

Alarm is latched using `alarm_latched` and continues until the user presses the reset button.

### 5. Manual Reset

The alarm is cleared when PA2 detects a button press.
LCD briefly displays "Alarm Reset".

### 6. LCD Boot Glitch Handling

An HD44780 wake-up sequence is applied at startup to avoid corrupted characters on first power-on.

---

## Project Folder Structure

```
Core/
  Inc/
    dht22.h
    lcd.h
  Src/
    main.c
    dht22.c
    lcd.c

Drivers/
  STM32F4xx_HAL_Driver/

USB_HOST/
...
```

---

## Build and Flash Instructions

1. Open the project in STM32CubeIDE.
2. Connect the STM32F407 Discovery board.
3. Build the project (Project > Build).
4. Flash to board (Run > Debug or Run > Run).
5. Open Bluetooth terminal on Android and pair with "Smart Home" (PIN 1234 or 0000).

---

## Debugging

### Checking buzzer pin (PC5)

A UART debug message can be added:

```
PC5_STATE=1 (HIGH~3.3V)
```

This helps verify whether alarms are being triggered correctly.

### LCD Issues

If random characters appear:

* Add 50–300 ms delay before LCD_Init.

---

## Future Improvements

* Add BLE-based mobile app for visualization - DONE
* Cloud connectivity (MQTT / WiFi)
* Data logging and graphs- DONE
* Enclosure design for deployment
* Multiple sensor nodes with wireless mesh

---


