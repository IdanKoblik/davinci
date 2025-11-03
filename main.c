#include <stdio.h>
#include <unistd.h>
#include <linux/input-event-codes.h>

#include "keyboard.h"

int main(void) {
    const int fd = create_keyboard();
    if (fd < 0) {
        return -1;
    }

    send_key(fd, KEY_A, 1);
    send_syn(fd);

    send_key(fd, KEY_A, 0);
    send_syn(fd);

    destroy_keyboard(fd);
    return 0;
}