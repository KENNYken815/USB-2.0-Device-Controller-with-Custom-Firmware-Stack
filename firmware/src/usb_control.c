#include "usb_control.h"
#include "usb_descriptors.h"
#include <string.h>
#define REQ_GET_STATUS 0u
#define REQ_CLEAR_FEATURE 1u
#define REQ_SET_FEATURE 3u
#define REQ_SET_ADDRESS 5u
#define REQ_GET_DESCRIPTOR 6u
#define REQ_GET_CONFIGURATION 8u
#define REQ_SET_CONFIGURATION 9u
static usb_ctrl_response_t stall(void){return (usb_ctrl_response_t){USB_CTRL_STALL,NULL,0};}
static usb_ctrl_response_t data_in(const uint8_t*p,uint16_t n){return (usb_ctrl_response_t){USB_CTRL_DATA_IN,p,n};}
usb_ctrl_response_t usb_control_handle_setup(usb_device_model_t*d,const usb_setup_packet_t*s,uint8_t*scratch,size_t cap){if(!d||!s)return stall();
 switch(s->bRequest){
 case REQ_GET_DESCRIPTOR:{if((s->bmRequestType&0x60u)!=0)return stall();uint8_t type=(uint8_t)(s->wValue>>8);size_t n=0;const uint8_t*p=NULL;if(type==1)p=usb_device_descriptor(&n);else if(type==2)p=usb_configuration_descriptor(&n);else return stall();if(n>s->wLength)n=s->wLength;return data_in(p,(uint16_t)n);}
 case REQ_SET_ADDRESS:if(s->bmRequestType!=0x00u||s->wIndex||s->wLength||s->wValue>127)return stall();if(usb_device_model_set_address(d,(uint8_t)s->wValue)!=USB_STATUS_OK)return stall();return (usb_ctrl_response_t){USB_CTRL_STATUS_IN,NULL,0};
 case REQ_SET_CONFIGURATION:if(s->bmRequestType!=0x00u||s->wIndex||s->wLength||s->wValue>1)return stall();if(usb_device_model_set_configuration(d,(uint8_t)s->wValue)!=USB_STATUS_OK)return stall();return (usb_ctrl_response_t){USB_CTRL_STATUS_IN,NULL,0};
 case REQ_GET_CONFIGURATION:if(s->bmRequestType!=0x80u||s->wLength==0||!scratch||cap==0)return stall();scratch[0]=d->configuration;return data_in(scratch,1);
 case REQ_GET_STATUS:if((s->bmRequestType&0x80u)==0||s->wLength<2||!scratch||cap<2)return stall();scratch[0]=0;scratch[1]=0;return data_in(scratch,2);
 case REQ_CLEAR_FEATURE:case REQ_SET_FEATURE:/* Endpoint halt needs controller-specific endpoint control. */return stall();
 default:return stall();
 }
}
