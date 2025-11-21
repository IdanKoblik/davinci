#include <fcntl.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <linux/uinput.h>
#include <signal.h>
#include "config.h"
#include "keyboard.h"
#include "device.h"

Keyboard *keyboard = nullptr;
Config *cfg = nullptr;
int device = -1;

void onDisable(int signo);

int main() {
    const char *configPath = std::getenv("DV_CONFIG_PATH");
    if (configPath == nullptr) {
        perror("CONFIG_PATH environment variable not found.");
        return 0;
    }

    cfg = new Config();
    cfg->readConfig(configPath);

    keyboard = new Keyboard();
    keyboard->createKeyboard("Davinci"); // TODO const
   
    device = getDevice(*cfg);
    signal(SIGABRT, onDisable);
    signal(SIGTERM, onDisable);

    std::cout << device << std::endl;
    input_event ev;
    while (true) {
        const ssize_t n = read(device, &ev, sizeof(ev));
        if (n != sizeof(ev)) {
            perror("read");
            return -1;
        }

        if (ev.type == EV_KEY) {
            if (ev.code == BTN_STYLUS) {
                if (!ev.value) {
                    keyboard->sendKey(KEY_A, 1);
                    keyboard->sendSyn();

                    keyboard->sendKey(KEY_A, 0);
                    keyboard->sendSyn();
                }
            }
        }

    }
    return 0;
}

void onDisable(int signo) {
    close(device);

    if (keyboard) {
        keyboard->closeKeyboard();
        delete keyboard;
    }
    delete cfg;
}

