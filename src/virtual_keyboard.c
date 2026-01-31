#include "virtual_keyboard.h"

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>
#include <stdio.h>
#include <string.h>

#define UINPUT_PATH "/dev/uinput"

int initVKeyboard(struct uinput_setup* usetup) {
    int fd = open(UINPUT_PATH, O_WRONLY | O_NONBLOCK);
    if (fd == -1)
        return -1;

    ioctl(fd, UI_SET_EVBIT, EV_KEY);

    for (int i = 1; i <= 120; i++)
        ioctl(fd, UI_SET_KEYBIT, i);

    memset(usetup, 0, sizeof(*usetup));
    usetup->id.bustype = BUS_USB;
    usetup->id.vendor = VENDOR_ID; 
    usetup->id.product = PRODUCT_ID; 
    strcpy(usetup->name, VIRTUAL_KEYBOARD_NAME);

    ioctl(fd, UI_DEV_SETUP, usetup);
    ioctl(fd, UI_DEV_CREATE);

    sleep(1);

    return fd;
}

void emitKey(int fd, int type, int code, int val) {
   struct input_event ie;

   ie.type = type;
   ie.code = code;
   ie.value = val;
   
   /* timestamp values below are ignored */
   ie.time.tv_sec = 0;
   ie.time.tv_usec = 0;

   write(fd, &ie, sizeof(ie));
}