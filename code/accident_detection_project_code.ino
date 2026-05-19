#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

// LCD (16x2 I2C)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// MPU6050
Adafruit_MPU6050 mpu;

// SoftwareSerial: GPS (RX->4, TX->3), GSM (RX->7, TX->8)
SoftwareSerial gpsSerial(4, 3);
SoftwareSerial gsmSerial(7, 8);
TinyGPSPlus gps;

// Pins
#define NO_CASUALTY_BUTTON 5
#define BUZZER 6

// Emergency Contacts
const char* emergencyNumbers[] = {"+911234567890", "+911234567890"};
const int numContacts = 2;

bool accidentDetected = false;

// ─── SETUP ───────────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(9600);

  gpsSerial.begin(9600);
  gsmSerial.begin(9600);
  delay(3000);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("System Starting");

  Wire.begin();
  if (!mpu.begin()) {
    lcd.setCursor(0, 1);
    lcd.print("MPU6050 Error!");
    while (1);
  }

  pinMode(NO_CASUALTY_BUTTON, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Initialized!");

  // Test GSM module
  gsmSerial.listen();
  gsmSerial.println("AT");
  delay(1000);
  while (gsmSerial.available()) {
    Serial.write(gsmSerial.read());
  }

  // Switch back to GPS
  gpsSerial.listen();
}

// ─── LOOP ────────────────────────────────────────────────────────────────────
void loop() {
  // Read GPS in background (gpsSerial is listening)
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  // MPU6050 readings
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Adafruit MPU6050 returns m/s², not g-force
  // At rest Z = ~9.8 m/s² (gravity), so only check X and Y for impact
  // Threshold set to 25 m/s² (~2.5g) — filters hard braking & speed bumps
  // Only real accident-level impacts will trigger this
  if (!accidentDetected &&
     (abs(a.acceleration.x) > 25 || abs(a.acceleration.y) > 25)) {

    accidentDetected = true;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Accident!!");
    lcd.setCursor(0, 1);
    lcd.print("Sending Alert...");

    digitalWrite(BUZZER, HIGH);
    sendAlert();
    delay(10000); // 10 second cancellation window

    // Reset after timeout if button not pressed
    accidentDetected = false;
    digitalWrite(BUZZER, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Monitoring...");
  }

  // If button pressed during alert — send no casualty SMS
  if (accidentDetected && digitalRead(NO_CASUALTY_BUTTON) == LOW) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("No Casualty");
    sendNoCasualtyAlert();
    digitalWrite(BUZZER, LOW);
    accidentDetected = false;
  }

  delay(500);
}

// ─── SEND ACCIDENT ALERT ─────────────────────────────────────────────────────
void sendAlert() {
  String location = getGPSLocation();

  gsmSerial.listen(); // switch SoftwareSerial to GSM before sending

  for (int i = 0; i < numContacts; i++) {
    gsmSerial.println("AT+CMGF=1");
    delay(500);
    gsmSerial.print("AT+CMGS=\"");
    gsmSerial.print(emergencyNumbers[i]);
    gsmSerial.println("\"");
    delay(500);
    gsmSerial.print("Accident Detected! Location: ");
    gsmSerial.print(location);
    gsmSerial.write(26); // Ctrl+Z to send SMS
    delay(5000);
  }

  gpsSerial.listen(); // switch back to GPS after sending
}

// ─── SEND NO CASUALTY ALERT ──────────────────────────────────────────────────
void sendNoCasualtyAlert() {
  gsmSerial.listen(); // switch SoftwareSerial to GSM before sending

  for (int i = 0; i < numContacts; i++) {
    gsmSerial.println("AT+CMGF=1");
    delay(500);
    gsmSerial.print("AT+CMGS=\"");
    gsmSerial.print(emergencyNumbers[i]);
    gsmSerial.println("\"");
    delay(500);
    gsmSerial.print("No Casualty. Ignore Alert.");
    gsmSerial.write(26); // Ctrl+Z to send SMS
    delay(5000);
  }

  gpsSerial.listen(); // switch back to GPS after sending
}

// ─── GET GPS LOCATION ────────────────────────────────────────────────────────
String getGPSLocation() {
  if (gps.location.isValid()) {
    String lat = String(gps.location.lat(), 6);
    String lng = String(gps.location.lng(), 6);
    return "Lat:" + lat + ", Lon:" + lng;
  } else {
    return "GPS Unavailable";
  }
}