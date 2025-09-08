import asyncio
from bleak import BleakScanner, BleakClient

# Step 1: Scan for BLE devices
async def scan_devices():
    print("Scanning for BLE devices...")
    devices = await BleakScanner.discover(timeout=8.0)
    for i, d in enumerate(devices):
        print(f"[{i}] Name: {d.name}, Address: {d.address}, RSSI: {d.rssi}")
    return devices

# Step 2: Connect and list services/characteristics
async def explore_device(address):
    async with BleakClient(address) as client:
        print(f"Connected to {address}")
        services = await client.get_services()
        for service in services:
            print(f"\nService: {service.uuid}")
            for char in service.characteristics:
                props = ",".join(char.properties)
                print(f"   Characteristic: {char.uuid} (Properties: {props})")

# Step 3: Main function
async def main():
    devices = await scan_devices()
    if not devices:
        print("No BLE devices found.")
        return

    # Pick first device (you can change the index)
    target = devices[0].address
    print(f"\nConnecting to {target} ...")
    await explore_device(target)

if __name__ == "__main__":
    asyncio.run(main())
