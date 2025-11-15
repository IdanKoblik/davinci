#ifndef DAVINCI_CONFIG_H
#define DAVINCI_CONFIG_H

#include <string.h>

#define FILE_NAME "davinci.ini"

typedef enum {
    STYLUS_BTN_1,
    STYLUS_BTN_2
} StylusButton;

typedef struct {
    StylusButton button;
    char shortcut[255];
} ButtonShortcut;

typedef struct {
    char app_name[255];
    ButtonShortcut shortcuts[2];
} AppMapping;

typedef struct {
    char vendor_id[255];
    AppMapping *apps;
    size_t size;
} Config;

static const char *button_to_string(StylusButton button) {
    switch (button) {
        case STYLUS_BTN_1: return "BTN_1";
        case STYLUS_BTN_2: return "BTN_2";
        default: return "UNKNOWN";
    }
}

static StylusButton string_to_button(const char *str) {
    if (strcmp(str, "BTN_1") == 0) return STYLUS_BTN_1;
    if (strcmp(str, "BTN_2") == 0) return STYLUS_BTN_2;
    return -1;
}


int write_config(Config *config);

int read_config(Config *config, char *path);

#endif //DAVINCI_CONFIG_H