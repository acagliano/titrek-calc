#include "usb.h"
#include "pico_stack.h"
#include "pico_device.h"
#include "descriptors.h"

#include <debug.h>
#include <usbdrvce.h>
#include <string.h>
#include <keypadc.h>

#define MTU 1518

#define min(a, b) ((a) < (b) ? (a) : (b))

//union eem_header {
//    uint16_t type : 1;
//    struct {
//        uint16_t type : 1;
//        uint16_t crc : 1;
//        uint16_t length : 14;
//    } data;
//    struct {
//        uint16_t type : 1;
//        uint16_t reserved : 1;
//        uint16_t cmd : 3;
//        uint16_t param : 11;
//    } command;
//};

typedef uint16_t eem_header_t;

#define EEM_HEADER_TYPE_DATA (0 << 15)
#define EEM_HEADER_TYPE_CMD  (1 << 15)

#define EEM_HEADER_NO_CHECKSUM  (0 << 14)
#define EEM_HEADER_CHECKSUM     (1 << 14)

#define EEM_HEADER_MASK_CMD   (7 << 11)
#define EEM_HEADER_MASK_PARAM (0x07ff)

enum eem_commands {
    EEM_ECHO                    = (0 << 11),
    EEM_ECHO_RESPONSE           = (1 << 11),
    EEM_SUSPEND_HINT            = (2 << 11),
    EEM_RESPONSE_HINT           = (3 << 11),
    EEM_RESPONSE_COMPLETE_HINT  = (4 << 11),
    EEM_TICKLE                  = (5 << 11),
};

struct eem_packet {
    eem_header_t header;
    uint8_t eth_packet[MTU];
};

static usb_error_t recv_callback(usb_endpoint_t endpoint,
                          usb_transfer_status_t status,
                          size_t transferred,
                          usb_transfer_data_t *data);

static int pkt_size = -1;
static struct eem_packet pkt;
static usb_endpoint_t rx_ep = NULL;
static usb_endpoint_t tx_ep = NULL;

//static uint8_t recv_buf[USB_PACKET_SIZE];

/*static void send_echo_response(const char *data, size_t len) {
    struct {
        union eem_header header;
        char data[USB_PACKET_SIZE - sizeof(union eem_header)];
    } packet = {
        .header = {
            .command.type = 1,
            .command.cmd = EEM_ECHO_RESPONSE,
            .command.param = len,
        },
        .data = {}
    };
    if(len > USB_PACKET_SIZE - sizeof(union eem_header)) {
        printf("Why is this echo so long? (%u bytes)\n", len);
        return;
    }
    memcpy(packet.data, data, len);
    // todo: send packet
}

static void handle_usb_packet(const uint8_t *data) {
    static bool reading_header = true;
    static union eem_header header;
    static size_t written = 0;
    static uint8_t eth_packet[MTU];

    const uint8_t *in = data;
    const uint8_t *end = data + USB_PACKET_SIZE;

    while(in < end) {
        if(reading_header) {
            const uint8_t *header_end = (uint8_t*) &header + 1;
            size_t size = min(sizeof header - written, end - in);
            memcpy(((uint8_t *)&header) + written, in, size);
            written += size;
            in += size;
            if(written == sizeof header) {
                written = 0;
                if(header.type == 1) {
                    // Command packet
                    if(header.command.cmd == EEM_ECHO) {
                        union eem_header new_header = {
                                .command.type = 1,
                                .command.cmd = EEM_ECHO_RESPONSE,
                                .command.param = header.command.param,
                        };
                        reading_header = false;
                        memcpy(eth_packet, &new_header, sizeof new_header);
                        written += sizeof new_header;
                    } else if(header.command.cmd == EEM_ECHO_RESPONSE) {
                        reading_header = false;
                    }
                } else {
                    // Data packet
                    reading_header = false;
                }
            }
        } else {
            size_t total_size = header.type ? header.command.param : header.data.length;
            size_t size = min(total_size - written, end - in);
            memcpy(eth_packet + written, in, size);
            written += size;
            in += size;
            if(written == total_size) {
                if(header.type == 1 && header.command.cmd == EEM_ECHO) {
                    // todo: send echo response
                }
                reading_header = true;
                written = 0;
            }
        }
    }

    // todo: schedule next packet
}*/

// on second thought, standards compliance is overrated, let's just
// implement the most convenient, limited subset of the standard that
// still Works on My Machine and deal with the rest Later™

static void schedule_read() {
    pkt_size = -1;
    usb_ScheduleTransfer(rx_ep, &pkt, sizeof pkt, recv_callback, NULL);
}

static usb_error_t recv_callback(usb_endpoint_t endpoint __attribute__((unused)),
                          usb_transfer_status_t status,
                          size_t transferred,
                          usb_transfer_data_t *data __attribute__((unused))) {
    if(status) {
        dbg("Err in recv: %x\n", status);
        pkt_size = -1;
        return USB_SUCCESS;
    }
    if(transferred < 2) {
        pkt_size = -1;
        schedule_read();
        return USB_SUCCESS;
    }
    // Ignore commands
    if (pkt.header & EEM_HEADER_TYPE_CMD) {
        dbg("rec cmd %x size %u\n", pkt.header & EEM_HEADER_MASK_CMD, transferred);
        pkt_size = -1;
        schedule_read();
    } else {
//        dbg("rec pkt size %u\n", transferred);
        pkt_size = transferred - sizeof pkt.header;
    }

    return USB_SUCCESS;
}

static usb_error_t send_callback(usb_endpoint_t endpoint __attribute__((unused)),
                                 usb_transfer_status_t status __attribute__((unused)),
                                 size_t transferred,
                                 usb_transfer_data_t *data) {
//    dbg("sent packet size %u\n", transferred);
    PICO_FREE(data);
    return USB_SUCCESS;
}

static usb_error_t handle_event(usb_event_t event, void *event_data __attribute__((unused)),
                                usb_callback_data_t *callback_data __attribute__((unused))) {
    if (event == USB_HOST_CONFIGURE_EVENT) {
        dbg("device configured\n");
        usb_device_t host = usb_FindDevice(NULL, NULL, USB_SKIP_HUBS);
        tx_ep = usb_GetDeviceEndpoint(host, 0x81);
        rx_ep = usb_GetDeviceEndpoint(host, 0x02);
        schedule_read();
    }
    return USB_SUCCESS;
}

void init_usb(void) {
    dbg("initting usb\n");
    usb_Init(handle_event, NULL, &usb_descriptors, USB_DEFAULT_INIT_FLAGS);
}

static int pico_eth_send(struct pico_device *dev __attribute__((unused)), void *buf, int len) {
//    dbg("send packet size %u\n", len);
    struct eem_packet *copy = PICO_ZALLOC(len + sizeof(eem_header_t) + 4);
    if(!copy) {
        dbg("Send alloc failed\n");
        return 0;
    }
    memcpy(&copy->eth_packet, buf, len);
    uint8_t steak[] = {0xde, 0xad, 0xbe, 0xef};
    memcpy(&copy->eth_packet[len], steak, 4);
    copy->header = EEM_HEADER_TYPE_DATA | EEM_HEADER_NO_CHECKSUM | (len + 4);
    usb_ScheduleTransfer(tx_ep, copy, len + sizeof(eem_header_t) + 4, send_callback, copy);
    return len;
}

static int pico_eth_poll(struct pico_device *dev __attribute__((unused)), int loop_score) {
    if (pkt_size > 0) {
        pico_stack_recv(dev, pkt.eth_packet, pkt_size); /* this will copy the frame into the stack */
    }
    if (pkt_size != -1) {
        schedule_read();
        loop_score--;
    }

    return loop_score;
}

struct pico_device *pico_eem_create(const char *name, uint8_t *mac) {
    while(!tx_ep) {
        usb_HandleEvents();
        if(kb_IsDown(kb_KeyClear)) return NULL;
    }
    struct pico_device *dev = PICO_ZALLOC(sizeof(struct pico_device));
    if(!dev) {
        return NULL;
    }

    dev->send = pico_eth_send;
    dev->poll = pico_eth_poll;

    if(0 != pico_device_init(dev, name, mac)) {
        dbg("Device init failed.\n");
        PICO_FREE(dev);
        return NULL;
    }

    dbg("Created device\n");

    return dev;
}
