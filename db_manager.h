#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#include "entry_data.h"

#include <sqlite3.h>
#include <map>
#include <set>
#include <string>
#include <vector>

struct Column{
    int index;
    std::string name;

    Column(int index, std::string name) : index(index), name(name) {}
};

/*********************
 * GENERIC CONSTANTS *
 *********************/
static const Column column_id = Column(0,"_id");

/*****************
 * DATA TABLE *
 *****************/
static const std::string ecodata_table_name = "ecodata";
// Humidity
static const Column ecodata_table_column_min_humidity = Column(1,"min_humidity");
static const Column ecodata_table_column_max_humidity = Column(2,"max_humidity");
// Illumination
static const Column ecodata_table_column_min_illumination = Column(3,"min_illumination");
static const Column ecodata_table_column_max_illumination = Column(4,"max_illumination");
// Temperature
static const Column ecodata_table_column_min_temp = Column(5,"min_temp");
static const Column ecodata_table_column_max_temp = Column(6,"max_temp");
// Duration
static const Column ecodata_table_column_duration = Column(7,"duration");
// Species
static const Column ecodata_table_column_species = Column(8,"species");


static const std::string ecodata_table_creation_code =
                "CREATE TABLE IF NOT EXISTS " + ecodata_table_name + "( " +
                                                column_id.name + " INTEGER PRIMARY KEY AUTOINCREMENT," +
                                                ecodata_table_column_min_humidity.name + " INTEGER NOT NULL," +
                                                ecodata_table_column_max_humidity.name + " INTEGER NOT NULL," +
                                                ecodata_table_column_min_illumination.name + " INTEGER NOT NULL," +
                                                ecodata_table_column_max_illumination.name + " INTEGER NOT NULL," +
                                                ecodata_table_column_min_temp.name + " INTEGER NOT NULL," +
                                                ecodata_table_column_max_temp.name + " INTEGER NOT NULL," +
                                                ecodata_table_column_duration.name + " INTEGER NOT NULL," +
                                                ecodata_table_column_species.name + " TEXT NOT NULL);";

class DBManager {
public:
    DBManager();
    ~DBManager();

    /****************************
     * OPEN DATABASE CONNECTION *
     ****************************/
    sqlite3 * open_db() const;
    void init() const;

    /**********
     * INSERT *
     **********/
    int insert(const EntryData & data) const;

    /*********
     * QUERY *
     *********/
    std::vector<EntryData> getData(const std::pair<int,int> humidity, const std::pair<int,int> illumination, const std::pair<int,int> temperature,
                                 const std::set<int> species) const;
    std::vector<EntryData> getAllData() const;

    /**********
     * DELETE *
     **********/
    void remove() const; // TODO: Whats the best reference for removal?

private:
    void exit_on_error(int p_code, int p_line,  char * p_error_msg = NULL) const;

private:

};

#endif // DB_MANAGER_H
