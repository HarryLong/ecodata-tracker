#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#include "entry_data.h"

#include <sqlite3.h>
#include <map>
#include <set>
#include <string>
#include <vector>


/*******************
 * DATABASE SCHEMA *
 *******************/
class DatabaseSchema
{
public:
    DatabaseSchema();
    ~DatabaseSchema();

    class Tables{
    public:
        enum TableNames{
            _ECODATA = 0,
            _TABLE_COUNT
        };
        Tables();
        ~Tables();
        std::string operator[](TableNames name) const;
    private:
        std::string m_tables[TableNames::_TABLE_COUNT];
    };
    class Columns{
    public:
        enum ColumnNames{
            _ID,
            _TIMESTAMP,
            _SPECIES,
            _DURATION,
            _SLOPE,
            _HUMIDITIES,
            _ILLUMINATIONS,
            _TEMPERATURES,
            _COUNT
        };

        Columns();
        ~Columns();
        std::string get(ColumnNames name, int index = 0) const;
    private:
        std::vector<std::string> m_columns[ColumnNames::_COUNT];
        int m_column_count;
    };
    std::string db_creation_code;
    Tables tables;
    Columns columns;
};

/*********************
 * GENERIC CONSTANTS *
 *********************/
//static const Column column_id = Column(0,"_id");

///*****************
// * DATA TABLE *
// *****************/
//// Humidity
//static Column column_humidities[12];// = Column(1,"min_humidity");
//// Illumination
//static Column column_illuminations[12];
//// Temperature
//static Column column_temperatures[12];
//// Duration
//static const Column ecodata_table_column_duration = Column(7,"duration");
//// Species
//static const Column ecodata_table_column_species = Column(8,"species");


//static const std::string ecodata_table_creation_code =
//                "CREATE TABLE IF NOT EXISTS " + ecodata_table_name + "( " +
//                                                column_id.name + " INTEGER PRIMARY KEY AUTOINCREMENT," +
//                                                ecodata_table_column_min_humidity.name + " INTEGER NOT NULL," +
//                                                ecodata_table_column_max_humidity.name + " INTEGER NOT NULL," +
//                                                ecodata_table_column_min_illumination.name + " INTEGER NOT NULL," +
//                                                ecodata_table_column_max_illumination.name + " INTEGER NOT NULL," +
//                                                ecodata_table_column_min_temp.name + " INTEGER NOT NULL," +
//                                                ecodata_table_column_max_temp.name + " INTEGER NOT NULL," +
//                                                ecodata_table_column_duration.name + " INTEGER NOT NULL," +
//                                                ecodata_table_column_species.name + " TEXT NOT NULL);";

class DBManager {
public:
    DBManager();
    ~DBManager();

    /****************************
     * OPEN DATABASE CONNECTION *
     ****************************/
    sqlite3 * open_db() const;

    /**********
     * INSERT *
     **********/
    int insert(const EntryData & data) const;

    /*********
     * QUERY *
     *********/
    std::vector<EntryData> getAllData() const;
    bool contains(const EntryData & entry, int & dir_name) const;

    /**********
     * DELETE *
     **********/
    void remove(int id) const; // TODO: Whats the best reference for removal?

private:
    void init();
    void exit_on_error(int p_code, int p_line,  char * p_error_msg = NULL) const;
    void build_prepared_statements();
    void bind_text(sqlite3_stmt * statement, const std::string & column_name, const std::string & svalue) const;
    void bind_int(sqlite3_stmt * statement, const std::string & column_name, int value) const;
    void bind_int64(sqlite3_stmt * statement, const std::string & column_name, unsigned long value) const;

    static const DatabaseSchema _SCHEMA;
    std::string m_insert_statement;
    std::string m_contains_statement;
};

#endif // DB_MANAGER_H
