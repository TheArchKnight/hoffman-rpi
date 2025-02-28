#include "mpu6050.h"
void MPU6050::begin()
{
  uint8_t val[] = {MPU6050_POWER, 0x00};
  i2c_write_blocking(I2C_PORT, MPU6050_ADDRESS, val, 2, false);
  sleep_ms(100);  // Wait for sensor to stabilize
}

void MPU6050::configure(int16_t *bias)
{
  /* 
    * TODO: make a bias calibration function/program
    * for getting the zero error of the sensor
    */

  uint8_t biasRegs[6] = {MPU6050_OFFSET_AX, MPU6050_OFFSET_AY, MPU6050_OFFSET_AZ,
    MPU6050_OFFSET_GX, MPU6050_OFFSET_GY, MPU6050_OFFSET_GZ};

  for(int i = 0; i < 6; i++)
  {
    uint8_t val[] = {biasRegs[i], (uint8_t)(bias[i] >> 8), (uint8_t)(bias[i])};
    i2c_write_blocking(I2C_PORT, MPU6050_ADDRESS, val, 3, false);
  }

}
void MPU6050::readAccel()
{
  uint8_t data[6];
  uint8_t val = MPU6050_ACCEL_X;
  i2c_write_blocking(I2C_PORT, MPU6050_ADDRESS, &val, 1, true);
  i2c_read_blocking(I2C_PORT, MPU6050_ADDRESS, data, 6, false);
  for(int i = 0; i < 3; i++)
  {
    this->sensorData.accel[i] = (data[i*2] << 8 | data[(i*2) + 1]);
  }
}

void MPU6050::readGyro()
{
  uint8_t data[6];
  uint8_t val = MPU6050_GYRO_X;
  i2c_write_blocking(I2C_PORT, MPU6050_ADDRESS, &val, 1, true);
  i2c_read_blocking(I2C_PORT, MPU6050_ADDRESS, data, 6, false);
  for(int i = 0; i < 3; i++)
  {
    this->sensorData.gyro[i] = (data[i*2] << 8 | data[(i*2) + 1]);
  }
}

void MPU6050::constructData(){
  readAccel();
  readGyro();
}
