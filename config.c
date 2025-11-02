#include "config.h"
#include <stdio.h>

int write_to_config(const char *filename, config *cfg) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Error opening config file for writing");
        return -1;
    }

    fwrite(cfg, sizeof(config), 1, file);
    fclose(file);
    return 0;
}

int read_from_config(const char *filename, config *cfg) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening config file for reading");
        return -1;
    }

    fread(cfg, sizeof(config), 1, file);
    fclose(file);
    return 0;
}

