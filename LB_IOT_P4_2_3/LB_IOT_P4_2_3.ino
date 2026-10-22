// ------------------------------------------------------------
//  Author: Miguel A.Lorenzo
//  Date: 21/10/2026
//  Subject: IoT Communications Laboratory 
//  Master: MSc in Electronic Engineering
//  University: University of Zaragoza EINA/UNIZAR
// ------------------------------------------------------------


#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h> // Librería del acelerómetro integrado (Para Nano 33 BLE Rev1)
// NOTA: Si usas una Arduino Nano 33 BLE Rev2, sustituye la librería por: #include <Arduino_LSM6DS3.h>

// Definición de UUIDs para el Servicio y la Característica del Acelerómetro
BLEService accelService("19B10010-E008-4E6F-9A37-7D23D95F3698");

// Característica de 12 bytes (3 variables float x 4 bytes cada una: X, Y, Z)
// Permisos solicitados: Solo Lectura (BLERead) y Notificaciones (BLENotify)
BLECharacteristic accelDataChar("19B10011-E008-4E6F-9A37-7D23D95F3698", BLERead | BLENotify, 12);

// Array para almacenar temporalmente los ejes [X, Y, Z]
float accelData[3] = {0.0f, 0.0f, 0.0f};

void setup() {
  Serial.begin(115200);

  // 1. Inicializar el sensor de movimiento (IMU)
  if (!IMU.begin()) {
    Serial.println("Error al inicializar el acelerómetro IMU.");
    while (1);
  }

  // 2. Inicializar el módulo BLE
  if (!BLE.begin()) {
    Serial.println("Error al inicializar el módulo BLE.");
    while (1);
  }

  // Configuración de la publicidad (Advertising)
  BLE.setLocalName("Nano33_Acelerometro");
  BLE.setAdvertisedService(accelService);

  // Configuración del árbol GATT
  accelService.addCharacteristic(accelDataChar);
  BLE.addService(accelService);

  // Valor inicial (12 bytes a cero)
  accelDataChar.writeValue((byte*)accelData, sizeof(accelData));

  // Iniciar la transmisión
  BLE.advertise();
  Serial.println("Periférico GATT activo. Esperando conexión del Central...");
}

void loop() {
  // Esperar a que un dispositivo central (móvil o PC) se conecte
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Dispositivo conectado: ");
    Serial.println(central.address());

    while (central.connected()) {
      // Comprobar si el acelerómetro tiene lecturas nuevas disponibles
      if (IMU.accelerationAvailable()) {
        // Leer las aceleraciones en Gs (X, Y, Z)
        IMU.readAcceleration(accelData[0], accelData[1], accelData[2]);

        // Actualizar el valor en el atributo GATT.
        // Al usar 'writeValue', la librería emite automáticamente una NOTIFICACIÓN 
        // a la app central si esta se ha suscrito a los avisos.
        accelDataChar.writeValue((byte*)accelData, sizeof(accelData));

        // Muestra de datos por puerto serie para depuración
        Serial.print("Acc X: "); Serial.print(accelData[0]);
        Serial.print(" | Y: "); Serial.print(accelData[1]);
        Serial.print(" | Z: "); Serial.println(accelData[2]);
      }

      // Tasa de refresco ajustable (100 ms = 10Hz)
      delay(100);
    }

    Serial.println("Dispositivo central desconectado.");
  }
}