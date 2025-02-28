#ifndef BLUETOOTHSENDER_H
#define BLUETOOTHSENDER_H
#define BAUD_RATE 9600
#define TX_PIN 0
#define RX_PIN 1

#include <hardware/gpio.h>
#include <cstring>
#include <hardware/structs/io_bank0.h>
#include <hardware/uart.h>
#include <pico/time.h>
#include "hardware/uart.h"
#include "mpu6050.h"

class BluetoothSender {
private:
    uart_inst_t* uart;

public:
    BluetoothSender(uart_inst_t* uartPort);
    int sendData(SensorData& data);
    int testConnection();
};

#endif
