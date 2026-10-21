// ------------------------------------------------------------
//  Author: Miguel A.Lorenzo
//  Date: 21/10/2026
//  Subject: IoT Communications Laboratory 
//  Master: MSc in Electronic Engineering
//  University: University of Zaragoza EINA/UNIZAR
// ------------------------------------------------------------


#include <ArduinoBLE.h>

// Definición de UUIDs para el Servicio GATT y la Característica de Control
BLEService ledService("19B10000-E008-4E6F-9A37-7D23D95F3698");
BLEByteCharacteristic ledColorCharacteristic("19B10001-E008-4E6F-9A37-7D23D95F3698", BLERead | BLEWrite);

// El LED RGB de la Arduino Nano 33 BLE funciona con lógica invertida (Active LOW):
// LOW = LED Encendido | HIGH = LED Apagado
void setColor(bool red, bool green, bool blue) {
  digitalWrite(LEDR, red ? LOW : HIGH);
  digitalWrite(LEDG, green ? LOW : HIGH);
  digitalWrite(LEDB, blue ? LOW : HIGH);
}

void setup() {
  Serial.begin(115200);

  // Configuración de los pines del LED RGB integrado
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);

  // Estado inicial: LEDs apagados
  setColor(false, false, false);

  // Inicialización del módulo BLE nativo de la Nano 33 BLE (nRF52840)
  if (!BLE.begin()) {
    Serial.println("Error al inicializar BLE en Arduino Nano 33 BLE");
    while (1);
  }

  // Nombre local para el Advertising
  BLE.setLocalName("Nano33_RGB_GATT");
  BLE.setAdvertisedService(ledService);

  // Estructura GATT: añade la característica al servicio
  ledService.addCharacteristic(ledColorCharacteristic);
  BLE.addService(ledService);

  // Valor por defecto (0 = Apagado)
  ledColorCharacteristic.writeValue(0);

  // Iniciar publicidad GATT
  BLE.advertise();
  Serial.println("Dispositivo Periférico GATT activo y esperando conexión...");
}

void loop() {
  // Esperar a que un dispositivo Central (móvil, PC) se conecte
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Conectado a Central: ");
    Serial.println(central.address());

    // Mantener la ejecución mientras dure la conexión
    while (central.connected()) {
      // Verificar si la Central ha escrito un nuevo valor en la característica
      if (ledColorCharacteristic.written()) {
        uint8_t valor = ledColorCharacteristic.value();
        Serial.print("Comando recibido: ");
        Serial.println(valor);

        switch (valor) {
          case 0: // Apagado
            setColor(false, false, false);
            break;
          case 1: // Rojo
            setColor(true, false, false);
            break;
          case 2: // Verde
            setColor(false, true, false);
            break;
          case 3: // Azul
            setColor(false, false, true);
            break;
          case 4: // Blanco (Rojo + Verde + Azul)
            setColor(true, true, true);
            break;
          case 5: // Amarillo (Rojo + Verde)
            setColor(true, true, false);
            break;
          case 6: // Cian (Verde + Azul)
            setColor(false, true, true);
            break;
          case 7: // Magenta (Rojo + Azul)
            setColor(true, false, true);
            break;
          default:
            setColor(false, false, false);
            break;
        }
      }
    }

    // Al desconectarse, apagar los LEDs por seguridad
    setColor(false, false, false);
    Serial.println("Dispositivo Central desconectado.");
  }
}