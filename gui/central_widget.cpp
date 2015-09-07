#include "central_widget.h"
#include "../utils.h"

#include <QBoxLayout>
#include <QLabel>
#include <iostream>

/***************
 * DATA WIDGET *
 ***************/
DataWidget::DataWidget()
{
    setColumnCount(DataColumns::ColumnCount);
    setHorizontalHeaderItem(DataColumns::MinHumidity ,new QTableWidgetItem("Min humidity"));
    setHorizontalHeaderItem(DataColumns::MaxHumidity ,new QTableWidgetItem("Max humidity"));

    setHorizontalHeaderItem(DataColumns::MinIllumination ,new QTableWidgetItem("Min illumination"));
    setHorizontalHeaderItem(DataColumns::MaxIllumination,new QTableWidgetItem("Max illumination"));

    setHorizontalHeaderItem(DataColumns::MinTemp ,new QTableWidgetItem("Min temp"));
    setHorizontalHeaderItem(DataColumns::MaxTemp,new QTableWidgetItem("Max temp"));

    setHorizontalHeaderItem(DataColumns::Duration,new QTableWidgetItem("Duration"));

    setHorizontalHeaderItem(DataColumns::Species,new QTableWidgetItem("Species"));

    setHorizontalHeaderItem(DataColumns::Dir,new QTableWidgetItem("dir"));
}

DataWidget::~DataWidget()
{

}

QTableWidgetItem * DataWidget::generate_read_only_cell(QString p_cell_content)
{
    QTableWidgetItem * cell = new QTableWidgetItem(p_cell_content);
    cell->setFlags(cell->flags() ^ Qt::ItemIsEditable);

    return cell;
}

void DataWidget::clear()
{
    while (rowCount() > 0)
        removeRow(0);
}

void DataWidget::set(std::vector<EntryData> data)
{
    clear();
    for(EntryData row : data)
    {
        int row_id (rowCount());
        insertRow(row_id);

        // Humidity
        setItem(row_id, DataColumns::MinHumidity,  generate_read_only_cell(QString::number(row.humidity.first)));
        setItem(row_id, DataColumns::MaxHumidity,  generate_read_only_cell(QString::number(row.humidity.second)));

        // Illumination
        setItem(row_id, DataColumns::MinIllumination,  generate_read_only_cell(QString::number(row.illumination.first)));
        setItem(row_id, DataColumns::MaxIllumination,  generate_read_only_cell(QString::number(row.illumination.second)));

        // Temperature
        setItem(row_id, DataColumns::MinTemp,  generate_read_only_cell(QString::number(row.temperature.first)));
        setItem(row_id, DataColumns::MaxTemp,  generate_read_only_cell(QString::number(row.temperature.second)));

        // Duration
        setItem(row_id, DataColumns::Duration,  generate_read_only_cell(QString::number(row.duration)));

        // Species
        setItem(row_id, DataColumns::Species,  generate_read_only_cell(QString::fromStdString(set_to_string(row.species))));

        // Dir
        setItem(row_id, DataColumns::Dir,  generate_read_only_cell(QString::number(row.dir)));
    }
}

/******************
 * CENTRAL WIDGET *
 ******************/
CentralWidget::CentralWidget()
{
    init_layout();
    refresh();
}

CentralWidget::~CentralWidget()
{

}

void CentralWidget::refresh()
{
    std::pair<int,int> humidity(get_int_line_edit_value(m_min_humidity_query), get_int_line_edit_value(m_max_humidity_query) );
    std::pair<int,int> illumination(get_int_line_edit_value(m_min_illumination_query), get_int_line_edit_value(m_max_illumination_query) );
    std::pair<int,int> temperature(get_int_line_edit_value(m_min_temperature_query), get_int_line_edit_value(m_max_temperature_query) );
    std::set<int> species ( string_to_set((m_species_query->text().trimmed().toStdString()) ));

    m_data_widget->set(m_db.getData(humidity, illumination, temperature, species));
}

int CentralWidget::get_int_line_edit_value(QLineEdit * line_edit)
{
    QString text (line_edit->text().trimmed());

    if(text.isEmpty())
        return -1;
    else
        return text.toInt();
}

void CentralWidget::init_layout()
{
    QVBoxLayout * main_layout = new QVBoxLayout;

    // Min/max humidity
    {
        m_min_humidity_query = create_connected_line_edit() ;
        m_max_humidity_query = create_connected_line_edit() ;
        QHBoxLayout * layout = new QHBoxLayout;

        layout->addWidget(new QLabel("Humidity       |  Min: "),0);
        layout->addWidget(m_min_humidity_query,0);
        layout->addWidget(new QLabel("  |  Max: "),0);
        layout->addWidget(m_max_humidity_query,0);

        // Padding
        layout->addWidget(new QLabel, 1);
        main_layout->addLayout(layout, 0);
    }
    // Min/max illumination
    {
        m_min_illumination_query = create_connected_line_edit() ;
        m_max_illumination_query = create_connected_line_edit() ;
        QHBoxLayout * layout = new QHBoxLayout;

        layout->addWidget(new QLabel("Illumination  |  Min: "),0);
        layout->addWidget(m_min_illumination_query,0);
        layout->addWidget(new QLabel("  |  Max: "),0);
        layout->addWidget(m_max_illumination_query,0);

        // Padding
        layout->addWidget(new QLabel, 1);
        main_layout->addLayout(layout, 0);
    }
    // Min/max temperature
    {
        m_min_temperature_query = create_connected_line_edit() ;
        m_max_temperature_query = create_connected_line_edit() ;
        QHBoxLayout * layout = new QHBoxLayout;

        layout->addWidget(new QLabel("Temperature  | Min: "),0);
        layout->addWidget(m_min_temperature_query,0);
        layout->addWidget(new QLabel("  |  Max: "),0);
        layout->addWidget(m_max_temperature_query,0);

        // Padding
        layout->addWidget(new QLabel, 1);

        main_layout->addLayout(layout, 0);
    }

    // Species
    {
        m_species_query = create_connected_line_edit() ;
        QHBoxLayout * layout = new QHBoxLayout;

        layout->addWidget(new QLabel("Species (comma separated): "),0);
        layout->addWidget(m_species_query,0);

        // Padding
        layout->addWidget(new QLabel, 1);
        main_layout->addLayout(layout, 0);
    }

    m_refresh_btn = new QPushButton("Refresh");
    connect(m_refresh_btn, SIGNAL(clicked()), this, SLOT(refresh()));
    main_layout->addWidget(m_refresh_btn,0, Qt::AlignCenter);

    m_data_widget = new DataWidget();
    main_layout->addWidget(m_data_widget, 1);

    setLayout(main_layout);
}

QLineEdit * CentralWidget::create_connected_line_edit()
{
    QLineEdit * line_edit = new QLineEdit;
    connect(line_edit, SIGNAL(textChanged(QString)), this, SLOT(refresh()));
    return line_edit;
}
