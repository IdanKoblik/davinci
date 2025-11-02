#include <linux/input-event-codes.h>
#include <stdio.h>
#include <dirent.h>
#include <linux/input.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include "config.h"

#define VENDOR_ID 0x056a

/**
 * BTN_STYLUS - LOWER BUTTON
 * BTN_STYLUS2 - HIGHER BUTTON
 */

char* get_device();

int main(void) {
	char *device = get_device();
	int fd = open(device, O_RDONLY);

	printf("%s\n", device);
	struct input_event event;
	while (1) {
		read(fd, &event, sizeof(struct input_event));
		if (event.type != EV_KEY)
			continue;

		if (event.code == BTN_STYLUS)
			printf("%s\n", (event.value) ? "Pressed" : "Released");
	}

	free(device);
	close(fd);
	return 0;
}

char* get_device() {
	const char *prefix = "event";
	char *path = "/dev/input/";

	struct dirent *entry;
	DIR *dir = opendir(path);
	if (dir == NULL ) {
		perror("Error opening directory\n");
		return "";
	}

	while ((entry = readdir(dir)) != NULL) {
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

		int fd = open(file, O_RDONLY);

		if (fd < 0)
			continue;

		struct input_id id;
		if (ioctl(fd, EVIOCGID, &id) == 0) {
			if (id.vendor == VENDOR_ID) {
				close(fd);
				closedir(dir);
				return file;
			}
		}

		free(file);
		close(fd);
	}

	closedir(dir);
	return "";
}

