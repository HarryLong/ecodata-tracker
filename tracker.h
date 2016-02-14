#ifndef TRACKER_H
#define TRACKER_H

#include <map>
#include <set>
#include <QString>
#include "entry_data.h"
#include <QTemporaryDir>

class Tracker {
public:
    static void addEntry(unsigned long timestamp, int slope, std::vector<int> humidities, std::vector<int> illuminations, std::vector<int> temperatures, int duration,
                         std::set<int> species, QTemporaryDir & tmp_data_dir);

    static bool contains(const EntryData & entry_data);

    static std::vector<EntryData> getAllEntries();
};

#endif
