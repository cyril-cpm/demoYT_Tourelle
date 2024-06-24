#include <ESP32Servo.h>
#include <STR.h>
#include <ESPNowCommunicator.h>

Servo myServo;

#define G_PIN 32
#define D_PIN 25
#define SERVO_PIN 12

uint8_t movingSpeed = 255;

void stopMove()
{
  analogWrite(G_PIN, 0);
  analogWrite(D_PIN, 0);
  delay(250);
}

void moveLeft()
{
  stopMove();
  //digitalWrite(G_PIN, HIGH);
  

  analogWrite(G_PIN, movingSpeed);
}

void moveRight() 
{
  stopMove();
  //digitalWrite(D_PIN, HIGH);
 

  analogWrite(D_PIN, movingSpeed);
}

void shoot()
{
  myServo.write(95);
  delay(1000);
  myServo.write(40);
}

void setup() {
  //Serial.begin(9600);
  analogWriteResolution(8);
  analogWriteFrequency(100);
  myServo.attach(SERVO_PIN);
  myServo.write(40);
  //INIT_DEFAULT_SETTINGATOR();
  STR.SetCommunicator(ESPNowCTR::CreateInstanceDiscoverableWithSSID("Turret"));
  pinMode(G_PIN, OUTPUT);
  pinMode(D_PIN, OUTPUT);
  analogWrite(G_PIN, 0);
  analogWrite(D_PIN, 0);

  STR.AddSetting(Setting::Type::Slider, &movingSpeed, sizeof(uint8_t), "SPEED");
  STR.AddSetting(Setting::Type::Trigger, nullptr, 0, "GAUCHE", &moveLeft);
  STR.AddSetting(Setting::Type::Trigger, nullptr, 0, "STOP", &stopMove);
  STR.AddSetting(Setting::Type::Trigger, nullptr, 0, "DROITE", &moveRight);
  STR.AddSetting(Setting::Type::Trigger, nullptr, 0, "SHOOT", &shoot);
}

void loop () {
  STR.Update();
}