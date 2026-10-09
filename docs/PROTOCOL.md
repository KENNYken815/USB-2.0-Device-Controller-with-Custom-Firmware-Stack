# Custom Binary Protocol

The reference protocol uses a 12-byte header followed by 0-256 payload bytes.

| Offset | Size | Field |
|---:|---:|---|
| 0 | 4 | magic `USB1` (`0x55425331`, little-endian) |
| 4 | 1 | version |
| 5 | 1 | command |
| 6 | 2 | payload length |
| 8 | 4 | CRC field |
| 12 | N | payload |

The firmware and Python host utility validate magic, version, exact packet size, payload bound and CRC before command dispatch. Commands: ping, get-info, echo, get-counters and reset-counters.

This packet protocol is an application layer above USB bulk endpoints. It is not the USB standard control-transfer protocol.
