#include "tracker.h"
#include "utils.h"
#include "settings.h"
#include "db_manager.h"
#include <iostream>
#include <QDebug>
#include <iostream>

void Tracker::addEntry(std::vector<int> humidities, std::vector<int> illuminations, std::vector<int> temperatures, int duration,
                       std::set<int> species, QTemporaryDir & tmp_data_dir)
{
    EntryData entry(humidities, illuminations, temperatures, species, duration);

    // INSERT INTO DB
    DBManager db;
    int unique_row_id ( db.insert(entry) );

    // COPY DISTRIBUTION DATA
    QString unique_path(Settings::_HOME_DIR.c_str());
    unique_path += "/" + QString::number(unique_row_id);
    std::cout << "Unique path: " << unique_path.toStdString() << std::endl;
    if( ! copyRecursively(tmp_data_dir.path(), unique_path)){
        qCritical() << "Failed to create folder for new statistical entry";
        db.remove(unique_row_id);
    }
}

std::vector<EntryData> Tracker::getAllEntries()
{
    return DBManager().getAllData();
}
