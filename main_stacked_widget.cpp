#include "main_stacked_widget.h"

MainStackedWidget::MainStackedWidget(QWidget *parent)
    : QWidget{parent}
{
    
    
        
}



StackedWidgetSwitches::StackedWidgetSwitches(QWidget *parent)
    : QWidget{parent}
{
    QGridLayout *grid = new QGridLayout;
    setLayout(grid);
    grid->setContentsMargins(0, 0, 0, 0);
    
    this->list_function_keys = {Qt::Key_F1, Qt::Key_F2, Qt::Key_F3, Qt::Key_F4, Qt::Key_F5, Qt::Key_F6, Qt::Key_F7, Qt::Key_F8, Qt::Key_F9, Qt::Key_F10, Qt::Key_F11, Qt::Key_F12};
    this->list_labels = {"F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12"};
    
    int row = 0;
    int col = 0;
    for (int i=0; i < this->list_labels.length(); i++)
    {
        if (i % 12 == 0)
        {
            row++;
            col = 0;
        }
        
        QPushButton *button = new QPushButton(this->list_labels.at(i));
        QString stylesheet = "QPushButton {"
                             "  font-size: 6pt;"
                             "}";
        button->setStyleSheet(stylesheet);
        //button->setMaximumWidth(25);
        button->setMinimumWidth(20);
        
        grid->addWidget(button, row, col);
        
        col++;
    }
    
    setMaximumWidth(370);
}
