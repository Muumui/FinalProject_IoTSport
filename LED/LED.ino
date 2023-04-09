#include "BluetoothSerial.h"
#include "esp_bt_device.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
int LED_L = 18;
int LED_R = 19;

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
  // put your setup code here, to run once:
  pinMode(LED_L, OUTPUT);
  pinMode(LED_R, OUTPUT);
  Serial.begin(115200);
  Serial.println("\n---Start---");
  SerialBT.begin("ESP32test3"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  Serial.println("Device Name: ESP32test3");
  Serial.print("BT MAC: ");
  printDeviceAddress();
  Serial.println();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    //Serial.write(SerialBT.read());
    char pattern = SerialBT.read();
    Serial.println(pattern);
    if (pattern == '>'){
      digitalWrite(LED_R, HIGH); 
    }
    if (pattern == '<'){
      digitalWrite(LED_L, HIGH);
    }
    if (pattern == '='){
      digitalWrite(LED_L, LOW);
      digitalWrite(LED_R, LOW);
    }
  }
  delay(20);
}
