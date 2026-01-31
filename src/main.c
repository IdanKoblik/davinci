#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/uinput.h>
#include <linux/input.h>
#include <string.h>
#include <signal.h>
#include "device.h"
#include "virtual_keyboard.h"

int fd = -1;
int stylusFd = -1;

void shutdown(int signum);

int main(void) {
    signal(SIGINT, shutdown);
    signal(SIGTERM, shutdown);

    DeviceSpec spec = {"056a", "037b"};
    Stylus stylus;
    int result = findStylus(spec, &stylus);

    if (result == 0) {
        printf("Stylus found.\n%s", stylus.path);
    } else {
        printf("Stylus not found (expected behavior in test env).\n");
        return -1;
    }

    struct uinput_setup usetup;
    fd = initVKeyboard(&usetup);
    if (fd == -1) {
        fprintf(stderr, "Failed to initialize virtual keyboard\n");
        return -1;
    }

    stylusFd = open(stylus.path, O_RDONLY);
    if (stylusFd == -1) {
        fprintf(stderr, "Failed to open stylus device\n");
        shutdown(0);
    }

    struct input_event ev;
    for (;;) {
        const ssize_t n = read(stylusFd, &ev, sizeof(ev));
        if (n != sizeof(ev)) {
            fprintf(stderr, "Cannot read stylus input");
            continue;
        }

        if (ev.type == EV_KEY) {
            if (ev.code == BTN_STYLUS && ev.value == 1) {
                emitKey(fd, EV_KEY, KEY_A, 1);
                emitKey(fd, EV_SYN, SYN_REPORT, 0);
                emitKey(fd, EV_KEY, KEY_A, 0);
                emitKey(fd, EV_SYN, SYN_REPORT, 0);

                sleep(1);
            }
        }
    }

    shutdown(0);
    return 0;
}

void shutdown(int signum) {
    printf("\nSignal %d received. Closing davinci.\n", signum);

    if (fd != -1) {
        ioctl(fd, UI_DEV_DESTROY);
        close(fd);
    }

    if (stylusFd != -1)
        close(stylusFd);
    
    _exit(0);
}