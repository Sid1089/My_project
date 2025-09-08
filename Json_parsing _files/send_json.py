import asyncio
from bleak import BleakClient

# Your ESP32 details
ESP32_ADDRESS = "68:25:DD:32:2E:46"  # Found from scan
CHARACTERISTIC_UUID = "87654321-4321-4321-4321-ba0987654321"

# JSON file to send
JSON_FILE = "Frost_1.json"

async def send_json():
    with open(JSON_FILE, "r") as f:
        data = f.read()

    print(f"Loaded {len(data)} bytes from {JSON_FILE}")

    async with BleakClient(ESP32_ADDRESS) as client:
        print(f"Connected to {ESP32_ADDRESS}")

        # BLE packets max ~20 bytes → send in chunks
        CHUNK_SIZE = 20
        for i in range(0, len(data), CHUNK_SIZE):
            chunk = data[i:i+CHUNK_SIZE]
            await client.write_gatt_char(CHARACTERISTIC_UUID, chunk.encode())
            print(f"Sent chunk: {chunk}")

        # End marker
        await client.write_gatt_char(CHARACTERISTIC_UUID, "[END]".encode())
        print("All data sent.")

asyncio.run(send_json())
