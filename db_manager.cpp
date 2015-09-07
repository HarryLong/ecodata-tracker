#include "db_manager.h"
#include "utils.h"
#include "settings.h"

#include <QString>
#include <iostream>
#include <QStringList>

/***********
 * COLUMNS *
 ***********/
DatabaseSchema::Columns::Columns()
{
    int index(0);
    // ID
    {
        std::vector<Column> columns;
        columns.push_back(Column(index++, "_id"));
        m_columns[ColumnNames::_ID] = columns;
    }

    // SPECIES
    {
        std::vector<Column> columns;
        columns.push_back(Column(index++, "species"));
        m_columns[ColumnNames::_SPECIES] = columns;
    }

    // DURATION
    {
        std::vector<Column> columns;
        columns.push_back(Column(index++, "duration"));
        m_columns[ColumnNames::_DURATION] = columns;
    }

    // HUMIDITIES
    {
        std::vector<Column> columns;
        for(int i(0); i <12; i++)
        {
            std::string name("humidity_");
            name += std::to_string(i);
            columns.push_back(Column(index++, name));
        }
        m_columns[ColumnNames::_HUMIDITIES] = columns;
    }
    // ILLUMINATION
    {
        std::vector<Column> columns;
        for(int i(0); i <12; i++)
        {
            std::string name("illumination_");
            name += std::to_string(i);
            columns.push_back(Column(index++, name));
        }
        m_columns[ColumnNames::_ILLUMINATIONS] = columns;
    }
    // TEMPERATURE
    {
        std::vector<Column> columns;
        for(int i(0); i <12; i++)
        {
            std::string name("temperature_");
            name += std::to_string(i);
            columns.push_back(Column(index++, name));
        }
        m_columns[ColumnNames::_TEMPERATURES] = columns;
    }

    m_column_count = index-1; // Remove the last increment
}

DatabaseSchema::Columns::~Columns()
{

}

Column DatabaseSchema::Columns::get(ColumnNames name, int index) const
{
    return m_columns[name].at(index);
}

int DatabaseSchema::Columns::columnCount() const
{
    return m_column_count;
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
                       columns.get(Columns::_ID).name + " INTEGER PRIMARY KEY AUTOINCREMENT," +
                       columns.get(Columns::_SPECIES).name + " INTEGER NOT NULL," +
                       columns.get(Columns::_DURATION).name + " TEXT NOT NULL,";
    // Humidity
    for(int i(0); i < 12; i++)
        db_creation_code += columns.get(Columns::_HUMIDITIES, i).name + " INTEGER NOT NULL,";

    // Illumination
    for(int i(0); i < 12; i++)
        db_creation_code += columns.get(Columns::_ILLUMINATIONS, i).name + " INTEGER NOT NULL,";

    // Temperature
    for(int i(0); i < 12; i++)
    {
        db_creation_code += columns.get(Columns::_TEMPERATURES, i).name + " INTEGER NOT NULL";
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

void DBManager::build_insert_statement()
{
    m_insert_statement = "INSERT INTO " + _SCHEMA.tables[DatabaseSchema::Tables::_ECODATA] + " (";
    // SPECIES
    m_insert_statement += _SCHEMA.columns.get(DatabaseSchema::Columns::_SPECIES).name + ",";
    // DURATION
    m_insert_statement += _SCHEMA.columns.get(DatabaseSchema::Columns::_DURATION).name + ",";
    // HUMIDITY
    for(int i (0); i < 12; i++)
        m_insert_statement += _SCHEMA.columns.get(DatabaseSchema::Columns::_HUMIDITIES,i).name + ",";
    // ILLUMINATION
    for(int i (0); i < 12; i++)
        m_insert_statement += _SCHEMA.columns.get(DatabaseSchema::Columns::_ILLUMINATIONS,i).name + ",";
    // TEMPERATURE
    for(int i (0); i < 12; i++)
    {
        m_insert_statement += _SCHEMA.columns.get(DatabaseSchema::Columns::_TEMPERATURES,i).name;
        if(i != 11)
            m_insert_statement += ",";
    }
    m_insert_statement += " VALUES(";
    for(int i(0); i < _SCHEMA.columns.columnCount()-1 ; i++) // We
    {
        if(i > 0)
            m_insert_statement += ",";
        m_insert_statement += "?";
    }
    m_insert_statement += ");";
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
    build_insert_statement();

    char *error_msg = 0;
    sqlite3 * db (open_db());

    // Specie Table
    int rc (sqlite3_exec(db, _SCHEMA.db_creation_code.c_str(), NULL, 0, &error_msg));
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

    // Prepare the statement
    exit_on_error(sqlite3_prepare_v2(db, m_insert_statement.c_str(),-1/*null-terminated*/,&statement,NULL), __LINE__);

    /***********
     * BINDING *
     ***********/
    // SPECIES
    exit_on_error(sqlite3_bind_text(statement, _SCHEMA.columns.get(DatabaseSchema::Columns::_SPECIES).index-1, set_to_string(data.species).c_str(), -1, NULL), __LINE__);
    // DURATION
    exit_on_error(sqlite3_bind_int(statement, _SCHEMA.columns.get(DatabaseSchema::Columns::_DURATION).index-1, data.duration), __LINE__);
    // HUMIDITY & TEMPERATURE & ILLUMINATION
    for(int i (0); i < 12; i++)
    {
        // Humidity
        exit_on_error(sqlite3_bind_int(statement, _SCHEMA.columns.get(DatabaseSchema::Columns::_HUMIDITIES).index-1, data.humidities[i]), __LINE__);
        // Illumination
        exit_on_error(sqlite3_bind_int(statement, _SCHEMA.columns.get(DatabaseSchema::Columns::_ILLUMINATIONS).index-1, data.illuminations[i]), __LINE__);
        // Temperature
        exit_on_error(sqlite3_bind_int(statement, _SCHEMA.columns.get(DatabaseSchema::Columns::_TEMPERATURES).index-1, data.temperatures[i]), __LINE__);
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
            if(c == _SCHEMA.columns.get(DatabaseSchema::Columns::_ID).index)
            {
                row_data.dir = sqlite3_column_int(statement,c);
            }
            else if(c == _SCHEMA.columns.get(DatabaseSchema::Columns::_SPECIES).index)
            {
                row_data.species = string_to_set(reinterpret_cast<const char*>(sqlite3_column_text(statement,c)));
            }
            else if(c == _SCHEMA.columns.get(DatabaseSchema::Columns::_DURATION).index)
            {
                row_data.duration = sqlite3_column_int(statement,c);
            }
            else
            {
                for(int i(0); i < 12; i++)
                {
                    if(c == _SCHEMA.columns.get(DatabaseSchema::Columns::_HUMIDITIES,i).index)
                    {
                        row_data.humidities[i] = sqlite3_column_int(statement,c);
                        break;
                    }
                    else if(c == _SCHEMA.columns.get(DatabaseSchema::Columns::_ILLUMINATIONS,i).index)
                    {
                        row_data.illuminations[i] = sqlite3_column_int(statement,c);
                        break;
                    }
                    else if(c == _SCHEMA.columns.get(DatabaseSchema::Columns::_TEMPERATURES,i).index)
                    {
                        row_data.illuminations[i] = sqlite3_column_int(statement,c);
                        break;
                    }
                }
            }
        }
        ret.push_back(row_data);
    }
    return ret;
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
