import serial
import time

# Set up serial connection
SERIAL_PORT = "/dev/rfcomm0"  # Adjust if needed
BAUD_RATE = 9600  # Must match HC-05 settings
TIMEOUT = 60  # Time in seconds to receive data


def receive_data(duration=TIMEOUT):
    """Receives data from the HC-05 for a given duration and analyzes it."""
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    received_messages = []
    start_time = time.time()

    print(f"Receiving data for {duration} seconds...")

    while time.time() - start_time < duration:
        data = ser.readline().decode().strip()  # Read a line
        if data:
            received_messages.append(data)  # Store received message

    ser.close()

    # Analysis
    total_messages = len(received_messages)
    total_bytes = sum(len(msg.encode()) for msg in received_messages)

    print("\n=== Data Analysis ===")
    print(f"Total messages received: {total_messages}")
    print(f"Total bytes received: {total_bytes} bytes")
    print(f"Average message size: {total_bytes / total_messages:.2f} bytes" if total_messages > 0 else "No data received.")

    # Save to file (optional)
    with open("received_data.txt", "w") as f:
        for msg in received_messages:
            f.write(msg + "\n")

    print("Data saved to received_data.txt")


receive_data()
