import asyncio
from bleak import BleakScanner

async def main():
    print("Scanning for BLE devices...")
    devices = await BleakScanner.discover(timeout=5.0)
    for i, d in enumerate(devices):
        # Try different ways to get RSSI depending on Bleak version
        rssi = getattr(d, "rssi", None)
        if rssi is None and hasattr(d, "metadata"):
            rssi = d.metadata.get("rssi", "N/A")
        if rssi is None:
            rssi = "N/A"

        print(f"[{i}] {d.name or 'Unknown'} | {d.address} | RSSI: {rssi}")

asyncio.run(main())
