# EP0 Control-Request Model

The control-request reference layer handles a bounded subset of standard USB requests:
- GET_DESCRIPTOR for device and configuration descriptors;
- SET_ADDRESS with range/request validation;
- SET_CONFIGURATION for configuration 0 or 1;
- GET_CONFIGURATION;
- GET_STATUS.

It returns response intent (data IN, status IN, or stall) for a USB controller driver to execute. It does not implement the electrical bus protocol, SET_ADDRESS status-stage timing at the peripheral level, SETUP packet FIFO reads, DATA0/DATA1 toggles, NAK behavior, or endpoint hardware registers. Endpoint CLEAR_FEATURE/SET_FEATURE are deliberately stalled until a controller-specific endpoint-halt implementation exists.
