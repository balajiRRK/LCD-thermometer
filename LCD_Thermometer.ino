```C++
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//SDA->21,SCL->22 
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

// Define constants
const int thermistorPin = 35; // Pin connected to the thermistor
const float referenceVoltage = 3.3;
const float referenceResistor = 10000; // the 'other' resistor
const float beta = 3950; // Beta value (Typical Value)
const float nominalTemperature = 25; // Nominal temperature for calculating the temperature coefficient
const float nominalResistance = 10000; // Resistance value at nominal temperature
const int buttonPin = 14; // Button Pin
int buttonState = 0;
int lastButtonState = 0;
int tempState = 0; // Fahreneight by default

void setup() {
  Serial.begin(115200); // Initialize serial communication
  pinMode(thermistorPin, INPUT); // Set pin as input
  lcd.init();// initialize the lcd 
  lcd.backlight(); // Turns on the LCD backlight.
  lcd.print("     Digital");   // Print a message to the LCD.
  lcd.setCursor(0, 1);
  lcd.print("Thermometer: ON");
  pinMode(buttonPin, INPUT);
  delay(3000);
}

void loop() {
  int adcValue = analogRead(thermistorPin); // Read ADC value
  float voltage = (adcValue * referenceVoltage) / 4095.0; // Calculate voltage
  float resistance = (voltage * referenceResistor) / (referenceVoltage - voltage); // Calculate thermistor resistance with updated configuration

  // Calculate temperature using the Beta parameter equation
  float tempK = 1 / (((log(resistance / nominalResistance)) / beta) + (1 / (nominalTemperature + 273.15)));
  
  float tempC = tempK - 273.15; // Get temperature in Celsius
  float tempF = 1.8 * tempC + 32.0; // Get temperature in Fahrenheit

  //Print temperature
  printf("TempC: %.2f C\n", tempC-7);
  printf("TempF: %.2f F\n", tempF-12);

  buttonState = digitalRead(buttonPin);

  if (buttonState == LOW && lastButtonState == HIGH)
  {
    delay(50);
    tempState = !tempState;   
  }

  if (tempState == LOW)
  {
    // Display temperature in F
    lcd.clear(); 
    lcd.setCursor(0, 0); // Sets the cursor position to the first row and first column (0, 0).
    lcd.print("Temp: ");
    lcd.print(tempF - 16, 0); // Prints the current value of the count variable.
    lcd.print(" F"); 
  } else
  {
    // Display temperature in C
    lcd.clear(); 
    lcd.setCursor(0, 0); // Sets the cursor position to the first row and first column (0, 0).
    lcd.print("Temp: ");
    lcd.print(tempC - 9, 0); // Prints the current value of the count variable.
    lcd.print(" C");
  }

  lastButtonState = buttonState;

  delay(100);
}
```