import serial
import time

def main():
    port = "/dev/rfcomm0"
    baudrate = 9600
    timeout = 1  # seconds

    try:
        ser = serial.Serial(port, baudrate, timeout=timeout)
    except Exception as e:
        print("Error opening serial port {}: {}".format(port, e))
        return

    if ser.is_open:
        print("Serial port {} opened successfully.".format(port))
    else:
        print("Failed to open serial port {}.".format(port))
        return

    while True:
        # Flush any old data from the input buffer.
        ser.reset_input_buffer()

        # Send handshake '1'
        handshake = b"1"
        ser.write(handshake)
        print("Sent handshake '1'.")

        # Read incoming data until "END" is received
        buffer = b""
        while True:
            chunk = ser.read(64)  # read up to 64 bytes at a time
            if chunk:
                buffer += chunk
                if b"END" in buffer:
                    break

        # Remove the marker "END" from the received data
        marker_index = buffer.find(b"END")
        received_data = buffer[:marker_index]

        print("Received data size: {} bytes".format(len(received_data)))
        # Optional delay before processing the next package
        time.sleep(1)

    ser.close()
if __name__ == "__main__":
    main()
