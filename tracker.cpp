#include "tracker.h"
#include "utils.h"
#include "settings.h"
#include "db_manager.h"
#include <iostream>
#include <QDebug>
#include <iostream>

void Tracker::addEntry(int slope, std::vector<int> humidities, std::vector<int> illuminations, std::vector<int> temperatures, int duration,
                       std::set<int> species, QTemporaryDir & tmp_data_dir)
{

    EntryData entry(slope, humidities, illuminations, temperatures, species, duration);

    // First check if this simulation is already in the database

    // INSERT INTO DB
    DBManager db;
    int unique_row_id ( db.insert(entry) );

    // COPY DISTRIBUTION DATA
    QString unique_path(Settings::_HOME_DIR.c_str());
    unique_path += "/" + QString::number(unique_row_id);
    std::cout << "Unique path: " << unique_path.toStdString() << std::endl;

    // First remove target directory if it already exists
    if(!removeRecursively(unique_path))
    {
        qCritical() << "Failed to remove existing destination folder";
        db.remove(unique_row_id);
    }

    if( ! copyRecursively(tmp_data_dir.path(), unique_path)){
        qCritical() << "Failed to create folder for new statistical entry";
        db.remove(unique_row_id);
    }
}

bool Tracker::contains(const EntryData & entry_data)
{
    int dummy_dir;
    return DBManager().contains(entry_data, dummy_dir);
}

std::vector<EntryData> Tracker::getAllEntries()
{
    return DBManager().getAllData();
}
