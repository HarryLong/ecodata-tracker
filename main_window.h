#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "central_widget.h"

#include <QMainWindow>

class MainWindow : public QMainWindow
{
Q_OBJECT
public:
    MainWindow(QWidget * parent = 0, Qt::WindowFlags flags = 0);
    ~MainWindow();

protected:
    virtual void closeEvent ( QCloseEvent * event );

private slots:

private:
    void init_layout();
    void init_actions();
    void init_menu();
    CentralWidget * m_central_widget;
};


#endif //MAIN_WINDOW_H
