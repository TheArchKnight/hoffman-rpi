#include "bt_sender.h"

BluetoothSender::BluetoothSender(uart_inst_t* uartPort) {
  uart_init(uartPort, BAUD_RATE);
  gpio_set_function(TX_PIN, GPIO_FUNC_UART);
  gpio_set_function(RX_PIN, GPIO_FUNC_UART);
  uart = uartPort;
}

int BluetoothSender::sendData(SensorData& data) {
  uint8_t buffer[12];
  std::memcpy(buffer, &data.accel[0], sizeof(int16_t) * 3);
  std::memcpy(buffer + 6, &data.gyro[0], sizeof(int16_t) * 3);

  for (size_t i = 0; i < sizeof(buffer); i++) {
    while (!uart_is_writable(uart)) {  
      sleep_us(10);
    }
    uart_putc(uart, buffer[i]);
  }
  return 0;
}
