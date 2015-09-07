#ifndef TRACKER_H
#define TRACKER_H

#include <map>
#include <set>
#include <QString>
#include "entry_data.h"
#include <QTemporaryDir>

class Tracker {
public:
    static void addEntry(std::vector<int> humidities, std::vector<int> illuminations, std::vector<int> temperatures, int duration,
                         std::set<int> species, QTemporaryDir & tmp_data_dir);

    static std::vector<EntryData> getAllEntries();
};

#endif
