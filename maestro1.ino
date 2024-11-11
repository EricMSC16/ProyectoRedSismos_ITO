#include <WiFi.h>
#include <esp_now.h>
#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

// Variables de calibración
int16_t ax_offset = 0, ay_offset = 0, az_offset = 0;
int16_t gx_offset = 0, gy_offset = 0, gz_offset = 0;

typedef struct {
    float accel_x;
    float accel_y;
    float accel_z;
    float gyro_x;
    float gyro_y;
    float gyro_z;
} SensorData;

uint8_t slaveAddress[] = {0xB0, 0xA7, 0x32, 0x34, 0x69, 0xB0}; // Dirección del esclavo

void setup() {
    Serial.begin(115200);
    Serial.println("Iniciando...");

    // Inicializar el MPU6050
    Wire.begin();
    mpu.initialize();
    if (!mpu.testConnection()) {
        Serial.println("Error al inicializar MPU6050");
        while (1);
    }

    // Calibración del sensor
    for (int i = 0; i < 1000; i++) {
        int16_t ax, ay, az, gx, gy, gz;
        mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
        
        ax_offset += ax;
        ay_offset += ay;
        az_offset += az;
        gx_offset += gx;
        gy_offset += gy;
        gz_offset += gz;

        delay(3);
    }
    ax_offset /= 1000;
    ay_offset /= 1000;
    az_offset /= 1000;
    gx_offset /= 1000;
    gy_offset /= 1000;
    gz_offset /= 1000;

    // Inicializar ESP-NOW
    WiFi.mode(WIFI_STA); // Modo estación para ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error al inicializar ESP-NOW");
        return;
    }

    // Configurar el peer para el esclavo
    esp_now_peer_info_t peerInfo;
    memset(&peerInfo, 0, sizeof(peerInfo)); // Limpia la estructura
    memcpy(peerInfo.peer_addr, slaveAddress, 6);
    peerInfo.channel = 0;  // Permitir que el ESP32 elija el canal automáticamente
    peerInfo.encrypt = false; // Sin cifrado

    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Error al agregar el peer");
    } else {
        Serial.println("Peer agregado correctamente");
    }
}

void loop() {
    static unsigned long lastSampleTime = 0; // Tiempo de la última muestra
    unsigned long currentTime = millis();     // Tiempo actual

    const unsigned long samplePeriod = 100; // Periodo de muestreo

    if (currentTime - lastSampleTime >= samplePeriod) {
        lastSampleTime = currentTime; // Actualiza el tiempo de la última muestra

        int16_t ax, ay, az, gx, gy, gz; 
        mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz); 

        SensorData data;
        data.accel_x = (float)(ax - ax_offset) / 16384.0;
        data.accel_y = (float)(ay - ay_offset) / 16384.0;
        data.accel_z = (float)(az - az_offset) / 16384.0;
        data.gyro_x = (float)(gx - gx_offset) / 131.0;
        data.gyro_y = (float)(gy - gy_offset) / 131.0;
        data.gyro_z = (float)(gz - gz_offset) / 131.0;

        // Imprimir los datos del sensor
        Serial.print("Aceleración: ");
        Serial.print(data.accel_x);
        Serial.print(", ");
        Serial.print(data.accel_y);
        Serial.print(", ");
        Serial.println(data.accel_z);
        
        Serial.print("Giroscopio: ");
        Serial.print(data.gyro_x);
        Serial.print(", ");
        Serial.print(data.gyro_y);
        Serial.print(", ");
        Serial.println(data.gyro_z);

        esp_err_t result = esp_now_send(slaveAddress, (uint8_t *)&data, sizeof(data));
if (result != ESP_OK) {
    Serial.print("Error al enviar datos: ");
    Serial.println(result);
} else {
    Serial.println("Datos enviados al esclavo.");
}
    }

    delay(1); // Pequeña pausa para no saturar el bucle
}
