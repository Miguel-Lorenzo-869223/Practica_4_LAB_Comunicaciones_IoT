// ------------------------------------------------------------
//  Author: Miguel A.Lorenzo
//  Date: 21/10/2026
//  Subject: IoT Communications Laboratory 
//  Master: MSc in Electronic Engineering
//  University: University of Zaragoza EINA/UNIZAR
// ------------------------------------------------------------

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// UUID con "MIGUEL" en hexadecimal (4d 49 47 55 45 4c)
#define SERVICE_UUID "4d494755-454c-0000-0000-000000000001"

void setup() {
  Serial.begin(115200);

  // Inicializa el BLE con el nombre del dispositivo
  BLEDevice::init("MIGUEL_ESP32");

  // Crea el servidor BLE y añade el servicio con tu UUID
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Inicia el servicio
  pService->start();

  // Configura y activa el Advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  
  BLEDevice::startAdvertising();

  Serial.println("Advertising activo con UUID de MIGUEL.");
}

void loop() {
  delay(1000);
}