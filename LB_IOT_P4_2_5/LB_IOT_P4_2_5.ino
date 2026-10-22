// ------------------------------------------------------------
//  Author: Miguel A.Lorenzo
//  Date: 21/10/2026
//  Subject: IoT Communications Laboratory 
//  Master: MSc in Electronic Engineering
//  University: University of Zaragoza EINA/UNIZAR
// ------------------------------------------------------------


#include "BluetoothSerial.h"

// Comprobación de seguridad de configuración Bluetooth
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth no está habilitado en las opciones de la placa ESP32.
#endif

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);

  // Inicia el Bluetooth Classic con el nombre del dispositivo
  SerialBT.begin("ESP32_Chat_SPP");

  Serial.println("ESP32 listo. Empareja el móvil y abre la APP de terminal.");
}

void loop() {
  // 1. De la PC (Monitor Serie) -> Al Móvil por Bluetooth
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }

  // 2. Del Móvil (Bluetooth) -> A la PC (Monitor Serie)
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
}