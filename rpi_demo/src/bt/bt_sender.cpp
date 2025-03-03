#include "bt_sender.h"
#include <hardware/uart.h>
BluetoothSender::BluetoothSender(uart_inst_t* uartPort) {
  uart_init(uartPort, BAUD_RATE);
  gpio_set_function(TX_PIN, GPIO_FUNC_UART);
  gpio_set_function(RX_PIN, GPIO_FUNC_UART);
  uart = uartPort;
}

int BluetoothSender::sendData(SensorData& data) {
  uint8_t buffer[sizeof(SensorData)]; 
  std::memcpy(buffer, data.accel, sizeof(data.accel));
  std::memcpy(buffer + sizeof(data.accel), data.gyro, sizeof(data.gyro));
  for (size_t i = 0; i < sizeof(buffer); i++) {
    uart_putc(uart, buffer[i]);
  }
  return 0;
}
