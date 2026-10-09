#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "usb_protocol.h"
#include "usb_device_model.h"
#include "usb_descriptors.h"
int main(void){uint8_t a[USB_PROTO_MAX_PACKET],b[USB_PROTO_MAX_PACKET];size_t n=0;usb_packet_view_t p;const uint8_t msg[]={1,2,3,4};assert(usb_packet_encode(a,sizeof(a),USB_CMD_ECHO,77,msg,sizeof(msg),&n)==0);assert(usb_packet_decode(a,n,&p)==USB_STATUS_OK);assert(p.sequence==77&&p.payload_len==4&&!memcmp(p.payload,msg,4));a[USB_PROTO_HEADER_SIZE]^=1;assert(usb_packet_decode(a,n,&p)!=USB_STATUS_OK);usb_device_model_t d;usb_device_model_init(&d);assert(usb_device_model_set_address(&d,4)==USB_STATUS_OK);assert(usb_device_model_set_configuration(&d,1)==USB_STATUS_OK);usb_packet_encode(a,sizeof(a),USB_CMD_PING,1,NULL,0,&n);assert(usb_device_model_handle_bulk_out(&d,a,n,b,sizeof(b))>0);size_t dl,cl;assert(usb_device_descriptor(&dl)&&dl==18);assert(usb_configuration_descriptor(&cl)&&cl==32);puts("USB protocol tests passed.");return 0;}
