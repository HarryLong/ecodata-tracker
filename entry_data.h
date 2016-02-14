#ifndef ROWDATA_H
#define ROWDATA_H

#include <set>
#include <map>
#include <vector>

class EntryData{
public:
    EntryData();
    EntryData(unsigned long timestamp, int slope, std::vector<int> humidities, std::vector<int> illuminations, std::vector<int> temperatures, std::set<int> species, int duration, int dir = 0);
    ~EntryData();

    std::vector<int> humidities;
    std::vector<int> illuminations;
    std::vector<int> temperatures;
    std::set<int> species;
    int slope;
    int duration;
    int dir;
    unsigned long timestamp;
};

#endif //ROWDATA_H
