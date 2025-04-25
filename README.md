# MEDICINE-DISPENSER
# 7-Day Dispenser System

This project controls a 3-axis stepper motor dispenser system using an Arduino, an LCD display, and a button interface. It is designed to dispense items corresponding to each day of the week.

## Features
- **Day Selection:**  
  Short button press cycles through days (Monday to Sunday).

- **Item Dispensing:**  
  Long button press (hold for more than 800 ms) activates the dispenser for the selected day.

- **Persistent Storage:**  
  The selected day is saved in EEPROM to retain the current day even after power cycles.

- **LCD Display:**  
  A 16x2 I2C LCD shows the system status, selected day, and dispensing status.

## Hardware Requirements
- Arduino-compatible board
- 16x2 I2C LCD display
- 3 Stepper motors with drivers
- Button
- Miscellaneous: wires, power supply, etc.

## Pin Configuration
| Component         | Arduino Pin |
|-------------------|-------------|
| Dispenser X Stepper | Pins 5 (step), 2 (direction) |
| Dispenser Y Stepper | Pins 6 (step), 3 (direction) |
| Dispenser Z Stepper | Pins 7 (step), 4 (direction) |
| Button             | A3          |
| LED (Optional)     | Pin 8       |

## Software Dependencies
- [LiquidCrystal_I2C](https://github.com/johnrickman/LiquidCrystal_I2C) — for the LCD screen
- [AccelStepper](https://www.airspayce.com/mikem/arduino/AccelStepper/) — for smooth and non-blocking stepper motor control
- EEPROM library (built-in)

## How It Works
1. **Startup:**  
   - Initializes the LCD and motors.
   - Reads the last selected day from EEPROM.

2. **Button Short Press:**  
   - Cycles through the days of the week.
   - Displays the currently selected day.
   - Saves the selected day to EEPROM.

3. **Button Long Press (>800ms):**  
   - Moves each stepper motor to a target position based on the selected day.
   - Displays dispensing progress and completion on the LCD.

4. **Stepper Movement:**  
   - Each day corresponds to a specific slot.
   - `stepsPerSlot` defines the distance between days (slots) — default is 2000 steps.

## Customization
- **stepsPerSlot:**  
  Adjust this constant to match your physical hardware layout.

- **Motor Speed and Acceleration:**  
  You can fine-tune the `setMaxSpeed` and `setAcceleration` values for smoother or faster movements.

## Important Notes
- Ensure all stepper motors are properly powered and wired according to your driver requirements.
- Debounce delay is set to 200ms after button presses.
- LCD I2C address is set to `0x27` by default. Adjust it if your display uses a different address.

## Example System Layout

- **Button**  
  Connect one side of the push button to **A3** and the other side to **GND**. (Internal pull-up resistor is enabled.)

- **LCD Display (I2C)**  
  Connect the LCD to the Arduino I2C pins:  
  - **SDA** → **A4** (on Arduino Uno)  
  - **SCL** → **A5** (on Arduino Uno)  
  - **VCC** → **5V**  
  - **GND** → **GND**

- **Stepper Motor Drivers** (e.g., A4988, TMC2208, etc.)  
  Each motor requires:
  - **STEP pin**: Connected to Arduino
  - **DIR pin**: Connected to Arduino

| Stepper Motor | Step Pin | Direction Pin |
|:-------------:|:--------:|:-------------:|
| X-axis        | 5        | 2             |
| Y-axis        | 6        | 3             |
| Z-axis        | 7        | 4             |

- **Optional LED**  
  Connect an LED to **Pin 8** through a suitable resistor (220Ω recommended).

---
 
