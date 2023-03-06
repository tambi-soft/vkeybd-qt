#include "help_message.h"

HelpMessage::HelpMessage(QString filename, QDialog *parent) : QDialog(parent)
{
    //320 500
    setMinimumWidth(500);
    setMinimumHeight(500);
    
    setWindowTitle("vkeybd-qt - help");
    
    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);
    
    QFile file(filename);
    
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
    
    layout->addWidget(text);
    
    QPushButton *button_close = new QPushButton("ok");
    connect(button_close, &QPushButton::clicked, this, [this]{ this->close(); });
    layout->addWidget(button_close);
    
    exec();
}
