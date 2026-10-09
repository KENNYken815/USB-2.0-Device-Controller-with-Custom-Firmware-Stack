#ifndef USB_PROTOCOL_H
#define USB_PROTOCOL_H
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#define USB_PROTO_MAGIC 0x55425331u
#define USB_PROTO_VERSION 1u
#define USB_PROTO_MAX_PAYLOAD 256u
#define USB_PROTO_HEADER_SIZE 16u
#define USB_PROTO_MAX_PACKET (USB_PROTO_HEADER_SIZE + USB_PROTO_MAX_PAYLOAD)
typedef enum { USB_CMD_PING=1, USB_CMD_GET_INFO=2, USB_CMD_ECHO=3, USB_CMD_GET_COUNTERS=4, USB_CMD_RESET_COUNTERS=5 } usb_command_t;
typedef enum { USB_STATUS_OK=0, USB_STATUS_BAD_MAGIC=1, USB_STATUS_BAD_VERSION=2, USB_STATUS_BAD_LENGTH=3, USB_STATUS_BAD_COMMAND=4, USB_STATUS_INTERNAL=5 } usb_status_t;
typedef struct { uint8_t version; uint8_t command; uint16_t payload_len; uint32_t sequence; uint32_t crc32; const uint8_t *payload; } usb_packet_view_t;
typedef struct { uint32_t rx_packets, tx_packets, malformed_packets, command_errors; } usb_counters_t;
uint32_t usb_crc32(const uint8_t *data, size_t length);
int usb_packet_encode(uint8_t *out, size_t capacity, uint8_t command, uint32_t sequence, const uint8_t *payload, uint16_t payload_len, size_t *written);
usb_status_t usb_packet_decode(const uint8_t *buffer, size_t length, usb_packet_view_t *packet);
size_t usb_handle_command(const usb_packet_view_t *request, uint8_t *response, size_t capacity, usb_counters_t *counters);
#endif
