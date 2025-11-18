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
    std::string getName(void) const;
    int getFd(void) const;

    int createKeyboard(std::string name);
    bool closeKeyboard();
    bool sendKey(int code, int value);
    bool sendSyn(void);
};

#endif //DAVINCI_KEYBOARD_H
