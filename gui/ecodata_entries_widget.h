#ifndef ECODATA_ENTRIES_WIDGET_H
#define ECODATA_ENTRIES_WIDGET_H

#include <QTableWidget>
#include <vector>
#include "../entry_data.h"

/***************
 * DATA WIDGET *
 ***************/
class EcodataEntriesWidget : public QTableWidget
{
Q_OBJECT
public:

    EcodataEntriesWidget();
    ~EcodataEntriesWidget();

    EntryData getEntryAtRow(int);
    QString getDirOfSelectedEntry();

signals:
    void row_selected(int);

public slots:
    void clear();
    void refresh();

private slots:
    void horizontal_header_pressed(int idx);
    void vertical_header_pressed(int idx);
    void cell_pressed(int row, int column);
    void sort();

private:
    enum class SortOrder : int{
        Ascending = 0,
        Descending
    } m_sort_order;

    class Columns{
    public:
        Columns();
        ~Columns();
        int timestamp;
        int species;
        int dir;
        int duration;
        int slope;
        int humidities[12];
        int illuminations[12];
        int temperatures[12];
        int column_count;
    };
    Columns m_columns;
    QTableWidgetItem * generate_read_only_cell(QString p_cell_content);
    std::vector<EntryData> m_data;
};

#endif // ECODATA_ENTRIES_WIDGET_H
