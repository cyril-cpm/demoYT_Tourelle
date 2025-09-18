//#define SERIAL_DEBUG

#include <HP_ESP32Servo.h>
#include <Settingator.h>
#include <ESPNowCommunicator.h>
#include <HardwareSerialCommunicator.h>
#include <MiscDef.h>
//#include <STR.h>
#include "CustomType.hpp"

#include <WebSocketCommunicator.h>

Servo triggerServo(GPIO_NUM_12);
Servo motorServo(GPIO_NUM_27);

#define TRIGGER_PIN 12
#define MOTOR_PIN 27

bool displayMotorPos = false;

#define CALLBACK_END_NOTIF 0x10

Settingator STR(nullptr);

STR_Float motorPos(0.0f, "POSITION");
STR_Float motorSpeed(0.0f, "SPEED");

void shoot()
{
  triggerServo.write(95);
  delay(1000);
  triggerServo.write(40);
}

void setup() {
    Serial.begin(115200);

    triggerServo.setSpeed(180.0);

    motorPos.SetCallback([](){
        motorServo.write(motorPos);
    });
    motorSpeed.SetCallback([](){
        motorServo.setSpeed(motorSpeed);
    });
    motorServo.setFadingCallback([](){ STR.SendNotif(CALLBACK_END_NOTIF); });

    motorServo.write(motorPos);
    triggerServo.write(40);


    //STR.SetCommunicator(HardwareSerialCTR::CreateInstance(115200));
    STR.AddSetting(Setting::Type::Trigger, nullptr, 0, "SHOOT", &shoot);

    ESPNowCore *espNowCore = nullptr;
    espNowCore = ESPNowCore::CreateInstance();
    if (espNowCore)
        espNowCore->BroadcastPing(); 
}

void loop () {
    STR.Update();
}