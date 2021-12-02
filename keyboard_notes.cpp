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
    
    setMinimumHeight(160);
}

void KeyboardNotes::drawGUI()
{
    QList<int> list_notes_full = {36, 38, 40, 41, 43, 45, 47, 48, 50, 52, 53, 55, 57, 59, 60, 62, 64, 65, 67, 69, 71, 72, 74, 76, 77, 79, 81, 83, 84};
    this->list_notes_half = {37, 39, 42, 44, 46, 49, 51, 54, 56, 58, 61, 63, 66, 68, 70, 73, 75, 78, 80, 82};
    
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
    
    
    qreal x_start = 10;
    qreal y_start = 57.5;
    for (int i=0; i < list_notes_full.length(); i++)
    {
        //QGraphicsEllipseItem *note = new QGraphicsEllipseItem(x_start, y_start, 10, 10);
        QGraphicsEllipseItem *note = new QGraphicsEllipseItem(QRectF(x_start, y_start, 8, 10));
        note->setBrush(QBrush(Qt::black));
        note->setPos(x_start, y_start);
        //note->setRotation(45);
        this->scene->addItem(note);
        
        note->hide();
        
        this->map_of_notes_full[list_notes_full.at(i)] = note;
        
        x_start += 4;
        y_start -= 2.5;
    }
    
    
    
}

void KeyboardNotes::keyPressed(int midicode)
{
    qDebug() << midicode;
    if (this->map_of_notes_full.contains(midicode))
    {
        this->map_of_notes_full[midicode]->show();
    }
    else if (this->list_notes_half.contains(midicode))
    {
        
    }
}
void KeyboardNotes::keyReleased(int midicode)
{
    if (this->map_of_notes_full.contains(midicode))
    {
        this->map_of_notes_full[midicode]->hide();
    }
    else if (this->list_notes_half.contains(midicode))
    {
        
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
