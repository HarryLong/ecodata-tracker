#include "main_window.h"
#include <iostream>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags) :
    QMainWindow(parent, flags)
{
    init_layout();
}

MainWindow::~MainWindow()
{

}

void MainWindow::init_layout()
{
    m_central_widget = new CentralWidget();
    setCentralWidget(m_central_widget);
}

void MainWindow::init_actions()
{

}

void MainWindow::init_menu()
{

}

void MainWindow::closeEvent ( QCloseEvent * event )
{
}

//#include "utils.h"
int main(int argc, char *argv[])
{
    std::cout << "Starting..." << std::endl;
    QApplication app(argc, argv);
    MainWindow w;
    w.resize(w.sizeHint());
    w.showMaximized();

    return app.exec();
}
