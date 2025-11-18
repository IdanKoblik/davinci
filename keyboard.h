#ifndef DAVINCI_KEYBOARD_H
#define DAVINCI_KEYBOARD_H

#include <string>

#define VENDOR_ID 0x1234
#define PRODUCT_ID 0x5678

class Keyboard {
private:
   std::string name;
   int fd;

public:
    std::string get_name(void) const;
    int get_fd(void) const;

    int create_keyboard(std::string name);
    bool close_keyboard();
    bool send_key(int code, int value);
    bool send_syn(void);
};

#endif //DAVINCI_KEYBOARD_H
