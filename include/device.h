#ifndef DEVICE_H
#define DEVICE_H

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <limits.h>

#define ID_LEN 5

typedef struct {
    char vendorId[ID_LEN];
    char productId[ID_LEN];
} DeviceSpec;

typedef struct {
    DeviceSpec spec;

    int fd;
    char path[PATH_MAX];
} Stylus;

int findStylus(DeviceSpec spec, Stylus* stylus);

#endif // DEVICE_H
