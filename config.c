#include "config.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int write_config(Config *config) {
    FILE *file = fopen(FILE_NAME, "w");
    if (file == NULL) {
        perror("fopen");
        return -1;
    }

    fprintf(file, "[Davinci - %s]\n\n", config->vendor_id);
    for (size_t i = 0; i < config->size; i++) {
        AppMapping *app = &config->apps[i];
        fprintf(file, "[%s]\n", app->app_name);
        for (int j = 0; j < 2; j++) {
            fprintf(file, "%s => \"%s\"\n",
                    button_to_string(app->shortcuts[j].button),
                    app->shortcuts[j].shortcut);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    return 0;
}

int read_config(Config *config, char *path) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        perror("fopen");
        return -1;
    }

    char line[512];
    AppMapping *current_app = NULL;

    config->apps = NULL;
    config->size = 0;
    config->vendor_id[0] = '\0';

    while (fgets(line, sizeof(line), file) != NULL) {
        char *p = line;
        while (isspace((unsigned char)*p))
            p++;

        if (*p == '\0' || *p == '\n' || *p == '#')
            continue;

        if (*p == '[') {
            char *end = strchr(p, ']');
            if (!end)
                continue;

            *end = '\0';
            p++;

            if (strncmp(p, "Davinci - ", 10) == 0) {
                strcpy(config->vendor_id, p + 10);
            } else {
                config->apps = realloc(config->apps, sizeof(AppMapping) * (config->size + 1));
                current_app = &config->apps[config->size];
                memset(current_app, 0, sizeof(AppMapping));
                strcpy(current_app->app_name, p);
                config->size++;
            }
        }

        // Parse BTN => "Shortcut"
        if (current_app) {
            char *arrow = strstr(p, "=>");
            if (!arrow) continue;
            *arrow = '\0';
            char *key = p;
            char *val = arrow + 2;

            while (isspace((unsigned char)*key)) key++;
            char *key_end = key + strlen(key) - 1;
            while (key_end > key && isspace((unsigned char)*key_end)) *key_end-- = '\0';

            while (isspace((unsigned char)*val)) val++;
            if (*val == '"') val++;
            char *quote = strchr(val, '"');
            if (quote) *quote = '\0';

            StylusButton btn = string_to_button(key);
            if (btn == STYLUS_BTN_1 || btn == STYLUS_BTN_2) {
                current_app->shortcuts[btn].button = btn;
                strncpy(current_app->shortcuts[btn].shortcut, val, sizeof(current_app->shortcuts[btn].shortcut) - 1);
            }
        }
    }

    fclose(file);
    return 0;
}