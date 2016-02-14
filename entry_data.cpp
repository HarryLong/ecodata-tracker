#include "entry_data.h"


EntryData::EntryData() : slope(0)
{
    for(int i(0); i < 12; i++)
    {
        humidities.push_back(0);
        illuminations.push_back(0);
        temperatures.push_back(0);
    }
}

EntryData::EntryData(unsigned long timestamp, int slope, std::vector<int> humidities, std::vector<int> illuminations, std::vector<int> temperatures, std::set<int> species, int duration, int dir) :
        timestamp(timestamp), slope(slope), humidities(humidities), illuminations(illuminations), temperatures(temperatures), species(species),
        duration(duration), dir(dir)
{

}

EntryData::~EntryData()
{

}
