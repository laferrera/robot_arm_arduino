/*

   File:        basic_ble_peripheral
   Description: Implements a basic BLE peripheral for device testing
   Author:      Rob Kerr (mail@robkerr.com)
   Date:        6 AUG 2021

*/
#include <M5Stack.h>
#include <MyCobotProBasic.h>
#include <ParameterProList.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define PERIPHERAL_NAME             "mrRobot Peripheral"
#define SERVICE_NAME                "mrRobot service"
#define SERVICE_UUID                "2220"
#define CHARACTERISTIC_UUID   "2222"
//#define CHARACTERISTIC_OUTPUT_UUID  "2221" //this is to SEND to iPad but doesn't work for whate

MyCobotProBasic myCobotpro;
//Angles angles = { -0.0, -0.0, -0.0, -0.0, -0.0, -0.0 };
int angleA = 0;
int angleB = 0;
int angleC = 0;
int angleD = 0;
int angleE = 0;
int angleF = 0;

int moveSpeed = 20;

// Current value of output characteristic persisted here
//static uint8_t outputData[1];
static uint8_t inputData[1];

// Output characteristic is used to send the response back to the connected phone
//BLECharacteristic *pOutputChar;

// Class defines methods called when a device connects and disconnects from the service
class ServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      Serial.println("BLE Client Connected");
      M5.Lcd.println("BLE Client Connected");
    }
    void onDisconnect(BLEServer* pServer) {
      BLEDevice::startAdvertising();
      Serial.println("BLE Client Disconnected");
      M5.Lcd.println("BLE Client Disconnected");
    }
};

class InputReceivedCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharWriteState) {
      uint8_t *inputValues = pCharWriteState->getData();

//      M5.Lcd.println("Input Values");
//      M5.Lcd.println(String(*inputValues));
//        M5.Lcd.fillScreen(BLACK);
//        M5.Lcd.setCursor(0, 0);

      switch (*inputValues) {
        case 0x00: // button UP
          Serial.printf("Moving UP");
          M5.Lcd.fillScreen(BLACK);
          M5.Lcd.setCursor(0, 0);
          M5.Lcd.println("Moving UP");
          myCobotpro.writeAngle(2, angleB++, moveSpeed);
          break;
        case 0x01: // button DOWN
          Serial.printf("Moving DOWN");
          M5.Lcd.fillScreen(BLACK);
          M5.Lcd.setCursor(0, 0);          
          M5.Lcd.println("Moving DOWN");
          myCobotpro.writeAngle(2, angleB--, moveSpeed);
          break;
        case 0x02: // button LEFT
          Serial.printf("Moving LEFT");
          M5.Lcd.fillScreen(BLACK);
          M5.Lcd.setCursor(0, 0);          
          M5.Lcd.println("Moving LEFT");
          myCobotpro.writeAngle(1, angleA++, moveSpeed);
          break;
        case 0x03: // button RIGHT
          Serial.printf("Moving RIGHT");
          M5.Lcd.fillScreen(BLACK);
          M5.Lcd.setCursor(0, 0);          
          M5.Lcd.println("Moving RIGHT");
          myCobotpro.writeAngle(1, angleA--, moveSpeed);
          break;
        case 0x04: // button vertical UP
          Serial.printf("Moving OUT");
          M5.Lcd.fillScreen(BLACK);
          M5.Lcd.setCursor(0, 0);          
          M5.Lcd.println("Moving OUT");
          myCobotpro.writeAngle(3, angleC++, moveSpeed);
          myCobotpro.writeAngle(4, angleD++, moveSpeed);
          break;
        case 0x05: // button vertical DOWN
          Serial.printf("Moving IN");
          M5.Lcd.fillScreen(BLACK);
          M5.Lcd.setCursor(0, 0);          
          M5.Lcd.println("Moving IN");
          myCobotpro.writeAngle(3, angleC--, moveSpeed);
          myCobotpro.writeAngle(4, angleD--, moveSpeed);
          break;

      }



    }
};

void setup() {
  // Use the Arduino serial monitor set to this baud rate to view BLE peripheral logs
  Serial.begin(115200);
  Serial.println("Begin Setup BLE Service and Characteristics");

  // Configure thes server

  BLEDevice::init(PERIPHERAL_NAME);
  BLEServer *pServer = BLEDevice::createServer();

  // Create the service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a characteristic for the service
  BLECharacteristic *pInputChar = pService->createCharacteristic(
                                    CHARACTERISTIC_UUID,
                                    BLECharacteristic::PROPERTY_WRITE_NR | BLECharacteristic::PROPERTY_WRITE);


//  BLECharacteristic *pOutputChar = pService->createCharacteristic(
//                            CHARACTERISTIC_OUTPUT_UUID,
//                          BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);



  // Hook callback to report server events
  pServer->setCallbacks(new ServerCallbacks());
  pInputChar->setCallbacks(new InputReceivedCallbacks());

  // Initial characteristic value
  //outputData[0] = 0x00;
  //pOutputChar->setValue((uint8_t *)outputData, 1);

  // Start the service
  pService->start();

  // Advertise the service
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  Serial.println("BLE Service is advertising");

  M5.begin();
  myCobotpro.setup();
  //打开扭矩输出
  myCobotpro.powerOn();
  //赋值容器内部元素


  myCobotpro.writeAngle(2, angleA, 50); //Set to home position
  delay(2000);
  myCobotpro.writeAngle(3, angleB, 50); //Set to home position
  delay(2000);
  myCobotpro.writeAngle(4, angleC, 50); //Set to home position
  delay(2000);
  myCobotpro.writeAngle(5, angleD, 50); //Set to home position
  delay(2000);
  myCobotpro.writeAngle(6, angleE, 50); //Set to home position
  delay(2000);
  myCobotpro.writeAngle(7, angleF, 50); //Set to home position
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:

}

/*
  ButtonUp - touchstart : 0x0
  ButtonUp - touchend : 0x10
  ButtonDown - touchstart : 0x1
  ButtonDown - touchend : 0x11
  ButtonLeft - touchstart : 0x2
  ButtonLeft - touchend : 0x12
  ButtonRight - touchstart : 0x3
  ButtonRight - touchend : 0x13

  buttonVerticalUp - touchstart: 0x4
  buttonVerticalUp - touchend: 0x14
  buttonVerticalDown - touchstart: 0x5
  buttonVerticalDown - touchend: 0x15

  buttonExpand - touchstart: 0x6
  buttonExpand - touchend: 0x16
  buttonCollapse - touchstart: 0x7
  buttonCollapse - touchend: 0x17

  button4g5g - 4g mode: 0x18
  button4g5g - 5g mode: 0x8
*/
