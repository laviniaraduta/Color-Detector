#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display


// Define sensor color pins
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOUT 8
#define buttonPin 3

#define RED 0
#define GREEN 1
#define BLUE 2

int buttonState = 0;
// Valorile rezultate in urma calibrarii
int minValues[3] = {255, 255, 255};
int maxValues[3] = {0, 0, 0};
//int redMin = 255;
//int redMax = 0;
//int greenMin = 255;
//int greenMax = 0;
//int blueMin = 255;
//int blueMax = 0;

// variables for Color Pulse Width Measurements
int redPW = 0;
int greenPW = 0;
int bluePW = 0;

// variabile pentru valorile finale RGB
int redValue;
int greenValue;
int blueValue;

byte customChar[] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

void setup() {
  lcd.init();
  lcd.clear();         
  lcd.backlight();      // Make sure backlight is on
  
  // S0 S1 S2 S3 sunt out-uri pt Arduino
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(buttonPin, INPUT);

  // sensorOUT este input pt placuta
  pinMode(sensorOUT, INPUT);

  // Pulse width scalling 100%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  // Serial Monitor
  Serial.begin(9600);

  

  lcd.setCursor(0,0);
  lcd.print("Calibrate White!");
  calibrateWhite();
  display_bar();
  display_PWs(minValues);

  
  Serial.print("calibrated values min : red");
  Serial.print(minValues[RED]);
  Serial.print(" - green ");
  Serial.print(minValues[GREEN]);
  Serial.print(" - blue ");
  Serial.println(minValues[BLUE]);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Calibrate Black!");

  calibrateBlack();
  display_bar();
  display_PWs(maxValues);

  Serial.print("calibrated values max : red");
  Serial.print(maxValues[RED]);
  Serial.print(" - green ");
  Serial.print(maxValues[GREEN]);
  Serial.print(" - blue ");
  Serial.println(maxValues[BLUE]);
}

void loop() {
  
  // put your main code here, to run repeatedly:
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    credits();
  }
  // citesc pulse width pt rosu
  redPW = getRedPW();
  // mapez valoarea pt red in 0-255
  // pulse width = 1 / frequency
  redValue = map(redPW, minValues[RED], maxValues[RED], 255, 0);
  // delay pt stabilizare 
  delay(200);

  // citesc pulse width pt verde
  greenPW = getGreenPW();
  // mapez valoarea pt green in 0-255
  // pulse width = 1 / frequency
  greenValue = map(greenPW, minValues[GREEN], maxValues[GREEN], 255, 0);
  // delay pt stabilizare 
  delay(200);

  // citesc pulse width pt blue
  bluePW = getBluePW();
  // mapez valoarea pt red in 0-255
  // pulse width = 1 / frequency
  blueValue = map(bluePW, minValues[BLUE], maxValues[BLUE], 255, 0);
  // delay pt stabilizare 
  delay(200);

  // Printez rezultatele pe ecran
  Serial.print("RED PW = ");
  Serial.print(redValue);
  Serial.print(" - GREEN PW = ");
  Serial.print(greenValue);
  Serial.print(" - BLUE PW = ");
  Serial.print(blueValue);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Color detected:");

  if (redValue < 20 && blueValue < 20 && greenValue < 20) {
    lcd.setCursor(3,1);
    lcd.print("BLACK");
    Serial.println(" - BLACK");
  } else if (redValue > 210 && blueValue > 210 && greenValue > 210) {
    lcd.setCursor(3,1);
    lcd.print("WHITE");
    Serial.println(" - WHITE");
  } else if (redValue > 200 && greenValue > 200 && blueValue < 200) {
    lcd.setCursor(3,1);
    lcd.print("YELLOW");
    Serial.println(" - YELLOW");
  } else if (redValue > blueValue && redValue > greenValue) {
    lcd.setCursor(3,1);
    lcd.print("RED");
    Serial.println(" - RED");
  } else if (greenValue > redValue && greenValue > blueValue) {
    lcd.setCursor(3,1);
    lcd.print("GREEN");
    Serial.println(" - GREEN");
  } else if (blueValue > redValue && blueValue > greenValue) {
    lcd.setCursor(3,1);
    lcd.print("BLUE");
    Serial.println(" - BLUE");
  } else {
    lcd.setCursor(2,1);
    lcd.print("UNKNOWN :(");
    Serial.println(" - UNKNOWN :(");
  }
//  lcd.setCursor(0,1);
//  lcd.print("R:");
//  lcd.print(redValue);
//  lcd.print("G:");
//  lcd.print(greenValue);
//  lcd.print("B:");
//  lcd.print(blueValue);

}

void display_PWs(int vals[]) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Obtained values:");
  
  lcd.setCursor(0,1);
  lcd.print("R:");
  lcd.print(vals[RED]);
  lcd.print("G:");
  lcd.print(vals[GREEN]);
  lcd.print("B:");
  lcd.print(vals[BLUE]);
  delay(2000);
}

void display_bar() {
  lcd.createChar(0, customChar);
  lcd.setCursor(0,1);
  for (int i = 0; i < 16; i++) {   
    lcd.write(0);
    delay(500);
  }
}

void credits() {
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Thank you! :)");
  while (true) {
  }
}

// pt black trebuie sa iau cele mai mari valori
void calibrateWhite() {
  lcd.setCursor(0, 1);
  for (int i = 0; i < 2000; i++) {
    redPW = getRedPW();
    if (redPW < minValues[RED]) {
      minValues[RED] = redPW;
    }
  }
  for (int i = 0; i < 2000; i++) {
    greenPW = getGreenPW();
    if (greenPW < minValues[GREEN]) {
      minValues[GREEN] = greenPW;
    }
  }
  for (int i = 0; i < 2000; i++) {
    bluePW = getBluePW();
    if (bluePW < minValues[BLUE]) {
      minValues[BLUE] = bluePW;
    }
  }
}

// pt white trebuie luate cele mai mici valori
void calibrateBlack() {
  lcd.setCursor(0, 1);
  for (int i = 0; i < 2000; i++) {
    redPW = getRedPW();
    if (redPW > maxValues[RED]) {
      maxValues[RED] = redPW;
    }
  }
 for (int i = 0; i < 2000; i++) {
    greenPW = getGreenPW();
    if (greenPW > maxValues[GREEN]) {
      maxValues[GREEN] = greenPW;
    }
 }
  for (int i = 0; i < 2000; i++) {
    bluePW = getBluePW();
    if (bluePW > maxValues[BLUE]) {
      maxValues[BLUE] = bluePW;
    }
  }
}


int getRedPW() {
  // ii spun senzorului sa citeasca numai rosu
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  // numar cat timp pulsul sta pe low
  int PW = pulseIn(sensorOUT, LOW);
  return PW;
}

int getGreenPW() {
  // ii spun senzorului sa citeasca numai verde
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  // numar cat timp pulsul sta pe low
  int PW = pulseIn(sensorOUT, LOW);
  return PW;
}

int getBluePW() {
  // ii spun senzorului sa citeasca numai albastru
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  // numar cat timp pulsul sta pe low
  int PW = pulseIn(sensorOUT, LOW);
  return PW;
}
