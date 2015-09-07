#include "tracker.h"
#include "utils.h"
#include "settings.h"

#include <iostream>

void Tracker::setNewData(const std::pair<int,int> humidity, const std::pair<int,int> illumination, const std::pair<int,int> temperature, const int duration,
                         const std::set<int> species, const QTemporaryDir & tmp_data_dir)
{
    DBManager db;
    int unique_row_id ( db.insert(to_entry_data(humidity, illumination, temperature, duration, species)) );

    QString unique_path(Settings::_HOME_DIR.c_str());
    unique_path.append(QString::number(unique_row_id));

    if( ! copyRecursively(tmp_data_dir.path(), unique_path)){
        std::cerr << "Failed to create folder " << unique_path.toStdString() << ". Aborting!" << std::endl;
    }
}

std::map<int, QString> Tracker::getActiveDirsIfAvailable(const std::pair<int,int> humidity, const std::pair<int,int> illumination, const std::pair<int,int> temperature,
                                                        const std::set<int> species)
{
    std::map<int, QString> ret;
    DBManager db;

    std::vector<EntryData> row_data(db.getData(humidity, illumination, temperature, species));

    for(EntryData data : row_data)
        ret.insert(std::pair<int, QString>(data.duration, QString::number(data.dir)));

    return ret;
}

std::vector<EntryData> Tracker::getAllData()
{
    DBManager db;
    return db.getAllData();
}

EntryData Tracker::to_entry_data(const std::pair<int,int> humidity, const std::pair<int,int> illumination, const std::pair<int,int> temperature, int duration, const std::set<int> species)
{
    EntryData row_data;
    row_data.humidity = humidity;
    row_data.illumination = illumination;
    row_data.temperature = temperature;
    row_data.duration = duration;
    row_data.species = species;
    return row_data;
}

