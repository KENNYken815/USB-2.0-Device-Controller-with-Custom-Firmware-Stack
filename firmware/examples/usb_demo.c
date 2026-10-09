#include <stdio.h>
#include <string.h>
#include "usb_device_model.h"
#include "usb_descriptors.h"
int main(void){usb_device_model_t d;usb_device_model_init(&d);size_t dn,cn;usb_device_descriptor(&dn);usb_configuration_descriptor(&cn);printf("USB descriptor model: device=%zu bytes config=%zu bytes\n",dn,cn);usb_device_model_set_address(&d,7);usb_device_model_set_configuration(&d,1);uint8_t tx[USB_PROTO_MAX_PACKET],rx[USB_PROTO_MAX_PACKET];size_t n=0;const uint8_t msg[]="hello-device";usb_packet_encode(tx,sizeof(tx),USB_CMD_ECHO,42,msg,sizeof(msg)-1,&n);size_t rn=usb_device_model_handle_bulk_out(&d,tx,n,rx,sizeof(rx));usb_packet_view_t p;usb_status_t s=usb_packet_decode(rx,rn,&p);printf("bulk echo: status=%d seq=%u payload=%.*s\n",s,p.sequence,p.payload_len,p.payload);return s==USB_STATUS_OK?0:1;}
