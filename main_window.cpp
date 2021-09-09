#include "main_window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setCentralWidget(this->piano);
    //resize(this->piano->size->width(), this->piano->size->height());
    resize(this->piano->size);
}

MainWindow::~MainWindow()
{
    
}

