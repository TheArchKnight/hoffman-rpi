#include "bt_sender.h"
#include "mpu6050.h"
#include <hardware/uart.h>
#include <pico/stdlib.h>
#include <vector>

#define TX_PIN 0          // Change these according to your wiring
#define RX_PIN 1

BluetoothSender::BluetoothSender(uart_inst_t* uartPort) {
  uart_init(uartPort, BAUD_RATE);
  gpio_set_function(TX_PIN, GPIO_FUNC_UART);
  gpio_set_function(RX_PIN, GPIO_FUNC_UART);
  uart_set_hw_flow(uartPort, false, false); // No hardware flow control
  uart_set_format(uartPort, 8, 1, UART_PARITY_NONE);

  uart = uartPort;
}

// Wait for handshake signal ('1')
bool BluetoothSender::receiveHandShake() {
  while (!uart_is_readable(uart)) {
    sleep_ms(10);  // Avoid busy waiting
  }

  char received = uart_getc(uart);
  return (received == '1');
}

// Send compressed data over UART
int BluetoothSender::sendData(Huffman& huffman, std::vector<SensorData>& data) {
  std::vector<int16_t> flattenedData;
  for(int i = 0; i < data.size(); i ++){
    flattenedData.insert(flattenedData.end(), std::begin(data[i].accel), std::end(data[i].accel));
    flattenedData.insert(flattenedData.end(), std::begin(data[i].gyro), std::end(data[i].gyro));
  }
  // Encode with Huffman
  std::vector<uint16_t> compressedData = huffman.encode(flattenedData);

  // Send data over UART
  for (uint16_t value : compressedData) {
    uart_putc(uart, value & 0xFF);       // Send lower byte
    uart_putc(uart, (value >> 8) & 0xFF); // Send upper byte
  }

  // Send "END" marker to signal the end of transmission
  uart_putc(uart, 'E');
  uart_putc(uart, 'N');
  uart_putc(uart, 'D');

  return 0;
}

