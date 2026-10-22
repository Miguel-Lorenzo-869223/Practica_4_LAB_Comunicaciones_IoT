// ------------------------------------------------------------
//  Author: Miguel A.Lorenzo
//  Date: 21/10/2026
//  Subject: IoT Communications Laboratory 
//  Master: MSc in Electronic Engineering
//  University: University of Zaragoza EINA/UNIZAR
// ------------------------------------------------------------

#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h> // Para Nano 33 BLE Rev1 (Sustituir por Arduino_LSM6DS3.h si usas Rev2)

// Definición del Servicio
BLEService accelService("19B10010-E008-4E6F-9A37-7D23D95F3698");

// Característica 1: Lectura y Notificaciones de los ejes [X, Y, Z] (12 bytes)
BLECharacteristic accelDataChar("19B10011-E008-4E6F-9A37-7D23D95F3698", BLERead | BLENotify, 12);

// Característica 2: Control de activación - Solo Escritura (BLEWrite)
BLEByteCharacteristic enableChar("19B10012-E008-4E6F-9A37-7D23D95F3698", BLEWrite);

float accelData[3] = {0.0f, 0.0f, 0.0f};
bool isAccelActive = true; // Variable de estado (activado por defecto)

void setup() {
  Serial.begin(115200);

  if (!IMU.begin()) {
    Serial.println("Error al inicializar el acelerómetro IMU.");
    while (1);
  }

  if (!BLE.begin()) {
    Serial.println("Error al inicializar BLE.");
    while (1);
  }

  BLE.setLocalName("Nano33_Acelerometro");
  BLE.setAdvertisedService(accelService);

  // Añadir ambas características al árbol GATT
  accelService.addCharacteristic(accelDataChar);
  accelService.addCharacteristic(enableChar);
  BLE.addService(accelService);

  // Valores iniciales
  accelDataChar.writeValue((byte*)accelData, sizeof(accelData));

  BLE.advertise();
  Serial.println("Servicio GATT iniciado. Esperando conexión...");
}

void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Central conectada: ");
    Serial.println(central.address());

    while (central.connected()) {
      // 1. Verificar si el cliente escribió en la característica de control
      if (enableChar.written()) {
        uint8_t comando = enableChar.value();
        if (comando == 1) {
          isAccelActive = true;
          Serial.println("Acelerómetro ACTIVADO por la Central.");
        } else if (comando == 0) {
          isAccelActive = false;
          Serial.println("Acelerómetro DESACTIVADO por la Central.");
        }
      }

      // 2. Solo leer el sensor y notificar si el acelerómetro está activo
      if (isAccelActive && IMU.accelerationAvailable()) {
        IMU.readAcceleration(accelData[0], accelData[1], accelData[2]);

        // Envía notificación con los nuevos datos
        accelDataChar.writeValue((byte*)accelData, sizeof(accelData));

        Serial.print("Acc X: "); Serial.print(accelData[0]);
        Serial.print(" | Y: "); Serial.print(accelData[1]);
        Serial.print(" | Z: "); Serial.println(accelData[2]);
      }

      delay(100);
    }

    Serial.println("Central desconectada.");
  }
}