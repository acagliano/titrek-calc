#include "descriptors.h"

const usb_device_descriptor_t dev_desc = {
        .bLength            = sizeof(usb_device_descriptor_t),
        .bDescriptorType    = USB_DEVICE_DESCRIPTOR,
        .bcdUSB             = 0x0200,
        .bDeviceClass       = 0,
        .bDeviceSubClass    = 0,
        .bDeviceProtocol    = 0,
        .bMaxPacketSize0    = 64,
        .idVendor           = 0x0451,
        .idProduct          = 0xE008,
        .bcdDevice          = 0x0220,
        .iManufacturer      = 0,
        .iProduct           = 0,
        .iSerialNumber      = 0,
        .bNumConfigurations = 1,
};

const struct {
    usb_configuration_descriptor_t conf;
    usb_interface_descriptor_t inter;
    usb_endpoint_descriptor_t in;
    usb_endpoint_descriptor_t out;
} conf_desc = {
        {
                .bLength             = sizeof(usb_configuration_descriptor_t),
                .bDescriptorType     = USB_CONFIGURATION_DESCRIPTOR,
                .wTotalLength        = sizeof(conf_desc),
                .bNumInterfaces      = 1,
                .bConfigurationValue = 1,
                .iConfiguration      = 0,
                .bmAttributes        = 0x80,
                .bMaxPower           = 250,
        },
        {
                .bLength            = sizeof(usb_interface_descriptor_t),
                .bDescriptorType    = USB_INTERFACE_DESCRIPTOR,
                .bInterfaceNumber   = 0x00,
                .bAlternateSetting  = 0x00,
                .bNumEndpoints      = 0x02,
                .bInterfaceClass    = 0x02,
                .bInterfaceSubClass = 0x0C,
                .bInterfaceProtocol = 0x07,
                .iInterface         = 0x00,
        },
        {
                .bLength            = sizeof(usb_endpoint_descriptor_t),
                .bDescriptorType    = USB_ENDPOINT_DESCRIPTOR,
                .bEndpointAddress   = 0x81,
                .bmAttributes       = 0x02,
                .wMaxPacketSize     = 0x0040,
                .bInterval          = 0x00
        },
        {
                .bLength            = sizeof(usb_endpoint_descriptor_t),
                .bDescriptorType    = USB_ENDPOINT_DESCRIPTOR,
                .bEndpointAddress   = 0x02,
                .bmAttributes       = 0x02,
                .wMaxPacketSize     = 0x0040,
                .bInterval          = 0x00
        }
};

const usb_configuration_descriptor_t * const conf_descs[] = {
        &conf_desc.conf
};

const usb_string_descriptor_t langids = {
        .bLength         = sizeof langids,
        .bDescriptorType = USB_STRING_DESCRIPTOR,
        .bString         = {0x0409}
};

const usb_standard_descriptors_t usb_descriptors = {
        &dev_desc,
        conf_descs,
        &langids,
        0,
        NULL
};