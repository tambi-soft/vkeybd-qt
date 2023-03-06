#include "help_about_widget.h"

HelpAboutWidget::HelpAboutWidget(QWidget *parent)
    : QWidget(parent)
    , layout (new QVBoxLayout)
    , tab_widget (new QTabWidget)
{
    setLayout(layout);
    layout->addWidget(tab_widget);
    
    showAbout();
    showLibraries();
    //showAuthors();
    //showThanks();
    showLicense();
}

void HelpAboutWidget::showAbout()
{
    QTextEdit *text = new QTextEdit();
    text->setText("vkeybd-qt - Software MIDI controller with a LOT of features\n\n\
- Early Alpha Version -\n\n\
(c) 2021-2023 The vkeybd-qt Author(s)\n\n");
    
    text->setReadOnly(true);
    this->tab_widget->addTab(text, "About");
}

void HelpAboutWidget::showLibraries()
{
    QTextEdit *text = new QTextEdit();
    text->setText("Qt (5.15.8 / 6.4.2)\n\
asound\n\
jack\n\
X11");
    
    text->setReadOnly(true);
    this->tab_widget->addTab(text, "Libraries");
}

void HelpAboutWidget::showAuthors()
{
    QTextEdit *text = new QTextEdit();
    text->setText("");
    
    text->setReadOnly(true);
    this->tab_widget->addTab(text, "Authors");
}

void HelpAboutWidget::showThanks()
{
    QTextEdit *text = new QTextEdit();
    text->setText("");
    
    text->setReadOnly(true);
    this->tab_widget->addTab(text, "Thanks To");
}

void HelpAboutWidget::showLicense()
{
    QFile file(":gpl");
    
    QString lines;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        while (!stream.atEnd())
        {
            lines.append(stream.readLine() + "\n");
        }
    }
    file.close();
    
    QTextEdit *text = new QTextEdit();
    text->setText(lines);
    
    text->setReadOnly(true);
    this->tab_widget->addTab(text, "License");
}
