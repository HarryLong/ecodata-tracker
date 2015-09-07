#include "db_manager.h"
#include "utils.h"
#include "settings.h"

#include <QString>
#include <iostream>
#include <QStringList>

DBManager::DBManager()
{
    init();
}

DBManager::~DBManager()
{

}


sqlite3 * DBManager::open_db() const
{
    sqlite3 * db;
    exit_on_error ( sqlite3_open(Settings::_DB_PATH.c_str(), &db), __LINE__, "" );
    exit_on_error( sqlite3_exec(db, "PRAGMA foreign_keys = ON;", 0, 0, 0), __LINE__);
    return db;
}

void DBManager::init() const
{
    char *error_msg = 0;
    sqlite3 * db (open_db());

    // Specie Table
    int rc (sqlite3_exec(db, ecodata_table_creation_code.c_str(), NULL, 0, &error_msg));
    exit_on_error ( rc, __LINE__, error_msg );

    sqlite3_close(db);

    std::cout << "All database tables created successfully!" << std::endl;
}

/**********
 * INSERT *
 **********/
int DBManager::insert(const EntryData & data) const
{
    sqlite3 * db (open_db());
    char *error_msg = 0;
    sqlite3_stmt * statement;

    const static std::string sql = "INSERT INTO " + ecodata_table_name + " (" +
            ecodata_table_column_min_humidity.name + "," +
            ecodata_table_column_max_humidity.name + "," +
            ecodata_table_column_min_illumination.name + "," +
            ecodata_table_column_max_illumination.name + "," +
            ecodata_table_column_min_temp.name + "," +
            ecodata_table_column_max_temp.name + "," +
            ecodata_table_column_duration.name + "," +
            ecodata_table_column_species.name + ")" +
            " VALUES ( ?, ?, ?, ?, ?, ?, ?, ?);";

    // Prepare the statement
    exit_on_error(sqlite3_prepare_v2(db, sql.c_str(),-1/*null-terminated*/,&statement,NULL), __LINE__);

    // Bind
    // Humidity
    exit_on_error(sqlite3_bind_int(statement, ecodata_table_column_min_humidity.index, data.humidity.first), __LINE__);
    exit_on_error(sqlite3_bind_int(statement, ecodata_table_column_max_humidity.index, data.humidity.second), __LINE__);
    // Illumination
    exit_on_error(sqlite3_bind_int(statement, ecodata_table_column_min_illumination.index, data.illumination.first), __LINE__);
    exit_on_error(sqlite3_bind_int(statement, ecodata_table_column_max_illumination.index, data.illumination.first), __LINE__);
    // Temperature
    exit_on_error(sqlite3_bind_int(statement, ecodata_table_column_min_temp.index, data.temperature.first), __LINE__);
    exit_on_error(sqlite3_bind_int(statement, ecodata_table_column_max_temp.index, data.temperature.first), __LINE__);
    // Duration
    exit_on_error(sqlite3_bind_int(statement, ecodata_table_column_duration.index, data.duration), __LINE__);
    // Species
    exit_on_error(sqlite3_bind_text(statement, ecodata_table_column_species.index, set_to_string(data.species).c_str(), -1, NULL), __LINE__);

    // Commit
    exit_on_error(sqlite3_step(statement), __LINE__);

    int inserted_row_id(sqlite3_last_insert_rowid(db));

    // finalise the statement
    sqlite3_finalize(statement);
    sqlite3_close(db);

    return inserted_row_id;
}

/*********
 * QUERY *
 *********/
std::vector<EntryData> DBManager::getData(const std::pair<int,int> humidity, const std::pair<int,int> illumination, const std::pair<int,int> temperature,
                                        const std::set<int> species) const
{
    sqlite3 * db (open_db());
    char * error_msg = 0;
    sqlite3_stmt * statement;

    std::string sql = "SELECT * FROM " + ecodata_table_name;

    std::string where_clause(" WHERE ");
    std::vector<int> int_binds;
    std::vector<std::string> string_binds;

    // Humidity
    if(humidity.first != -1)
    {
        if(!int_binds.empty())
            where_clause.append(" AND ");
        where_clause.append(ecodata_table_column_min_humidity.name + " = ?");
        int_binds.push_back(humidity.first);
    }
    if(humidity.second != -1)
    {
        if(!int_binds.empty())
            where_clause.append(" AND ");
        where_clause.append(ecodata_table_column_max_humidity.name + " = ?");
        int_binds.push_back(humidity.second);
    }

    // Illumination
    if(illumination.first != -1)
    {
        if(!int_binds.empty())
            where_clause.append(" AND ");
        where_clause.append(ecodata_table_column_min_illumination.name + " = ?");
        int_binds.push_back(illumination.first);
    }
    if(illumination.second != -1)
    {
        if(!int_binds.empty())
            where_clause.append(" AND ");
        where_clause.append(ecodata_table_column_max_illumination.name + " = ?");
        int_binds.push_back(illumination.second);
    }

    // Tempertature
    if(temperature.first != -1)
    {
        if(!int_binds.empty())
            where_clause.append(" AND ");
        where_clause.append(ecodata_table_column_min_temp.name + " = ?");
        int_binds.push_back(temperature.first);
    }
    if(temperature.second != -1)
    {
        if(!int_binds.empty())
            where_clause.append(" AND ");
        where_clause.append(ecodata_table_column_max_temp.name + " = ?");
        int_binds.push_back(temperature.second);
    }

    // Species
    if(!species.empty())
    {
        if(!int_binds.empty())
            where_clause.append(" AND ");
        where_clause.append(ecodata_table_column_species.name + " LIKE ?");
        string_binds.push_back(set_to_string(species).append("%"));
    }

    if(!int_binds.empty() || !string_binds.empty())
        sql.append(where_clause);

    sql.append(";");

    // Prepare the statement
    exit_on_error(sqlite3_prepare_v2(db, sql.c_str(),-1/*null-terminated*/,&statement,NULL), __LINE__);

    // Bind
    int bind_index(1);
    for(int bind_value : int_binds)
        exit_on_error(sqlite3_bind_int(statement, bind_index++, bind_value), __LINE__);
    for(std::string bind_value : string_binds)
        exit_on_error(sqlite3_bind_text(statement, bind_index++, bind_value.c_str(), -1, NULL), __LINE__);

    std::vector<EntryData> ret;
    while(sqlite3_step(statement) == SQLITE_ROW)
    {
        EntryData row_data;
        for(int c (0); c < sqlite3_column_count(statement); c++)
        {
            if(c == column_id.index)
                row_data.dir = sqlite3_column_int(statement,c);
            // Humidity
            else if(c == ecodata_table_column_min_humidity.index)
                row_data.humidity.first = sqlite3_column_int(statement,c);
            else if(c == ecodata_table_column_max_humidity.index)
                row_data.humidity.second = sqlite3_column_int(statement,c);
            // Illumination
            else if(c == ecodata_table_column_min_illumination.index)
                row_data.illumination.first = sqlite3_column_int(statement,c);
            else if(c == ecodata_table_column_max_illumination.index)
                row_data.illumination.second = sqlite3_column_int(statement,c);
            // Temperature
            else if(c == ecodata_table_column_min_temp.index)
                row_data.temperature.first = sqlite3_column_int(statement,c);
            else if(c == ecodata_table_column_max_temp.index)
                row_data.temperature.second = sqlite3_column_int(statement,c);
            // Duration
            else if(c == ecodata_table_column_duration.index)
                row_data.duration = sqlite3_column_int(statement,c);
            // Species
            else if(c == ecodata_table_column_species.index)
                row_data.species = string_to_set(reinterpret_cast<const char*>(sqlite3_column_text(statement,c)));
            else
            {
                std::cerr << "Unknown column: " << sqlite3_column_name(statement,c) <<
                             " in file " << __FILE__ << " and line " << __LINE__ << std::endl;
                exit(1);
            }
        }
        ret.push_back(row_data);
    }

    sqlite3_finalize(statement);
    sqlite3_close(db);
    return ret;
}

std::vector<EntryData> DBManager::getAllData() const
{
    std::pair<int,int> unfiltered(-1,-1);
    return getData(unfiltered, unfiltered, unfiltered, std::set<int>());
}

/**********
 * DELETE *
 **********/
void DBManager::remove() const
{

}


void DBManager::exit_on_error(int p_code, int p_line,  char * p_error_msg) const
{
    if(p_code != SQLITE_OK && p_code != SQLITE_DONE)
    {
        std::cerr << "Database failure!" << std::endl;
        std::cerr << "Error code: " << p_code << std::endl;
        std::cerr << "File: " << __FILE__ << std::endl;
        std::cerr << "Line: " << p_line << std::endl;
        std::cerr << "Error message:" << p_error_msg << std::endl;
        sqlite3_free(p_error_msg);
        exit(1);
    }
}
