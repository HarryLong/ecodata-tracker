#include "tracker.h"
#include "settings.h"
#include <iostream>
#include "utils.h"

void Tracker::setNewData(const std::pair<int,int> humidity, const std::pair<int,int> illumination, const std::pair<int,int> temperature, const int duration,
                         const std::set<int> species, const QTemporaryDir & tmp_data_dir)
{
    DBManager db;
    int unique_row_id ( db.insert(to_row_data(humidity, illumination, temperature, duration, species)) );

    QString unique_path(MAIN_DIR);
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

    std::vector<RowData> row_data(db.getData(humidity, illumination, temperature, species));

    for(RowData data : row_data)
        ret.insert(std::pair<int, QString>(data.duration, QString::number(data.dir)));

    return ret;
}

void Tracker::removeData() //TODO: Figure out how to reference it
{

}

std::vector<RowData> Tracker::getAllData()
{
    DBManager db;
    return db.getAllData();
}

RowData Tracker::to_row_data(const std::pair<int,int> humidity, const std::pair<int,int> illumination, const std::pair<int,int> temperature, int duration, const std::set<int> species)
{
    RowData row_data;
    row_data.humidity = humidity;
    row_data.illumination = illumination;
    row_data.temperature = temperature;
    row_data.duration = duration;
    row_data.species = species;
    return row_data;
}

