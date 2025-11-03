#ifndef DAVINCI_KEYBOARD_H
#define DAVINCI_KEYBOARD_H

#define VENDOR_ID 0x1234
#define PRODUCT_ID 0x5678

int create_keyboard(void);

int destroy_keyboard(int fd);

void send_key(int fd, int code, int value);

void send_syn(int fd);

#endif //DAVINCI_KEYBOARD_H