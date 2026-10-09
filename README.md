# USB 2.0 Device Controller with Custom Firmware Stack

An embedded C USB device/protocol reference project paired with a Python host utility. It models USB device/configuration descriptors, address/configuration state, bulk-endpoint command processing, packet validation, and a custom host-to-device binary protocol.

> **Scope:** The portable firmware code demonstrates descriptor data, USB state transitions and application protocol logic. It is not yet a complete hardware USB controller driver: real enumeration requires MCU-specific EP0, reset, endpoint/FIFO, packet-toggle and transfer-completion handling. No physical enumeration, throughput or board-level tests are claimed.

## Architecture

    PC Python utility (PyUSB)
             |
       USB bulk transfers
             |
    STM32 USB device controller
    [MCU-specific driver to integrate]
             |
    Descriptor / endpoint layer
             |
    Vendor-specific bulk interface
             |
    Binary packet validation (CRC)
             |
    Command dispatcher
      /      |      |      \
    ping    info   echo   counters

The algorithmic core does not depend on a specific STM32 HAL. The descriptor and state model are reference components intended to be connected to an STM32F4/F7/H7 USB device peripheral.

## Implemented Features

### USB descriptor reference
- USB 2.0 device descriptor.
- Configuration/interface descriptor data for a vendor-specific interface.
- Bulk OUT endpoint `0x01` and bulk IN endpoint `0x81` descriptors.
- Descriptor lengths checked by unit tests.

### Device state model
- Default, addressed and configured states.
- Address range validation.
- Configuration-state validation.
- Bulk protocol requests accepted only once configured.

### Custom binary protocol
Application packets use a 16-byte little-endian header followed by a payload of up to 256 bytes.

| Offset | Size | Field |
|---:|---:|---|
| 0 | 4 | Magic `USB1` |
| 4 | 1 | Protocol version |
| 5 | 1 | Command |
| 6 | 2 | Payload length |
| 8 | 4 | Sequence number |
| 12 | 4 | CRC-32 |
| 16 | N | Payload |

The firmware and Python utility check magic, version, exact length, payload bounds and CRC before command processing.

Supported commands:
- `ping` — liveness check;
- `info` — return device/protocol details;
- `echo` — validate a command-response data round trip;
- `counters` — return packet/error counters;
- `reset` — reset counters.

### Host-side utility
`host/usb_host.py` discovers a device by configurable VID/PID, selects bulk endpoints, sends a validated packet, enforces timeouts, parses the response and reports errors clearly. It requires PyUSB and a functioning USB device/controller implementation.

A protocol-only Python test module is included for the packet codec.

## Repository Structure

    USB-2.0-Device-Controller-with-Custom-Firmware-Stack/
    +-- docs/
    |   +-- HARDWARE_INTEGRATION.md
    |   +-- PROTOCOL.md
    |   +-- TEST_PLAN.md
    +-- firmware/
    |   +-- examples/usb_demo.c
    |   +-- include/
    |   |   +-- usb_descriptors.h
    |   |   +-- usb_device_model.h
    |   |   +-- usb_protocol.h
    |   +-- src/
    |   |   +-- usb_descriptors.c
    |   |   +-- usb_device_model.c
    |   |   +-- usb_protocol.c
    |   +-- tests/test_usb.c
    +-- host/
    |   +-- test_protocol.py
    |   +-- usb_host.py
    +-- .gitignore
    +-- LICENSE
    +-- Makefile
    +-- README.md

## Build and Test

Requirements:
- C11 compiler such as GCC or Clang;
- GNU Make;
- Python 3 for host-side tests;
- PyUSB only for communicating with a physical device.

Build the C demo and firmware reference tests:

    make

Run the demo:

    make demo

Run C tests:

    make test

Run Python packet-codec tests:

    cd host
    python -m unittest test_protocol.py

Install the host transport dependency when needed:

    python -m pip install pyusb

## Today's Milestone: Validated Command-Response Packet

The `usb_demo` performs a **software-model** transaction:
1. inspect the reference descriptor lengths;
2. set an address and configure the model;
3. encode an ECHO request with a sequence number and payload;
4. pass it through the firmware protocol validator;
5. decode and print the response.

It demonstrates application protocol round-trip logic. It does not prove that a computer has enumerated a physical USB device.

## Hardware Integration Path

For STM32F4/F7/H7, the remaining hardware-specific work includes:
- USB peripheral clock and PHY setup;
- reset/suspend/resume event handling;
- EP0 setup packet parsing and standard requests;
- descriptor responses through control transfers;
- correct SET_ADDRESS status-stage timing;
- SET_CONFIGURATION and endpoint enablement;
- endpoint FIFO/packet memory management;
- DATA0/DATA1 toggles, stalls and clear-halt;
- interrupt-driven IN/OUT completion;
- USB bus reset and disconnect recovery.

The descriptor arrays and device-state model are not a replacement for the USB peripheral driver. Integrate them with the selected vendor LL/HAL or a separately implemented controller driver before attempting physical enumeration.

## Host Debugging

Use USBPcap/Wireshark on Windows or a USB protocol analyzer to inspect:
- device/configuration descriptor reads;
- SET_ADDRESS and SET_CONFIGURATION;
- endpoint addresses and max packet sizes;
- bulk request/response sequence numbers;
- timeouts, malformed packets and reconnect recovery.

## Performance Measurements

When hardware is available, benchmark:
- enumeration success rate;
- round-trip latency percentiles;
- bulk throughput with several payload sizes;
- malformed-packet rejection;
- recovery after USB reset/disconnect;
- CPU load and buffer overflow behavior.

Do not treat desktop simulation or the demo output as measured USB throughput.

## License

MIT License.
