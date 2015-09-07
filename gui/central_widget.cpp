#include "central_widget.h"
#include "../utils.h"

#include <QBoxLayout>
#include <QLabel>
#include <iostream>


DataWidget::Columns::Columns()
{
    int i(0);
    species = i++;
    dir = i++;
    duration = i++;
    for(int ii(0); ii < 12; ii++)
        humidities[ii] = i++;
    for(int ii(0); ii < 12; ii++)
        illuminations[ii] = i++;
    for(int ii(0); ii < 12; ii++)
        temperatures[ii] = i++;
    column_count = i-1;
}

DataWidget::Columns::~Columns()
{

}

/***************
 * DATA WIDGET *
 ***************/
DataWidget::DataWidget()
{
    setColumnCount(m_columns.column_count);

    // Species
    setHorizontalHeaderItem(m_columns.species, new QTableWidgetItem("Species"));
    // Dir
    setHorizontalHeaderItem(m_columns.dir, new QTableWidgetItem("Dir"));
    // Duration
    setHorizontalHeaderItem(m_columns.duration, new QTableWidgetItem("Duration"));
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

        // Species
        setItem(row_id, m_columns.species, generate_read_only_cell(QString::fromStdString(set_to_string(row.species))));
        // Dir
        setItem(row_id, m_columns.dir, generate_read_only_cell(QString::number(row.dir)));
        // Duration
        setItem(row_id, m_columns.duration, generate_read_only_cell(QString::number(row.duration)));
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

/******************
 * CENTRAL WIDGET *
 ******************/
CentralWidget::CentralWidget() :
    m_data_widget(new DataWidget())
{
    init_layout();
    m_data_widget->set(m_db.getAllData());
}

CentralWidget::~CentralWidget()
{

}

//void CentralWidget::refresh()
//{
//    std::pair<int,int> humidity(get_int_line_edit_value(m_min_humidity_query), get_int_line_edit_value(m_max_humidity_query) );
//    std::pair<int,int> illumination(get_int_line_edit_value(m_min_illumination_query), get_int_line_edit_value(m_max_illumination_query) );
//    std::pair<int,int> temperature(get_int_line_edit_value(m_min_temperature_query), get_int_line_edit_value(m_max_temperature_query) );
//    std::set<int> species ( string_to_set((m_species_query->text().trimmed().toStdString()) ));

//    m_data_widget->set(m_db.getData(humidity, illumination, temperature, species));
//}

//int CentralWidget::get_int_line_edit_value(QLineEdit * line_edit)
//{
//    QString text (line_edit->text().trimmed());

//    if(text.isEmpty())
//        return -1;
//    else
//        return text.toInt();
//}

void CentralWidget::init_layout()
{
    QVBoxLayout * main_layout = new QVBoxLayout;

    main_layout->addWidget(m_data_widget, 1);

    setLayout(main_layout);

//    // Min/max humidity
//    {
//        m_min_humidity_query = create_connected_line_edit() ;
//        m_max_humidity_query = create_connected_line_edit() ;
//        QHBoxLayout * layout = new QHBoxLayout;

//        layout->addWidget(new QLabel("Humidity       |  Min: "),0);
//        layout->addWidget(m_min_humidity_query,0);
//        layout->addWidget(new QLabel("  |  Max: "),0);
//        layout->addWidget(m_max_humidity_query,0);

//        // Padding
//        layout->addWidget(new QLabel, 1);
//        main_layout->addLayout(layout, 0);
//    }
//    // Min/max illumination
//    {
//        m_min_illumination_query = create_connected_line_edit() ;
//        m_max_illumination_query = create_connected_line_edit() ;
//        QHBoxLayout * layout = new QHBoxLayout;

//        layout->addWidget(new QLabel("Illumination  |  Min: "),0);
//        layout->addWidget(m_min_illumination_query,0);
//        layout->addWidget(new QLabel("  |  Max: "),0);
//        layout->addWidget(m_max_illumination_query,0);

//        // Padding
//        layout->addWidget(new QLabel, 1);
//        main_layout->addLayout(layout, 0);
//    }
//    // Min/max temperature
//    {
//        m_min_temperature_query = create_connected_line_edit() ;
//        m_max_temperature_query = create_connected_line_edit() ;
//        QHBoxLayout * layout = new QHBoxLayout;

//        layout->addWidget(new QLabel("Temperature  | Min: "),0);
//        layout->addWidget(m_min_temperature_query,0);
//        layout->addWidget(new QLabel("  |  Max: "),0);
//        layout->addWidget(m_max_temperature_query,0);

//        // Padding
//        layout->addWidget(new QLabel, 1);

//        main_layout->addLayout(layout, 0);
//    }

//    // Species
//    {
//        m_species_query = create_connected_line_edit() ;
//        QHBoxLayout * layout = new QHBoxLayout;

//        layout->addWidget(new QLabel("Species (comma separated): "),0);
//        layout->addWidget(m_species_query,0);

//        // Padding
//        layout->addWidget(new QLabel, 1);
//        main_layout->addLayout(layout, 0);
//    }

//    m_refresh_btn = new QPushButton("Refresh");
//    connect(m_refresh_btn, SIGNAL(clicked()), this, SLOT(refresh()));
//    main_layout->addWidget(m_refresh_btn,0, Qt::AlignCenter);
}

//QLineEdit * CentralWidget::create_connected_line_edit()
//{
//    QLineEdit * line_edit = new QLineEdit;
//    connect(line_edit, SIGNAL(textChanged(QString)), this, SLOT(refresh()));
//    return line_edit;
//}
