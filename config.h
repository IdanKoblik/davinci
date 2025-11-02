#ifndef CONFIG_H
#define CONFIG_H

typedef enum {
    LOWER_BUTTON,
    HIGHER_BUTTON
} button;

typedef struct {
    button penButton;
    char action[50];  } penAction;

typedef union {
    int version;
    char signature[30];
    penAction action;
} config;

int write_to_config(const char *filename, config *cfg);
int read_from_config(const char *filename, config *cfg);

#endif // CONFIG_H

