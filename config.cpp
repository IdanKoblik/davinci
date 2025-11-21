#include "config.h"
#include "trim.h" 
#include <format>
#include <iostream>
#include <fstream>

std::string ButtonShortcut::buttonToString(StylusButton button) {
    switch (button) {
        case StylusButton::BTN_1: return "BTN_1";
        case StylusButton::BTN_2: return "BTN_2";
        default: return "UNKNOWN";
    }
}

StylusButton ButtonShortcut::stringToButton(const std::string &str) {
    if (str == "BTN_1") return StylusButton::BTN_1;
    if (str == "BTN_2") return StylusButton::BTN_2;
    return StylusButton::UNKNOWN;
}

const std::string& AppMapping::getAppName() const {
    return appName;
}

const std::vector<ButtonShortcut>& AppMapping::getShortcuts() const {
    return shortcuts;
}

void AppMapping::addShortcut(StylusButton button, const std::string& shortcut) {
    shortcuts[static_cast<int>(button)] = ButtonShortcut(button, shortcut);
}

void AppMapping::setAppName(const std::string &name) {
    appName = name;
}

const std::string& Config::getVendorId() const {
    return vendorId;
}

void Config::setVendorId(const std::string& vendor) {
    vendorId = vendor;
}

const std::vector<AppMapping>& Config::getApps() const {
    return apps;
}

void Config::addAppMapping(const AppMapping& mapping) {
    apps.push_back(mapping);
}

const StylusButton ButtonShortcut::getButton() const {
   return this->button;
}

const std::string ButtonShortcut::getShortcut() const {
   return this->shortcut;
}


void Config::writeConfig() const {
    std::ofstream file(FILE_NAME);
    if (!file.is_open()) {
        perror("Cannot open file");
        return;
    }

    std::string header = std::format("[Davinci - {}]\n\n", this->vendorId);
    file << header;

    for (const AppMapping& app : this->apps) {
        std::string appSection = std::format("[{}]\n", app.getAppName());
        file << appSection;

        for (const ButtonShortcut& shortcut : app.getShortcuts()) {
            std::string line = std::format("{} => \"{}\"\n",
                                           ButtonShortcut::buttonToString(shortcut.getButton()),
                                           shortcut.getShortcut());
            file << line;
        }
        file << std::endl;
    }

    file.close();
}

void Config::readConfig(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        perror("Cannot open file");
        return;
    }

    std::string line;
    AppMapping* currentApp = nullptr;
    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == ';' || line[0] == '#') continue;

        if (line.front() == '[' && line.back() == ']') {
            std::string section = line.substr(1, line.size() - 2);

            if (section.rfind("Davinci - ", 0) == 0) {
                this->vendorId = section.substr(10);
                currentApp = nullptr;
            } else {
                this->apps.emplace_back(AppMapping(section));
                currentApp = &this->apps.back();
            }
        } else if (currentApp) {
            size_t arrowPos = line.find("=>");
            if (arrowPos == std::string::npos) continue;

            std::string buttonStr = trim(line.substr(0, arrowPos));
            std::string shortcutStr = trim(line.substr(arrowPos + 2));

            if (!shortcutStr.empty() && shortcutStr.front() == '"' && shortcutStr.back() == '"') {
                shortcutStr = shortcutStr.substr(1, shortcutStr.size() - 2);
            }

            const StylusButton button = ButtonShortcut::stringToButton(buttonStr); 
            currentApp->addShortcut(button , shortcutStr);
        }
    }

    file.close();
}
