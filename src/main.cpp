#include <ESP32Servo.h>
#include <STR.h>
#include <ESPNowCommunicator.h>

Servo myServo;

#define G_PIN 32
#define D_PIN 25
#define SERVO_PIN 12

void stopMove()
{
  digitalWrite(G_PIN, LOW);
  digitalWrite(D_PIN, LOW);
  delay(1500);
}

void moveLeft()
{
  stopMove();
  digitalWrite(G_PIN, HIGH);
}

void moveRight() 
{
  stopMove();
  digitalWrite(D_PIN, HIGH);
}

void shoot()
{
  myServo.write(95);
  delay(1000);
  myServo.write(40);
}

void setup() {
  Serial.begin(9600);
  myServo.attach(SERVO_PIN);
  myServo.write(40);
  //INIT_DEFAULT_SETTINGATOR();
  STR.SetCommunicator(ESPNowCTR::CreateInstance());
  pinMode(G_PIN, OUTPUT);
  pinMode(D_PIN, OUTPUT);
  digitalWrite(G_PIN, LOW);
  digitalWrite(D_PIN, LOW);

  STR.AddSetting(Setting::Type::Trigger, nullptr, 0, "GAUCHE", &moveLeft);
  STR.AddSetting(Setting::Type::Trigger, nullptr, 0, "STOP", &stopMove);
  STR.AddSetting(Setting::Type::Trigger, nullptr, 0, "DROITE", &moveRight);
  STR.AddSetting(Setting::Type::Trigger, nullptr, 0, "SHOOT", &shoot);
}

void loop () {
  STR.Update();
}