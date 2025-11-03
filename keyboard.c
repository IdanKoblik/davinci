#include "keyboard.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <linux/uinput.h>

int create_keyboard(void) {
    const int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (fd < 0) {
        perror("Error opening /dev/uinput");
        return -1;
    }

    ioctl(fd, UI_SET_EVBIT, EV_KEY);
    ioctl(fd, UI_SET_EVBIT, EV_SYN);
    for (int k = KEY_A; k <= KEY_Z; k++)
        ioctl(fd, UI_SET_KEYBIT, k);

    struct uinput_user_dev uidev;
    memset(&uidev, 0, sizeof(uidev));
    snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "Davinci-VirtualKeyboard");
    uidev.id.bustype = BUS_USB;
    uidev.id.vendor = VENDOR_ID;
    uidev.id.product = PRODUCT_ID;
    uidev.id.version = 1;
    if (write(fd, &uidev, sizeof(uidev)) < 0) {
        perror("write");
        close(fd);
        return 1;
    }

    if (ioctl(fd, UI_DEV_CREATE) < 0) {
        perror("Error creating uinput device");
        close(fd);
        return -1;
    }

    sleep(2);
    return fd;
}

int destroy_keyboard(int fd) {
    sleep(2);

    if (ioctl(fd, UI_DEV_DESTROY) == -1) {
        perror("Error destroying keyboard");
        return -1;
    }

    if (close(fd) == -1) {
        perror("Error closing keyboard");
        return -1;
    }

    return 0;
}

void send_key(int fd, int code, int value) {
    struct input_event ev = {0};
    ev.type = EV_KEY;
    ev.code = code;
    ev.value = value;
    gettimeofday(&ev.time, NULL);

    if (write(fd, &ev, sizeof(ev)) < 0) {
        perror("write key event");
    }
}

void send_syn(int fd) {
    struct input_event ev = {0};
    ev.type = EV_SYN;
    ev.code = SYN_REPORT;
    ev.value = 0;
    gettimeofday(&ev.time, NULL);

    if (write(fd, &ev, sizeof(ev)) < 0) {
        perror("write syn event");
    }
}