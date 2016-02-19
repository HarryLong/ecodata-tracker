#include "ecodata_entries_widget.h"
#include <QHeaderView>
#include <time.h>
#include "../utils.h"
#include "../db_manager.h"
#include "../settings.h"

EcodataEntriesWidget::Columns::Columns()
{
    int i(0);
    timestamp = i++;
    species = i++;
    dir = i++;
    duration = i++;
    slope = i++;
    for(int ii(0); ii < 12; ii++)
        humidities[ii] = i++;
    for(int ii(0); ii < 12; ii++)
        illuminations[ii] = i++;
    for(int ii(0); ii < 12; ii++)
        temperatures[ii] = i++;
    column_count = i-1;
}

EcodataEntriesWidget::Columns::~Columns()
{

}

/***************
 * DATA WIDGET *
 ***************/
EcodataEntriesWidget::EcodataEntriesWidget() : m_sort_order(SortOrder::Descending)
{
    setSelectionMode(QAbstractItemView::SelectionMode::ContiguousSelection);

    setColumnCount(m_columns.column_count);

    setHorizontalHeaderItem(m_columns.timestamp, new QTableWidgetItem("Date"));
    // Species
    setHorizontalHeaderItem(m_columns.species, new QTableWidgetItem("Species"));
    // Dir
    setHorizontalHeaderItem(m_columns.dir, new QTableWidgetItem("Dir"));
    // Duration
    setHorizontalHeaderItem(m_columns.duration, new QTableWidgetItem("Duration"));
    // Slope
    setHorizontalHeaderItem(m_columns.slope, new QTableWidgetItem("Slope"));
    for(int i(0); i < 12; i++)
    {
        // HUMIDITY
        {
            QString header("Humidity [");
            header += QString::number(i) + "]";
            setHorizontalHeaderItem(m_columns.humidities[i], new QTableWidgetItem(header));
        }
        // ILLUMINATION
        {
            QString header("Illumination [");
            header += QString::number(i) + "]";
            setHorizontalHeaderItem(m_columns.illuminations[i], new QTableWidgetItem(header));
        }
        // TEMPERATURE
        {
            QString header("Temperature [");
            header += QString::number(i) + "]";
            setHorizontalHeaderItem(m_columns.temperatures[i], new QTableWidgetItem(header));
        }
    }
//    itemPressed(QTableWidgetItem * item)
//    connect(horizontalHeaderItem(0), SIGNAL())
    connect(this->horizontalHeader(), SIGNAL(sectionPressed(int)), this, SLOT(horizontal_header_pressed(int)));
    connect(this->verticalHeader(), SIGNAL(sectionPressed(int)), this, SLOT(vertical_header_pressed(int)));

    connect(this, SIGNAL(cellPressed(int,int)), this, SLOT(cell_pressed(int,int)));

    refresh();
}

EcodataEntriesWidget::~EcodataEntriesWidget()
{

}

QString EcodataEntriesWidget::getDirOfSelectedEntry()
{
    return QString::fromStdString(Settings::_HOME_DIR).append("/").append(QString::number(m_data.at(currentRow()).dir));
}

EntryData EcodataEntriesWidget::getEntryAtRow(int row_id)
{
    return m_data.at(row_id);
}

void EcodataEntriesWidget::refresh()
{
    m_data = DBManager().getAllData();
    sort();
}

void EcodataEntriesWidget::horizontal_header_pressed(int idx)
{
    if(idx == 0)
    {
        m_sort_order = static_cast<SortOrder>(((static_cast<int>(m_sort_order)+1)%2));
        sort();
    }
}

void EcodataEntriesWidget::vertical_header_pressed(int idx)
{
    emit row_selected(idx);
}

#include <QDebug>
void EcodataEntriesWidget::cell_pressed(int row, int column)
{
    selectRow(row);
    emit row_selected(row);
}

QTableWidgetItem * EcodataEntriesWidget::generate_read_only_cell(QString p_cell_content)
{
    QTableWidgetItem * cell = new QTableWidgetItem(p_cell_content);
    cell->setFlags(cell->flags() ^ Qt::ItemIsEditable);

    return cell;
}

void EcodataEntriesWidget::clear()
{
    while (rowCount() > 0)
        removeRow(0);
}

void EcodataEntriesWidget::sort()
{
    clear();

    if(m_sort_order == SortOrder::Ascending)
        std::sort(m_data.begin(), m_data.end(), [](EntryData one, EntryData two){return one.timestamp < two.timestamp;});
    else if(m_sort_order == SortOrder::Descending)
        std::sort(m_data.begin(), m_data.end(), [](EntryData one, EntryData two){return one.timestamp > two.timestamp;});

    for(EntryData row : m_data)
    {
        int row_id (rowCount());
        insertRow(row_id);
        // Timestamp
        {
            time_t rawtime(row.timestamp);
            setItem(row_id, m_columns.timestamp, generate_read_only_cell(QString(ctime(&rawtime))));
        }
        // Species
        setItem(row_id, m_columns.species, generate_read_only_cell(QString::fromStdString(set_to_string(row.species))));
        // Dir
        setItem(row_id, m_columns.dir, generate_read_only_cell(QString::number(row.dir)));
        // Duration
        setItem(row_id, m_columns.duration, generate_read_only_cell(QString::number(row.duration)));
        // Slope
        setItem(row_id, m_columns.slope, generate_read_only_cell(QString::number(row.slope)));
        // Humidities
        for(int i(0); i < 12; i++)
        {
            // HUMIDITY
            setItem(row_id, m_columns.humidities[i], generate_read_only_cell(QString::number(row.humidities[i])));
            // ILLUMINATION
            setItem(row_id, m_columns.illuminations[i], generate_read_only_cell(QString::number(row.illuminations[i])));
            // TEMPERATURE
            setItem(row_id, m_columns.temperatures[i], generate_read_only_cell(QString::number(row.temperatures[i])));
        }
    }
}
