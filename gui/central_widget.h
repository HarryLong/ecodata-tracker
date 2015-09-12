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
class DataWidget : public QTableWidget
{
public:
    DataWidget();
    ~DataWidget();

    void clear();
    void set(std::vector<EntryData> data);

private:
    class Columns{
    public:
        Columns();
        ~Columns();
        int species;
        int dir;
        int duration;
        int humidities[12];
        int illuminations[12];
        int temperatures[12];
        int column_count;
    };
    Columns m_columns;
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

public slots:
    void refresh();

private:
    void init_layout();
//    QLineEdit * create_connected_line_edit();
//    int get_int_line_edit_value(QLineEdit * line_edit);

    QPushButton * m_refresh_btn;
    DataWidget * m_data_widget;
    DBManager m_db;
};

#endif //CENTRAL_WIDGET_H
