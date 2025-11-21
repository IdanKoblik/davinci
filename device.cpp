#include "device.h"

#include <linux/input-event-codes.h>
#include <stdio.h>
#include <dirent.h>
#include <linux/input.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>

/**
 * BTN_STYLUS - LOWER BUTTON
 * BTN_STYLUS2 - HIGHER BUTTON
 */

int getDevice() {
    char *path = "/dev/input/";

    dirent *entry;
    DIR *dir = opendir(path);
    if (dir == nullptr) {
        perror("Error opening directory\n");
        return -1;
    }

    while ((entry = readdir(dir)) != nullptr) {
        const char *prefix = "event";
        if (strncmp(prefix, entry->d_name, strlen(prefix)) != 0)
            continue;

        const char *suffix = entry->d_name + strlen(prefix);
        if (*suffix == '\0')
            continue;

        if (*suffix && strspn(suffix, "01234567891") != strlen(suffix))
            continue;

        char *file;
        if (asprintf(&file, "%s%s", path, entry->d_name) == -1)
            continue;

        const int fd = open(file, O_RDONLY);
        if (fd < 0)
            continue;

        input_id id;
        if (ioctl(fd, EVIOCGID, &id) == 0) {
            if (id.vendor == VENDOR_ID) {
                closedir(dir);
                return fd;
            }
        }

        free(file);
        close(fd);
    }

    closedir(dir);
    return -1;
}
