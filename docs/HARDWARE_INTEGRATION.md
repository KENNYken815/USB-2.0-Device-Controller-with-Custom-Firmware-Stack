# Hardware Integration

Suggested first target: STM32F4/F7/H7 with a USB full-speed device peripheral, vendor-specific interface and two bulk endpoints. Confirm the selected MCU's USB device mode, endpoint count, FIFO/RAM limits and HAL/LL support before selecting a board.

A real device controller must implement the USB reset, EP0 setup/data/status stages, descriptor requests, SET_ADDRESS timing, SET_CONFIGURATION, endpoint halt/clear-halt, suspend/resume, packet toggles and transfer completion using the selected USB peripheral. The descriptors and state machine in this repository are a reference model; they do not replace the MCU's low-level USB PHY/controller driver.

## Suggested firmware layering
- USB IRQ: capture controller events and acknowledge hardware status.
- EP0/control task: descriptor responses and standard requests.
- Bulk RX: copy received packets to a bounded buffer.
- Protocol task: validate CRC/length and dispatch commands.
- Bulk TX: queue response packets and submit controller transfers.

## Debugging
Use USBPcap/Wireshark on Windows or a hardware USB protocol analyzer. Check enumeration descriptor reads, SET_ADDRESS, SET_CONFIGURATION, bulk endpoint addresses, short packets, timeouts and reset/retry behavior.
