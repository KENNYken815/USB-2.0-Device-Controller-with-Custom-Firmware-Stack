#ifndef USB_DESCRIPTORS_H
#define USB_DESCRIPTORS_H
#include <stddef.h>
#include <stdint.h>
const uint8_t *usb_device_descriptor(size_t *length);
const uint8_t *usb_configuration_descriptor(size_t *length);
#endif
