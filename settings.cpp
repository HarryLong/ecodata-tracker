#include "settings.h"
#include "utils.h"

#include <QDebug>
#include <fstream>

const std::string SettingsFileTags::_HOME = "home_dir=";


const std::string Settings::_HOME_DIR = Settings::load_home_dir();
const std::string Settings::_DB_PATH = Settings::_HOME_DIR + "/ECODATA.db";

std::string Settings::load_home_dir()
{
    // First try local configuration location
    std::string config_file;

    if(file_exists("ecotracker.conf"))
        config_file = "ecotracker.conf";
    else if(file_exists("/usr/local/etc/ecotracker/ecotracker.conf"))
        config_file = "/usr/local/etc/ecotracker/ecotracker.conf";
    else if(file_exists("/etc/ecotracker/ecotracker.conf"))
        config_file = "/etc/ecotracker/ecotracker.conf";
    else
    {
        qCritical() << "Could not find configuration file!";
        exit(1);
    }

    std::ifstream config_reader(config_file, std::ios_base::in);
    if(config_reader.fail())
    {
        qCritical() << "Failed to read config file: " << config_file.c_str();
        exit(1);
    }

    std::string line;
    while (std::getline(config_reader, line))
    {
        if(line.find(SettingsFileTags::_HOME) != std::string::npos)
        {
            line.replace(0, SettingsFileTags::_HOME.length(), "");
            if(file_exists(line))
            {
                return line;
            }
            qCritical() << "Database file not found: " << line.c_str();
            exit(1);
        }
        else
        {
            qCritical() << "Unknown config parameter: " << line.c_str();
        }
    }
    qCritical() << "Could not find database location descriptor in configuration file: " << config_file.c_str();
    exit(1);
}
