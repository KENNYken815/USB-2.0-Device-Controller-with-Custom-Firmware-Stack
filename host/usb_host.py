"""Host utility for a vendor-specific USB bulk interface (VID/PID configurable)."""
import argparse, struct, zlib, sys
MAGIC=0x55425331
VERSION=1
HEADER=12
MAX_PAYLOAD=256
CMD={"ping":1,"info":2,"echo":3,"counters":4,"reset":5}
def encode(cmd,seq,payload=b""):
    if len(payload)>MAX_PAYLOAD: raise ValueError("payload too large")
    header=struct.pack("<IBBHI",MAGIC,VERSION,cmd,len(payload),seq)
    crc=(zlib.crc32(header[:8]) ^ zlib.crc32(payload)) & 0xffffffff
    return header[:8]+struct.pack("<I",crc)+payload
def decode(packet):
    if len(packet)<HEADER: raise ValueError("short packet")
    magic,ver,cmd,n,seq=struct.unpack("<IBBHI",packet[:12])
    if magic!=MAGIC or ver!=VERSION or n>MAX_PAYLOAD or len(packet)!=HEADER+n: raise ValueError("invalid header/length")
    crc=(zlib.crc32(packet[:8]+b"\0\0\0\0") ^ zlib.crc32(packet[HEADER:])) & 0xffffffff
    if crc != struct.unpack("<I",packet[8:12])[0]: raise ValueError("CRC mismatch")
    return cmd,seq,packet[HEADER:]
def main():
    ap=argparse.ArgumentParser(); ap.add_argument("--vid",type=lambda x:int(x,0),default=0x1209); ap.add_argument("--pid",type=lambda x:int(x,0),default=0x0001); ap.add_argument("--command",choices=CMD,default="ping"); ap.add_argument("--text",default="hello-device"); ap.add_argument("--timeout",type=int,default=1000); args=ap.parse_args()
    try:
        import usb.core, usb.util
    except ImportError:
        print("Install PyUSB with: python -m pip install pyusb",file=sys.stderr); return 2
    dev=usb.core.find(idVendor=args.vid,idProduct=args.pid)
    if dev is None: print("USB device not found; verify VID/PID and OS permissions",file=sys.stderr); return 2
    try:
        dev.set_configuration(); cfg=dev.get_active_configuration(); intf=cfg[(0,0)]
        out_ep=usb.util.find_descriptor(intf,custom_match=lambda e: usb.util.endpoint_direction(e.bEndpointAddress)==usb.util.ENDPOINT_OUT)
        in_ep=usb.util.find_descriptor(intf,custom_match=lambda e: usb.util.endpoint_direction(e.bEndpointAddress)==usb.util.ENDPOINT_IN)
        if out_ep is None or in_ep is None: raise RuntimeError("bulk endpoints not found")
        payload=args.text.encode() if args.command=="echo" else b""
        req=encode(CMD[args.command],1,payload); out_ep.write(req,timeout=args.timeout); resp=bytes(in_ep.read(HEADER+MAX_PAYLOAD,timeout=args.timeout)); cmd,seq,data=decode(resp)
        print(f"response_command=0x{cmd:02X} sequence={seq} payload={data!r}")
    except Exception as exc:
        print(f"USB transaction failed: {exc}",file=sys.stderr); return 1
    finally: usb.util.dispose_resources(dev)
    return 0
if __name__=="__main__": raise SystemExit(main())
