#ifndef CENTRAL_WIDGET_H
#define CENTRAL_WIDGET_H

#include <vector>
#include <QString>
#include <QTableWidget>


class QPushButton;
class EcodataEntriesWidget;

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
    EcodataEntriesWidget * m_data_widget;
};

#endif //CENTRAL_WIDGET_H
