#include <assert.h>
#include <stdio.h>
#include "usb_control.h"
#include "usb_descriptors.h"
int main(void){usb_device_model_t d;usb_device_model_init(&d);uint8_t scratch[64];
 usb_setup_packet_t s={.bmRequestType=0x80,.bRequest=6,.wValue=0x0100,.wLength=18};usb_ctrl_response_t r=usb_control_handle_setup(&d,&s,scratch,sizeof(scratch));assert(r.result==USB_CTRL_DATA_IN&&r.length==18&&r.data[1]==1);
 s=(usb_setup_packet_t){.bmRequestType=0,.bRequest=5,.wValue=5};r=usb_control_handle_setup(&d,&s,scratch,sizeof(scratch));assert(r.result==USB_CTRL_STATUS_IN&&d.address==5);
 s=(usb_setup_packet_t){.bmRequestType=0,.bRequest=9,.wValue=1};r=usb_control_handle_setup(&d,&s,scratch,sizeof(scratch));assert(r.result==USB_CTRL_STATUS_IN&&d.configuration==1);
 s=(usb_setup_packet_t){.bmRequestType=0x80,.bRequest=8,.wLength=1};r=usb_control_handle_setup(&d,&s,scratch,sizeof(scratch));assert(r.result==USB_CTRL_DATA_IN&&r.data[0]==1);
 puts("USB control-request tests passed.");return 0;}
