//#define SERIAL_DEBUG

#include <ESP32Servo.h>
#include <Settingator.h>
#include <ESPNowCommunicator.h>
#include <HardwareSerialCommunicator.h>
#include <MiscDef.h>

Servo myServo;

#define G_PIN 32
#define D_PIN 25
#define SERVO_PIN 12

uint8_t movingSpeed = 255;

ulong motorTime = micros();

#define NONE 0
#define LEFT 1
#define RIGHT 2
#define PULSE_WIDTH 30000 //micro seconds

int side = 0;
bool leftMotorRunning = false;
bool rightMotorRunning  = false;

bool motorOnTab[256] = { false };

void updateMotorOnTab()
{
  bzero(motorOnTab, 256);
  if (!movingSpeed)
    return;

  float interval = 255.0 / (float)movingSpeed;

  int tabIndex = 0;

  for (int i = 0; i < movingSpeed; i++)
  {
    tabIndex = (int)(i * interval);
    motorOnTab[tabIndex] = true;
  }
}

void updateMotor()
{
    ulong currentTime = micros();
    ulong elapsedTime = currentTime - motorTime;

    ulong dutyStep = elapsedTime / PULSE_WIDTH;

    if (motorOnTab[dutyStep])
    {
        if (side == LEFT && !leftMotorRunning && !rightMotorRunning)
        {
            digitalWrite(G_PIN, HIGH);
            leftMotorRunning = true;
        }
        else if (side == RIGHT && !rightMotorRunning && !leftMotorRunning)
        {
            digitalWrite(D_PIN, HIGH);
            rightMotorRunning = true;
        }

    }
    else
    {
        if (side == LEFT && leftMotorRunning)
        {
            digitalWrite(G_PIN, LOW);
            leftMotorRunning = false;
        }
        else if (side == RIGHT && rightMotorRunning)
        {
            digitalWrite(D_PIN, LOW);
            rightMotorRunning = false;
        }
    }

    if (dutyStep > 255)
        motorTime = currentTime;
}

void stopMove()
{
  digitalWrite(G_PIN, LOW);
  digitalWrite(D_PIN, LOW);
  leftMotorRunning = false;
  rightMotorRunning = false;
  side = NONE;
  delay(250);
}

void moveLeft()
{
  stopMove();
  
  side = LEFT;
}

void moveRight() 
{
  stopMove();
 
  side = RIGHT;
}

void shoot()
{
  myServo.write(95);
  delay(1000);
  myServo.write(40);
}

void setup() {
  //Serial.begin(9600);

  myServo.attach(SERVO_PIN);
  myServo.write(40);

  STR.SetCommunicator(ESPNowCTR::CreateInstanceDiscoverableWithSSID("Turret"));
  //STR.SetCommunicator(HardwareSerialCTR::CreateInstance(9600));
  updateMotorOnTab();
  pinMode(G_PIN, OUTPUT);
  pinMode(D_PIN, OUTPUT);
  digitalWrite(G_PIN, LOW);
  digitalWrite(D_PIN, LOW);

  STR.AddSetting(Setting::Type::Slider, &movingSpeed, sizeof(uint8_t), "SPEED", &updateMotorOnTab);
  STR.AddSetting(Setting::Type::Trigger, nullptr, 0, "GAUCHE", &moveLeft);
  STR.AddSetting(Setting::Type::Trigger, nullptr, 0, "STOP", &stopMove);
  STR.AddSetting(Setting::Type::Trigger, nullptr, 0, "DROITE", &moveRight);
  STR.AddSetting(Setting::Type::Trigger, nullptr, 0, "SHOOT", &shoot);
}

void loop () {
  STR.Update();
  updateMotor();
}