# Test Plan

## Host-side protocol tests
- valid packet round-trip;
- CRC corruption rejection;
- short headers and oversized payloads;
- unsupported commands;
- sequence-number preservation;
- transfer timeout and device-disconnect handling.

## Firmware reference tests
- descriptor lengths;
- address/configuration state transitions;
- command echo;
- malformed packet rejection;
- counter command behavior.

## Hardware measurements
Record enumeration success, control-transfer outcomes, bulk throughput, round-trip latency percentiles, dropped packets, reconnect recovery time and CPU load. Do not report simulated timings as measured USB hardware throughput.
