"""Host utility for the vendor-specific USB bulk protocol."""
import argparse
import struct
import zlib
import sys

MAGIC = 0x55425331
VERSION = 1
HEADER = 16
MAX_PAYLOAD = 256
CMD = {"ping": 1, "info": 2, "echo": 3, "counters": 4, "reset": 5}

def packet_crc(packet):
    data = bytearray(packet)
    data[12:16] = b"\x00\x00\x00\x00"
    return zlib.crc32(data) & 0xFFFFFFFF

def encode(cmd, seq, payload=b""):
    if len(payload) > MAX_PAYLOAD:
        raise ValueError("payload too large")
    packet = struct.pack("<IBBHI", MAGIC, VERSION, cmd, len(payload), seq) + b"\x00\x00\x00\x00" + payload
    crc = packet_crc(packet)
    return packet[:12] + struct.pack("<I", crc) + packet[16:]

def decode(packet):
    if len(packet) < HEADER:
        raise ValueError("short packet")
    magic, ver, cmd, n, seq = struct.unpack("<IBBHI", packet[:12])
    if magic != MAGIC or ver != VERSION or n > MAX_PAYLOAD or len(packet) != HEADER + n:
        raise ValueError("invalid header/length")
    expected = struct.unpack("<I", packet[12:16])[0]
    if packet_crc(packet) != expected:
        raise ValueError("CRC mismatch")
    return cmd, seq, packet[HEADER:]

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--vid", type=lambda x: int(x, 0), default=0x1209)
    ap.add_argument("--pid", type=lambda x: int(x, 0), default=0x0001)
    ap.add_argument("--command", choices=CMD, default="ping")
    ap.add_argument("--text", default="hello-device")
    ap.add_argument("--timeout", type=int, default=1000)
    args = ap.parse_args()
    try:
        import usb.core
        import usb.util
    except ImportError:
        print("Install PyUSB: python -m pip install pyusb", file=sys.stderr)
        return 2
    dev = usb.core.find(idVendor=args.vid, idProduct=args.pid)
    if dev is None:
        print("USB device not found; verify VID/PID and OS permissions", file=sys.stderr)
        return 2
    try:
        dev.set_configuration()
        intf = dev.get_active_configuration()[(0, 0)]
        out_ep = usb.util.find_descriptor(intf, custom_match=lambda e: usb.util.endpoint_direction(e.bEndpointAddress) == usb.util.ENDPOINT_OUT)
        in_ep = usb.util.find_descriptor(intf, custom_match=lambda e: usb.util.endpoint_direction(e.bEndpointAddress) == usb.util.ENDPOINT_IN)
        if out_ep is None or in_ep is None:
            raise RuntimeError("bulk endpoints not found")
        payload = args.text.encode() if args.command == "echo" else b""
        out_ep.write(encode(CMD[args.command], 1, payload), timeout=args.timeout)
        response = bytes(in_ep.read(HEADER + MAX_PAYLOAD, timeout=args.timeout))
        command, sequence, data = decode(response)
        print(f"response_command=0x{command:02X} sequence={sequence} payload={data!r}")
    except Exception as exc:
        print(f"USB transaction failed: {exc}", file=sys.stderr)
        return 1
    finally:
        usb.util.dispose_resources(dev)
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
