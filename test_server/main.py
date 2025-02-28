import serial
import struct

# Configure serial port (adjust as needed)
serial_port = "/dev/rfcomm0"  # Your Bluetooth COM port
baud_rate = 9600

try:
    ser = serial.Serial(serial_port, baud_rate, timeout=1)
    print(f"Connected to {serial_port} at {baud_rate} baud")

    while True:
        # Read 12 bytes (each int16_t is 2 bytes, and we have 6 values)
        raw_data = ser.read(12)

        if len(raw_data) == 12:  # Ensure we received full packet
            accel = struct.unpack('<hhh', raw_data[:6])
            gyro = struct.unpack('<hhh', raw_data[6:])
            print(f"Accel: {accel}, Gyro: {gyro}")

except serial.SerialException as e:
    print(f"Error: {e}")
except KeyboardInterrupt:
    print("\nExiting...")

finally:
    if 'ser' in locals() and ser.is_open:
        ser.close()
