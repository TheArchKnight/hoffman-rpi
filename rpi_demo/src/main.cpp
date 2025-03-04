#include "huffman.h"
#include <hardware/gpio.h>
#include <pico/stdlib.h>
#include <hardware/uart.h>
#include <bt_sender.h>
#include <mpu6050.h>
#include <pico/stdio.h>
#include <vector>
#define UART_ID uart0 
#define LED_PIN 25
#define N_SAMPLES 100

void setUp(){
  stdio_init_all();
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
  gpio_put(LED_PIN, true);
  i2c_init(I2C_PORT, 400 * 1000);  // Ensure I2C_PORT is defined appropriately
  gpio_set_function(4, GPIO_FUNC_I2C);
  gpio_set_function(5, GPIO_FUNC_I2C);
  gpio_pull_up(4);
  gpio_pull_up(5);
}

// Pass objects by reference to avoid unnecessary copies.
int constructSamples(int n, MPU6050 &mpu, BluetoothSender &bt, Huffman &huffman) {
  std::vector<SensorData> data;
  for(int i = 0; i < n; i++){
    mpu.constructData();
    data.push_back(mpu.sensorData);
  }
  bt.sendData(huffman, data);
  return 0;
}

int main() {
  setUp();
  MPU6050 mpu;
  BluetoothSender bt(UART_ID);
  Huffman huffman;

  mpu.begin();

  // Collect initial data for Huffman tree building
  std::vector<int16_t> trainingData;
  for (int i = 0; i < 100; i++) {  // Use 100 samples to build the Huffman tree
    mpu.constructData();
    trainingData.insert(trainingData.end(), std::begin(mpu.sensorData.accel), std::end(mpu.sensorData.accel));
    trainingData.insert(trainingData.end(), std::begin(mpu.sensorData.gyro), std::end(mpu.sensorData.gyro));
  }
  // Build Huffman tree
  huffman.buildTree(trainingData);
  
  bool handShake = false;
  while(true) {
    // Wait for handshake until a '1' is received
    handShake = bt.receiveHandShake();
    if (handShake) {
      gpio_put(LED_PIN, false);
      constructSamples(N_SAMPLES, mpu, bt, huffman);
      // Reset handshake flag to wait for the next handshake
      handShake = false;
    }
  }
  return 0;
}

