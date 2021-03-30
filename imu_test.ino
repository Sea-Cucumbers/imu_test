#include <Adafruit_LSM6DS33.h>

Adafruit_LSM6DS33 lsm6ds33;

float yaw = 0;
float bias = 0;

void setup() {  
  Serial.begin(9600);  // start serial for output

  if (!lsm6ds33.begin_I2C()) {
    Serial.println("Failed to find LSM6DS33 chip");
    while (1) {
      delay(10);
    }
  }

  Serial.println("LSM6DS33 Found!");
 
  lsm6ds33.setAccelRange(LSM6DS_ACCEL_RANGE_16_G);
  lsm6ds33.setGyroRange(LSM6DS_GYRO_RANGE_125_DPS);
  lsm6ds33.setAccelDataRate(LSM6DS_RATE_12_5_HZ);
  lsm6ds33.setGyroDataRate(LSM6DS_RATE_12_5_HZ);

  // First few IMU readings are garbage
  for (int i = 0; i < 10; ++i) {
    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t temp;
    lsm6ds33.getEvent(&accel, &gyro, &temp);
    delay(100);
  }

  // Calibrate bias
  bias = 0;
  for (int i = 0; i < 10; ++i) {
    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t temp;
    lsm6ds33.getEvent(&accel, &gyro, &temp);
  
    bias += gyro.gyro.z;
    delay(100);
  }

  bias /= 10;
}

void loop() {
  // Get IMU

  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  lsm6ds33.getEvent(&accel, &gyro, &temp);

  yaw += (gyro.gyro.z - bias)*0.1;
  Serial.println(yaw);
  
  delay(100);
}
