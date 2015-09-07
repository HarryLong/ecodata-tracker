#ifndef CENTRAL_WIDGET_H
#define CENTRAL_WIDGET_H

#include <vector>
#include <QString>
#include <QTableWidget>
#include "../entry_data.h"
#include "../db_manager.h"

#include <QLineEdit>
#include <QPushButton>

/***************
 * DATA WIDGET *
 ***************/
enum DataColumns{
    MinHumidity = 0,
    MaxHumidity,
    MinIllumination,
    MaxIllumination,
    MinTemp,
    MaxTemp,
    Duration,
    Species,
    Dir,
    ColumnCount
};

class DataWidget : public QTableWidget
{
public:
    DataWidget();
    ~DataWidget();

    void clear();
    void set(std::vector<EntryData> data);

private:
    QTableWidgetItem * generate_read_only_cell(QString p_cell_content);
};

/******************
 * CENTRAL WIDGET *
 ******************/
class CentralWidget : public QWidget
{
    Q_OBJECT
public:
    CentralWidget();
    ~CentralWidget();

private slots:
    void refresh();

private:
    void init_layout();
    QLineEdit * create_connected_line_edit();
    int get_int_line_edit_value(QLineEdit * line_edit);

    DataWidget * m_data_widget;
    DBManager m_db;

    QLineEdit * m_min_temperature_query;
    QLineEdit * m_max_temperature_query;

    QLineEdit * m_min_humidity_query;
    QLineEdit * m_max_humidity_query;

    QLineEdit * m_min_illumination_query;
    QLineEdit * m_max_illumination_query;

    QLineEdit * m_species_query;

    QPushButton * m_refresh_btn;
};

#endif //CENTRAL_WIDGET_H
