#include <LiquidCrystal_I2C.h>
#include <AccelStepper.h>
#include <EEPROM.h>

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Stepper Motors
AccelStepper dispenserZ(AccelStepper::DRIVER, 7, 4);
AccelStepper dispenserX(AccelStepper::DRIVER, 5, 2);
AccelStepper dispenserY(AccelStepper::DRIVER, 6, 3);

// Button
const int buttonPin = A3;
unsigned long buttonPressTime = 0;
bool buttonHeld = false;

// Config
const int stepsPerSlot = 2000;
int currentDay = 0;
String days[7] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

void setup() {
  Serial.begin(9600);

  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);

  dispenserX.setMaxSpeed(500); dispenserX.setAcceleration(300);
  dispenserY.setMaxSpeed(500); dispenserY.setAcceleration(300);
  dispenserZ.setMaxSpeed(500); dispenserZ.setAcceleration(300);

  lcd.init();
  lcd.backlight();

  pinMode(buttonPin, INPUT_PULLUP);

  currentDay = EEPROM.read(0);
  if (currentDay < 0 || currentDay > 6) currentDay = 0;

  lcd.setCursor(0, 0);
  lcd.print("Dispenser Ready");
  lcd.setCursor(0, 1);
  lcd.print("Day: ");
  lcd.print(days[currentDay]);
}

void loop() {
  dispenserX.run();
  dispenserY.run();
  dispenserZ.run();

  if (digitalRead(buttonPin) == LOW && !buttonHeld) {
    buttonPressTime = millis();
    buttonHeld = true;
  }

  if (digitalRead(buttonPin) == HIGH && buttonHeld) {
    unsigned long duration = millis() - buttonPressTime;
    buttonHeld = false;

    if (duration > 800) {
      // Long press - dispense
      dispenseToday();
    } else {
      // Short press - cycle to next day
      currentDay = (currentDay + 1) % 7;
      EEPROM.write(0, currentDay);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Select Day:");
      lcd.setCursor(0, 1);
      lcd.print(days[currentDay]);
    }
    delay(200); // debounce
  }
}

void dispenseToday() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dispensing:");
  lcd.setCursor(0, 1);
  lcd.print(days[currentDay]);

  long targetPosition = currentDay * stepsPerSlot;

  dispenserX.moveTo(targetPosition);
  dispenserY.moveTo(targetPosition);
  dispenserZ.moveTo(targetPosition);

  while (dispenserX.distanceToGo() != 0 ||
         dispenserY.distanceToGo() != 0 ||
         dispenserZ.distanceToGo() != 0) {
    dispenserX.run();
    dispenserY.run();
    dispenserZ.run();
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Done!");
  lcd.setCursor(0, 1);
  lcd.print("Next: ");
  lcd.print(days[currentDay]);

  delay(1000);
}
