#include <unistd.h>

#include "config.h"
#include "device.h"
#include "keyboard.h"

int main(void) {
    /*
    const int fd = create_keyboard();
    if (fd < 0) {
        return -1;
    }

    const int device = get_device();
    if (device < 0) {
        destroy_keyboard(fd);
        return -1;
    }
    */

    Config cfg = {
        .vendor_id = "0x056a",
        .apps = (AppMapping[]){
            {
                .app_name = "krita",
                .shortcuts = {
                    { STYLUS_BTN_1, "Ctrl+Z" },
                    { STYLUS_BTN_2, "Ctrl+Y" }
                }
            },
            {
                .app_name = "gimp",
                .shortcuts = {
                    { STYLUS_BTN_1, "B" },
                    { STYLUS_BTN_2, "E" }
                }
            }
        },
        .size = 2
    };

    write_config(&cfg);

    /*
    struct input_event ev;
    while (1) {
        const ssize_t n = read (device, &ev, sizeof(ev));
        if (n != sizeof(ev)) {
            perror ("read");
            return -1;
        }

        if (ev.type == EV_KEY) {
            if (ev.code == BTN_STYLUS) {
                if (!ev.value) {
                    send_key(fd, KEY_A, 1);
                    send_syn(fd);

                    send_key(fd, KEY_A, 0);
                    send_syn(fd);
                }
            } else if (ev.code == BTN_STYLUS2) {
                printf("Higher button (%s)\n", ev.value ? "pressed" : "released");
            }
        }
    }
*/

    /*close(fd);
    destroy_keyboard(fd);*/
    return 0;
}