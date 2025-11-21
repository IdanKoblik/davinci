#include "keyboard.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <linux/uinput.h>

std::string Keyboard::getName() const {
    return this->name;
}

int Keyboard::getFd() const {
    return this->fd;
}

int Keyboard::createKeyboard(std::string name) {
    const int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (fd < 0) {
        perror("Error opening /dev/uinput");
        return -1;
    }

    ioctl(fd, UI_SET_EVBIT, EV_KEY);
    ioctl(fd, UI_SET_EVBIT, EV_SYN);
    for (int k = KEY_RESERVED; k <= KEY_MICMUTE; k++)
        ioctl(fd, UI_SET_KEYBIT, k);

    uinput_user_dev uidev;
    memset(&uidev, 0, sizeof(uidev));
    snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "%s", name.c_str());
    this->name = uidev.name;

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

    this->fd = fd;
    return fd;
}

bool Keyboard::closeKeyboard() {
    if (ioctl(this->fd, UI_DEV_DESTROY) == -1) {
        perror("Error destroying keyboard");
        return false;
    }

    if (close(this->fd) == -1) {
        perror("Error closing keyboard");
        return false;
    }

    return true;
}

bool Keyboard::sendKey(int code, int value) {
    input_event ev;
    ev.type = EV_KEY;
    ev.code = code;
    ev.value = value;
    gettimeofday(&ev.time, nullptr);

    if (write(this->fd, &ev, sizeof(ev)) < 0) {
        perror("write key event");
        return false;
    }

    return true;
}

bool Keyboard::sendSyn() {
    input_event ev;
    ev.type = EV_SYN;
    ev.code = SYN_REPORT;
    ev.value = 0;
    gettimeofday(&ev.time, nullptr);

    if (write(this->fd, &ev, sizeof(ev)) < 0) {
        perror("write syn event");
        return false;
    }

    return true;
}
