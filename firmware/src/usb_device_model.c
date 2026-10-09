#include "usb_device_model.h"
#include <string.h>
void usb_device_model_init(usb_device_model_t*d){if(d)memset(d,0,sizeof(*d));}
usb_status_t usb_device_model_set_address(usb_device_model_t*d,uint8_t a){if(!d||a>127)return USB_STATUS_BAD_LENGTH;d->address=a;d->state=a?USB_STATE_ADDRESSED:USB_STATE_DEFAULT;return USB_STATUS_OK;}
usb_status_t usb_device_model_set_configuration(usb_device_model_t*d,uint8_t c){if(!d||c>1)return USB_STATUS_BAD_LENGTH;if(c==0){d->configuration=0;d->state=d->address?USB_STATE_ADDRESSED:USB_STATE_DEFAULT;}else if(d->address){d->configuration=1;d->state=USB_STATE_CONFIGURED;}else return USB_STATUS_BAD_LENGTH;return USB_STATUS_OK;}
size_t usb_device_model_handle_bulk_out(usb_device_model_t*d,const uint8_t*in,size_t il,uint8_t*out,size_t oc){if(!d||d->state!=USB_STATE_CONFIGURED||!in||!out)return 0;usb_packet_view_t p;usb_status_t s=usb_packet_decode(in,il,&p);if(s!=USB_STATUS_OK){d->counters.malformed_packets++;return 0;}return usb_handle_command(&p,out,oc,&d->counters);}
