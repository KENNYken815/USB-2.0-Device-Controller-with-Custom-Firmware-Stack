#include "usb_descriptors.h"
/* Example vendor-specific USB 2.0 descriptors. A real controller stack must expose
 * these to EP0 and drive reset/address/configuration through the USB peripheral. */
static const uint8_t device_desc[]={18,1,0x00,0x02,0x00,0x00,0x00,64,0x09,0x12,0x01,0x00,0x00,0x01,1,2,3,1};
static const uint8_t config_desc[]={9,2,32,0,1,1,0,0x80,50,9,4,0,0,2,0xFF,0,0,0,7,5,0x01,2,64,0,0,7,5,0x81,2,64,0,0};
const uint8_t*usb_device_descriptor(size_t*n){if(n)*n=sizeof(device_desc);return device_desc;}
const uint8_t*usb_configuration_descriptor(size_t*n){if(n)*n=sizeof(config_desc);return config_desc;}
