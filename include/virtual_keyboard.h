#ifndef VIRTUAL_KEYBOARD
#define VIRTUAL_KEYBOARD

#include <linux/uinput.h>

#define VENDOR_ID 0x1234
#define PRODUCT_ID 0x5678

#define VIRTUAL_KEYBOARD_NAME "DAVINCI VKEYBOARD"

int initVKeyboard(struct uinput_setup* usetup);

void emitKey(int fd, int type, int code, int val);

#endif // VIRTUAL_KEYBOARD
