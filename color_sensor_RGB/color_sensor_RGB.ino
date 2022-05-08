#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display


// Define sensor color pins
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOUT 8

// Valorile rezultate in urma calibrarii
//int redMin = 5;
//int redMax = 111;
//int greenMin = 7;
//int greenMax = 130;
//int blueMin = 6;
//int blueMax = 116;

int redMin = 255;
int redMax = 0;
int greenMin = 255;
int greenMax = 0;
int blueMin = 255;
int blueMax = 0;

// variables for Color Pulse Width Measurements
int redPW = 0;
int greenPW = 0;
int bluePW = 0;

// variabile pentru valorile finale RGB
int redValue;
int greenValue;
int blueValue;


void setup() {
  lcd.init();
  lcd.clear();         
  lcd.backlight();      // Make sure backlight is on
  
  // S0 S1 S2 S3 sunt out-uri pt Arduino
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

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

  delay(20000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Obtained values:");
  
  lcd.setCursor(0,1);
  lcd.print("R:");
  lcd.print(redMin);
  lcd.print("  G:");
  lcd.print(greenMin);
  lcd.print("  B:");
  lcd.print(blueMin);
  
  delay(10000);
//  Serial.print("calibrated values min : red");
//  Serial.print(redMin);
//  Serial.print(" - green ");
//  Serial.print(greenMin);
//  Serial.print(" - blue ");
//  Serial.println(blueMin);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Calibrate Black!");
//  delay(20000);

  delay(10000);
  calibrateBlack();
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Obtained values:");
  
  lcd.setCursor(0,1);
  lcd.print("R:");
  lcd.print(redMax);
  lcd.print("G:");
  lcd.print(greenMax);
  lcd.print("B:");
  lcd.print(blueMax);
  delay(10000);

//  Serial.print("calibrated values max : red");
//  Serial.print(redMax);
//  Serial.print(" - green ");
//  Serial.print(greenMax);
//  Serial.print(" - blue ");
//  Serial.println(blueMax);
}

void loop() {
  // put your main code here, to run repeatedly:

  // citesc pulse width pt rosu
  redPW = getRedPW();
  // mapez valoarea pt red in 0-255
  // pulse width = 1 / frequency
  redValue = map(redPW, redMin, redMax, 255, 0);
  // delay pt stabilizare 
  delay(200);

  // citesc pulse width pt verde
  greenPW = getGreenPW();
  // mapez valoarea pt green in 0-255
  // pulse width = 1 / frequency
  greenValue = map(greenPW, greenMin, greenMax, 255, 0);
  // delay pt stabilizare 
  delay(200);

  // citesc pulse width pt blue
  bluePW = getBluePW();
  // mapez valoarea pt red in 0-255
  // pulse width = 1 / frequency
  blueValue = map(bluePW, blueMin, blueMax, 255, 0);
  // delay pt stabilizare 
  delay(200);

  // Printez rezultatele pe ecran
//  Serial.print("RED PW = ");
//  Serial.print(redValue);
//  Serial.print(" - GREEN PW = ");
//  Serial.print(greenValue);
//  Serial.print(" - BLUE PW = ");
//  Serial.println(blueValue);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Color detected:");

  if (redValue < 20 && blueValue < 20 && greenValue < 20) {
    lcd.setCursor(3,1);
    lcd.print("BLACK");
  } else if (redValue > 210 && blueValue > 210 && greenValue > 210) {
    lcd.setCursor(3,1);
    lcd.print("WHITE");
  } else if (redValue > blueValue && redValue > greenValue) {
    lcd.setCursor(3,1);
    lcd.print("RED");
  } else if (greenValue > redValue && greenValue > blueValue) {
    lcd.setCursor(3,1);
    lcd.print("GREEN");
  } else if (blueValue > redValue && blueValue > greenValue) {
    lcd.setCursor(3,1);
    lcd.print("BLUE");
  } else {
    lcd.setCursor(2,1);
    lcd.print("UNKNOWN :(");
  }
//  lcd.setCursor(0,1);
//  lcd.print("R:");
//  lcd.print(redValue);
//  lcd.print("G:");
//  lcd.print(greenValue);
//  lcd.print("B:");
//  lcd.print(blueValue);
}

// pt black trebuie sa iau cele mai mari valori
void calibrateWhite() {
  for (int i = 0; i < 2000; i++) {
    redPW = getRedPW();
    if (redPW < redMin) {
      redMin = redPW;
    }
  }
  for (int i = 0; i < 2000; i++) {
    greenPW = getGreenPW();
    if (greenPW < greenMin) {
      greenMin = greenPW;
    }
  }
  for (int i = 0; i < 2000; i++) {
    bluePW = getBluePW();
    if (bluePW < blueMin) {
      blueMin = bluePW;
    }
  }
}

// pt white trebuie luate cele mai mici valori
void calibrateBlack() {
  for (int i = 0; i < 2000; i++) {
    redPW = getRedPW();
    if (redPW > redMax) {
      redMax = redPW;
    }
  }
 for (int i = 0; i < 2000; i++) {
    greenPW = getGreenPW();
    if (greenPW > greenMax) {
      greenMax = greenPW;
    }
 }
  for (int i = 0; i < 2000; i++) {
    bluePW = getBluePW();
    if (bluePW > blueMax) {
      blueMax = bluePW;
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
