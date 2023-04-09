#include "BluetoothSerial.h"
#include "esp_bt_device.h"
#include <VL53L1X.h>
#include <Wire.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
VL53L1X sensorL;

void printDeviceAddress() {
 
  const uint8_t* point = esp_bt_dev_get_address();
 
  for (int i = 0; i < 6; i++) {
 
    char str[3];
 
    sprintf(str, "%02X", (int)point[i]);
    Serial.print(str);
 
    if (i < 5){
      Serial.print(":");
    }
 
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("\n---Start---");
  SerialBT.begin("ESP32test2"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  Serial.println("Device Name: ESP32test2");
  Serial.print("BT MAC: ");
  printDeviceAddress();
  Serial.println();
  Wire.begin();
  Wire.setClock(400000);
  sensorL.setTimeout(500);
  if (!sensorL.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1);
  }
  sensorL.setDistanceMode(VL53L1X::Short);
  sensorL.setMeasurementTimingBudget(20000);
  sensorL.startContinuous(20);
  sensorL.setROISize(4,4);
}

void loop() {
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    //Serial.write(SerialBT.read());
    //char pattern = SerialBT.read();
    //Serial.println(pattern);
    while (sensorL.read() > 100) {
        String b = "0";
        uint8_t buf[b.length()];
        memcpy(buf,b.c_str(),b.length());
        SerialBT.write(buf,b.length());
        delay(100);
        continue;
      }
    if (sensorL.read() <= 50) {
        String a = "L";
        uint8_t buf[a.length()];
        memcpy(buf,a.c_str(),a.length());
        SerialBT.write(buf,a.length());
        delay(400);
      }
  }
  delay(20);
}