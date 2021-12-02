#include "keyboard_notes.h"

KeyboardNotes::KeyboardNotes(QWidget *parent) : QWidget(parent)
{
    this->scene = new QGraphicsScene;
    
    this->view = new QGraphicsView;
    this->view->setRenderHint(QPainter::Antialiasing);
    this->view->setScene(this->scene);
    
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(this->view);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);
    
    drawGUI();
    
    setMinimumHeight(150);
}

void KeyboardNotes::drawGUI()
{
    qreal y_line_start = 0;
    for (int i=0; i < 11; i++)
    {
        if (i != 5)
        {
            QGraphicsLineItem *line = new QGraphicsLineItem(0, 0, 300, 0);
            line->setPos(0, y_line_start);
            this->scene->addItem(line);
        }
        y_line_start += 10;
    }
    
    QGraphicsSvgItem *clef_violin = new QGraphicsSvgItem(":clef_violin");
    clef_violin->setScale(.5);
    clef_violin->setPos(-5, -17);
    this->scene->addItem(clef_violin);
    
    QGraphicsSvgItem *clef_bass = new QGraphicsSvgItem(":clef_bass");
    clef_bass->setScale(.06);
    clef_bass->setPos(-5, 45);
    this->scene->addItem(clef_bass);
    
    qreal x_start = 20;
    qreal y_start = 50;
    for (int i=41; i <= 41+22; i++)
    //for (int i=41; i <= 84; i++)
    {
        QGraphicsEllipseItem *note = new QGraphicsEllipseItem(x_start, y_start, 10, 10);
        note->setBrush(QBrush(Qt::black));
        note->setPos(x_start, y_start);
        this->scene->addItem(note);
        
        note->hide();
        
        this->map_of_notes_full[i] = note;
        
        x_start += 5;
        y_start -= 2.5;
    }
}

void KeyboardNotes::keyPressed(int midicode)
{
    if (this->map_of_notes_full.contains(midicode))
    {
        this->map_of_notes_full[midicode]->show();
    }
}
void KeyboardNotes::keyReleased(int midicode)
{
    if (this->map_of_notes_full.contains(midicode))
    {
        this->map_of_notes_full[midicode]->hide();
    }
}
void KeyboardNotes::allKeysUp()
{
    QList<int> keys = this->map_of_notes_full.keys();
    for (int i=0; i < keys.length(); i++)
    {
        this->map_of_notes_full[keys.at(i)]->hide();
    }
}
