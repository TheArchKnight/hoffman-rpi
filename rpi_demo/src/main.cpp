#define UART_ID uart0 
#define LED_PIN 25

#include <hardware/gpio.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <pico/stdlib.h>
#include <hardware/uart.h>
#include <bt_sender.h>
#include <mpu6050.h>

MPU6050 mpu;
BluetoothSender bt(UART_ID);

void setUp(){
  mpu.begin();
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
  gpio_put(LED_PIN, 1);
  gpio_set_function(4, GPIO_FUNC_I2C);
  gpio_set_function(5, GPIO_FUNC_I2C);
  gpio_pull_up(4);
  gpio_pull_up(5);
}

int main(){
  setUp();
  mpu.constructData();

}
