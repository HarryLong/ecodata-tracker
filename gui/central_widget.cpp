#include "central_widget.h"
#include "../utils.h"

#include <QBoxLayout>
#include <QLabel>

#include "ecodata_entries_widget.h"
#include <QLineEdit>
#include <QPushButton>


/******************
 * CENTRAL WIDGET *
 ******************/
CentralWidget::CentralWidget() :
    m_data_widget(new EcodataEntriesWidget()),
    m_refresh_btn(new QPushButton("Refresh", this))
{
    init_layout();

    connect(m_refresh_btn, SIGNAL(clicked(bool)), this, SLOT(refresh()));
}

CentralWidget::~CentralWidget()
{

}

void CentralWidget::refresh()
{
    m_data_widget->refresh();
}

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

    // Refresh Button
    {
        QHBoxLayout * h_layout = new QHBoxLayout;
        h_layout->addWidget(m_refresh_btn, 0, Qt::AlignCenter);

        main_layout->addLayout(h_layout);
    }

    // MainWidget
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
