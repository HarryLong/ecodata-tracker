#ifndef ROWDATA_H
#define ROWDATA_H

#include <set>
#include <map>

struct RowData{
    std::pair<int,int> humidity;
    std::pair<int,int> illumination;
    std::pair<int,int> temperature;
    std::set<int> species;
    int duration;
    int dir;
};

#endif //ROWDATA_H
