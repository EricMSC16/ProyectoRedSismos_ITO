#include <WiFi.h>
#include <esp_now.h>

typedef struct {
    float accel_x;
    float accel_y;
    float accel_z;
    float gyro_x;
    float gyro_y;
    float gyro_z;
    char timestamp[20];
} SensorData;

// Estructuras para almacenar datos de ambos maestros
SensorData sensorDataMaster1;
SensorData sensorDataMaster2;

// Direcciones MAC de los maestros
uint8_t masterAddress1[] = {0xB0, 0xA7, 0x32, 0x34, 0x55, 0xEC}; // Maestro 1
uint8_t masterAddress2[] = {0xE4, 0x65, 0xB8, 0x19, 0xC5, 0x68}; // Maestro 2

/*id onDataReceive(const uint8_t *mac_addr, const uint8_t *data, int len) {
    if (memcmp(mac_addr, masterAddress1, 6) == 0) {
        memcpy(&sensorDataMaster1, data, sizeof(sensorDataMaster1));
        Serial.print("Datos recibidos del Maestro 1: ");
        Serial.print(sensorDataMaster1.accel_x);
        Serial.print(", ");
        Serial.print(sensorDataMaster1.accel_y);
        Serial.print(", ");
        Serial.println(sensorDataMaster1.accel_z);
    } else if (memcmp(mac_addr, masterAddress2, 6) == 0) {
        memcpy(&sensorDataMaster2, data, sizeof(sensorDataMaster2));
        Serial.print("Datos recibidos del Maestro 2: ");
        Serial.print(sensorDataMaster2.accel_x);
        Serial.print(", ");
        Serial.print(sensorDataMaster2.accel_y);
        Serial.print(", ");
        Serial.println(sensorDataMaster2.accel_z);
    } else {
        Serial.println("Dirección MAC no coincide.");
    }
}
*/
void onDataReceive(const uint8_t *mac_addr, const uint8_t *data, int len) {
    if (memcmp(mac_addr, masterAddress1, 6) == 0) {
        memcpy(&sensorDataMaster1, data, sizeof(sensorDataMaster1));
        Serial.print("Datos recibidos del Maestro 1: ");
        Serial.print(sensorDataMaster1.timestamp);
        Serial.print(",");
        Serial.print(sensorDataMaster1.accel_x);
        Serial.print(",");
        Serial.print(sensorDataMaster1.accel_y);
        Serial.print(",");
        Serial.print(sensorDataMaster1.accel_z);
        Serial.print(",");
        Serial.print(sensorDataMaster1.gyro_x);
        Serial.print(",");
        Serial.print(sensorDataMaster1.gyro_y);
        Serial.print(",");
        Serial.println(sensorDataMaster1.gyro_z);
    } else if (memcmp(mac_addr, masterAddress2, 6) == 0) {
        memcpy(&sensorDataMaster2, data, sizeof(sensorDataMaster2));
        Serial.print("Datos recibidos del Maestro 2:");
        Serial.print(sensorDataMaster2.timestamp);
        Serial.print(",");
        Serial.print(sensorDataMaster2.accel_x);
        Serial.print(",");
        Serial.print(sensorDataMaster2.accel_y);
        Serial.print(",");
        Serial.print(sensorDataMaster2.accel_z);
        Serial.print(",");
        Serial.print(sensorDataMaster2.gyro_x);
        Serial.print(",");
        Serial.print(sensorDataMaster2.gyro_y);
        Serial.print(",");
        Serial.println(sensorDataMaster2.gyro_z);
    } else {
        Serial.println("Dirección MAC no coincide.");
    }
}

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA); // Modo estación

    if (esp_now_init() != ESP_OK) {
        Serial.println("Error inicializando ESP-NOW");
        return;
    }

    esp_now_register_recv_cb(onDataReceive);

    // Agregar el primer maestro como peer
    esp_now_peer_info_t peerInfo1;
    memset(&peerInfo1, 0, sizeof(peerInfo1));
    memcpy(peerInfo1.peer_addr, masterAddress1, 6);
    peerInfo1.channel = 0; // Permitir que el ESP32 elija el canal automáticamente
    peerInfo1.encrypt = false;

    if (esp_now_add_peer(&peerInfo1) != ESP_OK) {
        Serial.println("Error al agregar el primer peer");
    }

    // Agregar el segundo maestro como peer
    esp_now_peer_info_t peerInfo2;
    memset(&peerInfo2, 0, sizeof(peerInfo2));
    memcpy(peerInfo2.peer_addr, masterAddress2, 6);
    peerInfo2.channel = 0; // Permitir que el ESP32 elija el canal automáticamente
    peerInfo2.encrypt = false;

    if (esp_now_add_peer(&peerInfo2) != ESP_OK) {
        Serial.println("Error al agregar el segundo peer");
    }

    Serial.println("Esclavo listo para recibir datos.");
}

void loop() {
    delay(100);
}
