#ifndef TRACKER_H
#define TRACKER_H

#include <map>
#include <set>
#include <QString>
#include "db_manager.h"
#include "entry_data.h"
#include <QTemporaryDir>

class Tracker {
public:
    static std::map<int, QString> getActiveDirsIfAvailable(const std::pair<int,int> humidity, const std::pair<int,int> illumination, const std::pair<int,int> temperature,
                                           const std::set<int> species);

    static void setNewData(const std::pair<int,int> humidity, const std::pair<int,int> illumination, const std::pair<int,int> temperature, const int duration,
                           const std::set<int> species, const QTemporaryDir & tmp_data_dir);

    static std::vector<EntryData> getAllData();

private:
    static EntryData to_entry_data(std::pair<int,int> humidity, std::pair<int,int> illumination, std::pair<int,int> temperature, int duration, std::set<int> species);
};

#endif
