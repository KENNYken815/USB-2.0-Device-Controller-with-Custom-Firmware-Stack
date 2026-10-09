#ifndef USB_DEVICE_MODEL_H
#define USB_DEVICE_MODEL_H
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "usb_protocol.h"
typedef enum { USB_STATE_DEFAULT=0, USB_STATE_ADDRESSED, USB_STATE_CONFIGURED } usb_device_state_t;
typedef struct { usb_device_state_t state; uint8_t address; uint8_t configuration; usb_counters_t counters; } usb_device_model_t;
void usb_device_model_init(usb_device_model_t *device);
usb_status_t usb_device_model_set_address(usb_device_model_t *device, uint8_t address);
usb_status_t usb_device_model_set_configuration(usb_device_model_t *device, uint8_t configuration);
size_t usb_device_model_handle_bulk_out(usb_device_model_t *device,const uint8_t *input,size_t input_length,uint8_t *output,size_t output_capacity);
#endif
