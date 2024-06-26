#include <ESP32Servo.h>
#include <STR.h>
#include <ESPNowCommunicator.h>

Servo myServo;

#define G_PIN 32
#define D_PIN 25
#define SERVO_PIN 12

uint8_t movingSpeed = 255;

ulong motorTime = micros();

#define NONE 0
#define LEFT 1
#define RIGHT 2

int side = 0;
bool leftMotorRunning = false;
bool rightMotorRunning  = false;

void updateMotor()
{
    ulong currentTime = micros();
    ulong elapsedTime = currentTime - motorTime;

    ulong duty = elapsedTime / 3000;

    if (duty < movingSpeed)
    {
        if (side == LEFT && !leftMotorRunning)
        {
            digitalWrite(G_PIN, HIGH);
            leftMotorRunning = true;
        }
        else if (side == RIGHT && !rightMotorRunning)
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

    if (duty > 255)
        motorTime = currentTime;
}

void stopMove()
{
  digitalWrite(G_PIN, 0);
  digitalWrite(D_PIN, 0);
  side = NONE;
  delay(250);
}

void moveLeft()
{
  stopMove();
  //digitalWrite(G_PIN, HIGH);
  
    side = LEFT;
  //analogWrite(G_PIN, movingSpeed);
}

void moveRight() 
{
  stopMove();
  //digitalWrite(D_PIN, HIGH);
 
    side = RIGHT;
  //analogWrite(D_PIN, movingSpeed);
}

void shoot()
{
  myServo.write(95);
  delay(1000);
  myServo.write(40);
}

void setup() {
  //Serial.begin(9600);
  //analogWriteResolution(8);
  //analogWriteFrequency(25);
  myServo.attach(SERVO_PIN);
  myServo.write(40);
  //INIT_DEFAULT_SETTINGATOR();
  STR.SetCommunicator(ESPNowCTR::CreateInstanceDiscoverableWithSSID("Turret"));
  pinMode(G_PIN, OUTPUT);
  pinMode(D_PIN, OUTPUT);
  //analogWrite(G_PIN, 0);
  //analogWrite(D_PIN, 0);
    digitalWrite(G_PIN, LOW);
    digitalWrite(D_PIN, LOW);

  STR.AddSetting(Setting::Type::Slider, &movingSpeed, sizeof(uint8_t), "SPEED");
  STR.AddSetting(Setting::Type::Trigger, nullptr, 0, "GAUCHE", &moveLeft);
  STR.AddSetting(Setting::Type::Trigger, nullptr, 0, "STOP", &stopMove);
  STR.AddSetting(Setting::Type::Trigger, nullptr, 0, "DROITE", &moveRight);
  STR.AddSetting(Setting::Type::Trigger, nullptr, 0, "SHOOT", &shoot);
}

void loop () {
  STR.Update();
  updateMotor();
}