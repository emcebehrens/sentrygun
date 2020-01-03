#include <stdio.h>
#include "libusb.h"
#include "libusc.h"

static void print_devs(libusb_device **devs)
{
    libusb_device *dev;
    int i = 0, j = 0;
    uint8_t path[8];

    while ((dev = devs[i++]) != NULL) {
        struct libusb_device_descriptor desc;
        int r = libusb_get_device_descriptor(dev, &desc);
        if (r < 0) {
            fprintf(stderr, "failed to get device descriptor");
            return;
        }

        printf("%04x:%04x (bus %d, device %d)",
            desc.idVendor, desc.idProduct,
            libusb_get_bus_number(dev), libusb_get_device_address(dev));

        r = libusb_get_port_numbers(dev, path, sizeof(path));
        if (r > 0) {
            printf(" path: %d", path[0]);
            for (j = 1; j < r; j++)
                printf(".%d", path[j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[])
{
    //libusb_device **devs;
    libusc_device* device = NULL;
    libusc_device **device_list;
    int r;
    ssize_t cnt;

    printf("Testing libusb and Meastro servo controller\n");

    r = libusb_init(NULL);
    if (r < 0)
        return r;

    /*cnt = libusb_get_device_list(NULL, &devs);
    if (cnt < 0)
        return (int) cnt;

    print_devs(devs);*/

    int numChannels = device ? libusc_get_num_channels(device) : 0;
    printf("libusc numChannels = %d\n", numChannels);

    cnt = libusc_get_device_list(NULL, &device_list);
    for (int i=0; i<cnt; i++) {
        libusc_device_settings settings;
        libusc_get_device_settings(device_list[i], &settings);
        //deviceList->addItem("#" + QString::number(settings.serialDeviceNumber), qVariantFromValue((void*)device_list[i]));

        device = device_list[0];
    }



    libusc_free_device_list(device_list, 1);
    //libusb_free_device_list(devs, 1);
    libusb_exit(NULL);
    return 0;
}
