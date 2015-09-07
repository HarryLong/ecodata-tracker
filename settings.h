#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

class SettingsFileTags{
public:
    static const std::string _HOME;
};

class Settings{
public:
    static const std::string _HOME_DIR;
    static const std::string _DB_PATH;

    static std::string load_home_dir();
};

#endif // SETTINGS_H

