#include "db_manager.h"
#include "utils.h"
#include "settings.h"

#include <QString>
#include <iostream>
#include <QStringList>
#include <QDebug>

/***********
 * COLUMNS *
 ***********/
DatabaseSchema::Columns::Columns()
{
    int index(0);
    // ID
    {
        std::vector<std::string> columns;
        columns.push_back("_id");
        m_columns[ColumnNames::_ID] = columns;
    }

    // SPECIES
    {
        std::vector<std::string> columns;
        columns.push_back("species");
        m_columns[ColumnNames::_SPECIES] = columns;
    }

    // DURATION
    {
        std::vector<std::string> columns;
        columns.push_back("duration");
        m_columns[ColumnNames::_DURATION] = columns;
    }

    // DURATION
    {
        std::vector<std::string> columns;
        columns.push_back("slope");
        m_columns[ColumnNames::_SLOPE] = columns;
    }

    // HUMIDITIES
    {
        std::vector<std::string> columns;
        for(int i(0); i <12; i++)
        {
            std::string name("humidity_");
            name += std::to_string(i);
            columns.push_back(name);
        }
        m_columns[ColumnNames::_HUMIDITIES] = columns;
    }
    // ILLUMINATION
    {
        std::vector<std::string> columns;
        for(int i(0); i <12; i++)
        {
            std::string name("illumination_");
            name += std::to_string(i);
            columns.push_back(name);
        }
        m_columns[ColumnNames::_ILLUMINATIONS] = columns;
    }
    // TEMPERATURE
    {
        std::vector<std::string> columns;
        for(int i(0); i <12; i++)
        {
            std::string name("temperature_");
            name += std::to_string(i);
            columns.push_back(name);
        }
        m_columns[ColumnNames::_TEMPERATURES] = columns;
    }

    m_column_count = index;
}

DatabaseSchema::Columns::~Columns()
{

}

std::string DatabaseSchema::Columns::get(ColumnNames name, int index) const
{
    return m_columns[name].at(index);
}

/**********
 * TABLES *
 **********/
DatabaseSchema::Tables::Tables()
{
    m_tables[DatabaseSchema::Tables::TableNames::_ECODATA] = "ecodata";
}

DatabaseSchema::Tables::~Tables()
{

}

std::string DatabaseSchema::Tables::operator[](DatabaseSchema::Tables::TableNames name) const
{
    return m_tables[name];
}

/*******************
 * DATABASE SCHEMA *
 *******************/
DatabaseSchema::DatabaseSchema()
{
    // Build db creation code
    db_creation_code = "CREATE TABLE IF NOT EXISTS " + tables[Tables::_ECODATA] + "( " +
                       columns.get(Columns::_ID) + " INTEGER PRIMARY KEY AUTOINCREMENT," +
                       columns.get(Columns::_SPECIES) + " INTEGER NOT NULL," +
                       columns.get(Columns::_DURATION) + " TEXT NOT NULL,";
    // SLOPE
    db_creation_code += columns.get(Columns::_SLOPE) + " INT NOT NULL,";

    // Humidity
    for(int i(0); i < 12; i++)
        db_creation_code += columns.get(Columns::_HUMIDITIES, i) + " INTEGER NOT NULL,";

    // Illumination
    for(int i(0); i < 12; i++)
        db_creation_code += columns.get(Columns::_ILLUMINATIONS, i) + " INTEGER NOT NULL,";

    // Temperature
    for(int i(0); i < 12; i++)
    {
        db_creation_code += columns.get(Columns::_TEMPERATURES, i) + " INTEGER NOT NULL";
        if(i != 11)
            db_creation_code += ",";
    }

    db_creation_code +=  ");";
}

DatabaseSchema::~DatabaseSchema()
{

}

//-----------------------------
const DatabaseSchema DBManager::_SCHEMA = DatabaseSchema();
DBManager::DBManager()
{
    init();
}

DBManager::~DBManager()
{

}

void DBManager::build_prepared_statements()
{
    /***********************
     * INSERTION STATEMENT *
     ***********************/
    {
        std::string m_values_substatement(" VALUES(");
        m_insert_statement = "INSERT INTO " + _SCHEMA.tables[DatabaseSchema::Tables::_ECODATA] + " (";
        // SPECIES
        {
            std::string column_name(_SCHEMA.columns.get(DatabaseSchema::Columns::_SPECIES));
            m_insert_statement += column_name + ",";
            m_values_substatement += "@" + column_name + ",";
        }
        // DURATION
        {
            std::string column_name(_SCHEMA.columns.get(DatabaseSchema::Columns::_DURATION));
            m_insert_statement += column_name+ ",";
            m_values_substatement += "@" + column_name + ",";
        }
        // SLOPE
        {
            std::string column_name(_SCHEMA.columns.get(DatabaseSchema::Columns::_SLOPE));
            m_insert_statement += column_name+ ",";
            m_values_substatement += "@" + column_name + ",";
        }
        // HUMIDITY
        for(int i (0); i < 12; i++)
        {
            std::string column_name(_SCHEMA.columns.get(DatabaseSchema::Columns::_HUMIDITIES,i));
            m_insert_statement += column_name + ",";
            m_values_substatement += "@" + column_name + ",";
        }
        // ILLUMINATION
        for(int i (0); i < 12; i++)
        {
            std::string column_name(_SCHEMA.columns.get(DatabaseSchema::Columns::_ILLUMINATIONS,i));
            m_insert_statement += column_name + ",";
            m_values_substatement += "@" + column_name + ",";
        }
        // TEMPERATURE
        for(int i (0); i < 12; i++)
        {
            std::string column_name(_SCHEMA.columns.get(DatabaseSchema::Columns::_TEMPERATURES,i));
            m_insert_statement += column_name + (i != 11 ? "," : ")");
            m_values_substatement += "@" + column_name + (i != 11 ? "," : ");");
        }
        m_insert_statement += m_values_substatement;
    }
    /**********************
     * CONTAINS STATEMENT *
     **********************/
    {
        m_contains_statement = "SELECT * FROM " + _SCHEMA.tables[DatabaseSchema::Tables::_ECODATA] + " WHERE ";
        // SPECIES
        {
            std::string column_name(_SCHEMA.columns.get(DatabaseSchema::Columns::_SPECIES));
            m_contains_statement += column_name + " = @" + column_name + " AND ";
        }
        // SLOPE
        {
            std::string column_name(_SCHEMA.columns.get(DatabaseSchema::Columns::_SLOPE));
            m_contains_statement += column_name + " = @" + column_name + " AND ";
        }
        // HUMIDITY
        for(int i (0); i < 12; i++)
        {
            std::string column_name(_SCHEMA.columns.get(DatabaseSchema::Columns::_HUMIDITIES,i));
            m_contains_statement += column_name + " = @" + column_name + " AND ";
        }
        // ILLUMINATION
        for(int i (0); i < 12; i++)
        {
            std::string column_name(_SCHEMA.columns.get(DatabaseSchema::Columns::_ILLUMINATIONS,i));
            m_contains_statement += column_name + " = @" + column_name + " AND ";
        }
        // TEMPERATURE
        for(int i (0); i < 12; i++)
        {
            std::string column_name(_SCHEMA.columns.get(DatabaseSchema::Columns::_TEMPERATURES,i));
            m_contains_statement += column_name + " = @" + column_name + (i != 11 ? " AND " : "");
        }
    }
}


sqlite3 * DBManager::open_db() const
{
    sqlite3 * db;
    exit_on_error ( sqlite3_open(Settings::_DB_PATH.c_str(), &db), __LINE__, "" );
    exit_on_error( sqlite3_exec(db, "PRAGMA foreign_keys = ON;", 0, 0, 0), __LINE__);
    return db;
}

void DBManager::init()
{
    build_prepared_statements();

    char *error_msg = 0;
    sqlite3 * db (open_db());

    // Specie Table
    int rc (sqlite3_exec(db, _SCHEMA.db_creation_code.c_str(), NULL, 0, &error_msg));
    exit_on_error ( rc, __LINE__, error_msg );

    sqlite3_close(db);

    std::cout << "All database tables created successfully!" << std::endl;
}

void DBManager::bind_text(sqlite3_stmt * statement, const std::string & column_name, const std::string & value) const
{
    std::string column_bind_name = "@" + column_name;
    int parameterIndex( sqlite3_bind_parameter_index(statement, column_bind_name.c_str()) );
    if(parameterIndex == 0)
    {
        qCritical() << "Failed to get column index!";
        exit(1);
    }

    exit_on_error(sqlite3_bind_text(statement, parameterIndex, value.c_str(), -1, NULL), __LINE__);
}

void DBManager::bind_int(sqlite3_stmt * statement, const std::string & column_name, int value) const
{
    std::string column_bind_name = "@" + column_name;
    int parameterIndex( sqlite3_bind_parameter_index(statement, column_bind_name.c_str()) );
    if(parameterIndex == 0)
    {
        qCritical() << "Failed to get column index!";
        exit(1);
    }

    exit_on_error(sqlite3_bind_int(statement, parameterIndex, value), __LINE__);
}

/**********
 * INSERT *
 **********/
int DBManager::insert(const EntryData & data) const
{
    // Check if already present --> overwrite directory
    {
        int dir(-1);
        if(contains(data, dir))
            return dir;
    }

    sqlite3 * db (open_db());
    char *error_msg = 0;
    sqlite3_stmt * statement;

    // Prepare the statement
    exit_on_error(sqlite3_prepare_v2(db, m_insert_statement.c_str(),-1/*null-terminated*/,&statement,NULL), __LINE__);

    /***********
     * BINDING *
     ***********/
    // SPECIES
    bind_text(statement, _SCHEMA.columns.get(DatabaseSchema::Columns::_SPECIES), set_to_string(data.species));
    // DURATION
    bind_int(statement, _SCHEMA.columns.get(DatabaseSchema::Columns::_DURATION), data.duration);
    // SLOPE
    bind_int(statement, _SCHEMA.columns.get(DatabaseSchema::Columns::_SLOPE), data.slope);
    // HUMIDITY & TEMPERATURE & ILLUMINATION
    for(int i (0); i < 12; i++)
    {
        // Humidity
        bind_int(statement, _SCHEMA.columns.get(DatabaseSchema::Columns::_HUMIDITIES,i), data.humidities[i]);
        // Illumination
        bind_int(statement, _SCHEMA.columns.get(DatabaseSchema::Columns::_ILLUMINATIONS,i), data.illuminations[i]);
        // Temperature
        bind_int(statement, _SCHEMA.columns.get(DatabaseSchema::Columns::_TEMPERATURES,i), data.temperatures[i]);
    }

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
std::vector<EntryData> DBManager::getAllData() const
{
    sqlite3 * db (open_db());
    char *error_msg = 0;
    sqlite3_stmt * statement;

    std::string sql = "SELECT * FROM " + _SCHEMA.tables[DatabaseSchema::Tables::_ECODATA] + ";";

    // Prepare the statement
    exit_on_error(sqlite3_prepare_v2(db, sql.c_str(),-1/*null-terminated*/,&statement,NULL), __LINE__);

    std::vector<EntryData> ret;
    while(sqlite3_step(statement) == SQLITE_ROW)
    {
        EntryData row_data;
        for(int c (0); c < sqlite3_column_count(statement); c++)
        {
            std::string column_name(sqlite3_column_name(statement, c));
            if(column_name == _SCHEMA.columns.get(DatabaseSchema::Columns::_ID))
            {
                row_data.dir = sqlite3_column_int(statement,c);
            }
            else if(column_name == _SCHEMA.columns.get(DatabaseSchema::Columns::_SPECIES))
            {
                row_data.species = string_to_set(reinterpret_cast<const char*>(sqlite3_column_text(statement,c)));
            }
            else if(column_name == _SCHEMA.columns.get(DatabaseSchema::Columns::_DURATION))
            {
                row_data.duration = sqlite3_column_int(statement,c);
            }
            else if(column_name == _SCHEMA.columns.get(DatabaseSchema::Columns::_SLOPE))
            {
                row_data.slope = sqlite3_column_int(statement,c);
            }
            else
            {
                for(int i(0); i < 12; i++)
                {
                    if(column_name == _SCHEMA.columns.get(DatabaseSchema::Columns::_HUMIDITIES,i))
                    {
                        row_data.humidities[i] = sqlite3_column_int(statement,c);
                        break;
                    }
                    else if(column_name == _SCHEMA.columns.get(DatabaseSchema::Columns::_ILLUMINATIONS,i))
                    {
                        row_data.illuminations[i] = sqlite3_column_int(statement,c);
                        break;
                    }
                    else if(column_name == _SCHEMA.columns.get(DatabaseSchema::Columns::_TEMPERATURES,i))
                    {
                        row_data.temperatures[i] = sqlite3_column_int(statement,c);
                        break;
                    }
                }
            }
        }
        ret.push_back(row_data);
    }
    return ret;
}


bool DBManager::contains(const EntryData & entry, int & dir_name) const
{
    sqlite3 * db (open_db());
    char *error_msg = 0;
    sqlite3_stmt * statement;

    // Prepare the statement
    exit_on_error(sqlite3_prepare_v2(db, m_contains_statement.c_str(),-1/*null-terminated*/,&statement,NULL), __LINE__);

    /***********
     * BINDING *
     ***********/
    // SPECIES
    bind_text(statement, _SCHEMA.columns.get(DatabaseSchema::Columns::_SPECIES), set_to_string(entry.species));
    // SLOPE
    bind_int(statement, _SCHEMA.columns.get(DatabaseSchema::Columns::_SLOPE), entry.slope);
    // HUMIDITY & TEMPERATURE & ILLUMINATION
    for(int i (0); i < 12; i++)
    {
        // Humidity
        bind_int(statement, _SCHEMA.columns.get(DatabaseSchema::Columns::_HUMIDITIES,i), entry.humidities[i]);
        // Illumination
        bind_int(statement, _SCHEMA.columns.get(DatabaseSchema::Columns::_ILLUMINATIONS,i), entry.illuminations[i]);
        // Temperature
        bind_int(statement, _SCHEMA.columns.get(DatabaseSchema::Columns::_TEMPERATURES,i), entry.temperatures[i]);
    }

    dir_name = -1;
    while(sqlite3_step(statement) == SQLITE_ROW)
    {
        for(int c (0); c < sqlite3_column_count(statement); c++)
        {
            std::string column_name(sqlite3_column_name(statement, c));
            if(column_name == _SCHEMA.columns.get(DatabaseSchema::Columns::_ID))
            {
                dir_name = sqlite3_column_int(statement,c);
            }
        }
    }

    // finalise the statement
    sqlite3_finalize(statement);
    sqlite3_close(db);

    return dir_name != -1;
}

/**********
 * DELETE *
 **********/
void DBManager::remove(int id) const
{
    sqlite3 * db (open_db());
    char *error_msg = 0;
    sqlite3_stmt * statement;

    std::string sql = "DELETE FROM " + _SCHEMA.tables[DatabaseSchema::Tables::_ECODATA];
    sql += " WHERE " + _SCHEMA.columns.get(DatabaseSchema::Columns::_ID) + " = " + std::to_string(id) + ";";

    // Prepare the statement
    exit_on_error(sqlite3_prepare_v2(db, sql.c_str(),-1/*null-terminated*/,&statement,NULL), __LINE__);
    // Commit
    exit_on_error(sqlite3_step(statement), __LINE__);
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
