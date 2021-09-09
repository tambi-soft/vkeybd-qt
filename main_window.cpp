#include "main_window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setCentralWidget(this->piano);
}

MainWindow::~MainWindow()
{
    
}

