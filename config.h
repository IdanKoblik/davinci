#ifndef DAVINCI_CONFIG_H
#define DAVINCI_CONFIG_H

#include <string>
#include <vector>

#define FILE_NAME "davinci.ini"

enum class StylusButton {
    BTN_1,
    BTN_2,
    UNKNOWN
};

class ButtonShortcut {
private:
   StylusButton button;
   std::string shortcut;

public:
   ButtonShortcut(StylusButton btn = StylusButton::UNKNOWN, std::string sc = "")
      : button(btn), shortcut(std::move(sc)) {}

   static std::string buttonToString(StylusButton button);
   static StylusButton stringToButton(const std::string &str);

   const StylusButton getButton(void) const;
   const std::string getShortcut(void) const;
};

class AppMapping {
private:
   std::string appName;
   std::vector<ButtonShortcut> shortcuts{2};

public:
   AppMapping(const std::string& name = "") : appName(name) {}

    const std::string& getAppName(void) const;
    const std::vector<ButtonShortcut>& getShortcuts(void) const;
    void addShortcut(StylusButton button, const std::string& shortcut);
    void setAppName(const std::string& name);
};

class Config {
private:
   std::string vendorId;
   std::vector<AppMapping> apps;

public:
   void writeConfig(void) const;
   void readConfig(const std::string& path);

    const std::string& getVendorId() const;
    void setVendorId(const std::string& vendor);

    const std::vector<AppMapping>& getApps() const;
    void addAppMapping(const AppMapping& mapping);
};

#endif // DAVINCI_CONFIG_H
