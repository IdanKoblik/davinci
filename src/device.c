#include "device.h"

#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>

#define SYS_INPUT "/sys/class/input"
#define DEV_INPUT "/dev/input"

#define BUF 64

static int read_trimmed(const char *path, char *out, size_t len) {
    FILE *f = fopen(path, "r");
    if (!f)
       return -1;

    if (!fgets(out, len, f)) {
        fclose(f);
        return -1;
    }

    fclose(f);

    out[strcspn(out, "\n")] = 0;
    return 0;
}

int findStylus(DeviceSpec spec, Stylus* stylus) {
    DIR* dir = opendir(SYS_INPUT);
    if (!dir)
       return -1;

    struct dirent* entry;
    while ((entry = readdir(dir))) {
        if (strncmp(entry->d_name, "event", ID_LEN) != 0)
           continue;

        char vendorPath[BUF];
        char productPath[BUF];

        snprintf(vendorPath, sizeof(vendorPath), SYS_INPUT"/%s/device/id/vendor", entry->d_name);
        snprintf(productPath, sizeof(productPath), SYS_INPUT"/%s/device/id/product", entry->d_name);

        char vendor[ID_LEN];
        char product[ID_LEN];

        if (read_trimmed(vendorPath, vendor, sizeof(vendor)) != 0)
            continue;

        if (read_trimmed(productPath, product, sizeof(product)) != 0)
            continue;

        if (strcasecmp(vendor, spec.vendorId) == 0 && strcasecmp(product, spec.productId) == 0) {
            stylus->spec = spec;

            snprintf(stylus->path, sizeof(stylus->path), DEV_INPUT"/%s", entry->d_name);
            stylus->fd = -1;

            closedir(dir);
            return 0;
        }
    }

    closedir(dir);
    return -1;
}
