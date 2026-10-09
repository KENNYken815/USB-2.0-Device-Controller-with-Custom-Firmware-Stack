#ifndef USB_CONTROL_H
#define USB_CONTROL_H
#include <stddef.h>
#include <stdint.h>
#include "usb_device_model.h"
typedef struct { uint8_t bmRequestType,bRequest; uint16_t wValue,wIndex,wLength; } usb_setup_packet_t;
typedef enum { USB_CTRL_STALL=0, USB_CTRL_DATA_IN, USB_CTRL_STATUS_IN, USB_CTRL_STATUS_OUT, USB_CTRL_NO_DATA } usb_ctrl_result_t;
typedef struct { usb_ctrl_result_t result; const uint8_t *data; uint16_t length; } usb_ctrl_response_t;
usb_ctrl_response_t usb_control_handle_setup(usb_device_model_t *device,const usb_setup_packet_t *setup,uint8_t *scratch,size_t scratch_capacity);
#endif
