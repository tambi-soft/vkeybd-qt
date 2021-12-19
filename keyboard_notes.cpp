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
    
    setMinimumHeight(170);
}

void KeyboardNotes::drawGUI()
{
    this->list_notes_full = {36, 38, 40, 41, 43, 45, 47, 48, 50, 52, 53, 55, 57, 59, 60, 62, 64, 65, 67, 69, 71, 72, 74, 76, 77, 79, 81, 83, 84};
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
    for (int i=0; i < this->list_notes_full.length(); i++)
    {
        QGraphicsEllipseItem *note = new QGraphicsEllipseItem(QRectF(x_start, y_start, 8, 10));
        note->setBrush(QBrush(Qt::black));
        note->setPos(x_start, y_start);
        this->scene->addItem(note);
        
        note->hide();
        
        this->map_of_notes_full[this->list_notes_full.at(i)] = note;
        
        x_start += 4;
        y_start -= 2.5;
        
        drawAuxiliaryLines(this->list_notes_full.at(i));
    }
    
    qreal x_sharp = 0;
    qreal y_sharp = 110;
    for (int i=0; i < this->list_notes_full.length(); i++)
    {
        QGraphicsSvgItem *sharp = new QGraphicsSvgItem(":sharp");
        sharp->setPos(x_sharp, y_sharp);
        this->scene->addItem(sharp);
        
        sharp->hide();
        
        this->map_of_sharps[this->list_notes_full.at(i)] = sharp;
        
        x_sharp += 4*2;
        y_sharp -= 2.5*2;
    }
    
    qreal x_flat = -10;
    qreal y_flat = 102;
    for (int i=0; i < this->list_notes_full.length(); i++)
    {
        QGraphicsSvgItem *flat = new QGraphicsSvgItem(":flat");
        flat->setScale(.07);
        flat->setPos(x_flat, y_flat);
        this->scene->addItem(flat);
        
        flat->hide();
        
        this->map_of_flats[this->list_notes_full.at(i)] = flat;
        
        x_flat += 4*2;
        y_flat -= 2.5*2;
    }
}

void KeyboardNotes::drawAuxiliaryLines(int note)
{
    if (note == 81)
    {
        QGraphicsLineItem *line = new QGraphicsLineItem(0, 0, 15, 0);
        line->setPos(225, -10);
        line->hide();
        this->scene->addItem(line);
        
        QList<QGraphicsLineItem*> lines;
        lines.append(line);
        this->map_of_auxiliary_lines[81] = lines;
    }
    else if (note == 83)
    {
        QGraphicsLineItem *line = new QGraphicsLineItem(0, 0, 15, 0);
        line->setPos(232, -10);
        line->hide();
        this->scene->addItem(line);
        
        QList<QGraphicsLineItem*> lines;
        lines.append(line);
        this->map_of_auxiliary_lines[83] = lines;
    }
    else if (note == 84)
    {
        QGraphicsLineItem *line_a = new QGraphicsLineItem(0, 0, 15, 0);
        line_a->setPos(240, -10);
        line_a->hide();
        this->scene->addItem(line_a);
        
        QGraphicsLineItem *line_b = new QGraphicsLineItem(0, 0, 15, 0);
        line_b->setPos(240, -20);
        line_b->hide();
        this->scene->addItem(line_b);
        
        QList<QGraphicsLineItem*> lines;
        lines.append(line_a);
        lines.append(line_b);
        this->map_of_auxiliary_lines[84] = lines;
    }
    else if (note == 60)
    {
        QGraphicsLineItem *line = new QGraphicsLineItem(0, 0, 15, 0);
        line->setPos(128, 50);
        line->hide();
        this->scene->addItem(line);
        
        QList<QGraphicsLineItem*> lines;
        lines.append(line);
        this->map_of_auxiliary_lines[60] = lines;
    }
    else if (note == 40)
    {
        QGraphicsLineItem *line = new  QGraphicsLineItem(0, 0, 15, 0);
        line->setPos(32, 110);
        line->hide();
        this->scene->addItem(line);
        
        QList<QGraphicsLineItem*> lines;
        lines.append(line);
        this->map_of_auxiliary_lines[40] = lines;
    }
    else if (note == 38)
    {
        QGraphicsLineItem *line = new QGraphicsLineItem(0, 0, 15, 0);
        line->setPos(24, 110);
        line->hide();
        this->scene->addItem(line);
        
        QList<QGraphicsLineItem*> lines;
        lines.append(line);
        this->map_of_auxiliary_lines[38] = lines;
    }
    else if (note == 36)
    {
        QGraphicsLineItem *line_a = new QGraphicsLineItem(0, 0, 15, 0);
        line_a->setPos(16, 110);
        line_a->hide();
        this->scene->addItem(line_a);
        
        QGraphicsLineItem *line_b = new QGraphicsLineItem(0, 0, 15, 0);
        line_b->setPos(16, 120);
        line_b->hide();
        this->scene->addItem(line_b);
        
        QList<QGraphicsLineItem*> lines;
        lines.append(line_a);
        lines.append(line_b);
        this->map_of_auxiliary_lines[36] = lines;
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
        keyPressed(midicode-1);
        keyPressed(midicode+1);
        
        this->map_of_sharps[midicode-1]->show();
        this->map_of_flats[midicode+1]->show();
    }
    
    showHideAuxiliaryLines(midicode, true);
}
void KeyboardNotes::keyReleased(int midicode)
{
    if (this->map_of_notes_full.contains(midicode))
    {
        this->map_of_notes_full[midicode]->hide();
    }
    else if (this->list_notes_half.contains(midicode))
    {
        keyReleased(midicode-1);
        keyReleased(midicode+1);
        
        this->map_of_sharps[midicode-1]->hide();
        this->map_of_flats[midicode+1]->hide();
    }
    
    showHideAuxiliaryLines(midicode, false);
}
void KeyboardNotes::showHideAuxiliaryLines(int midicode, bool show)
{
    if (this->map_of_auxiliary_lines.contains(midicode))
    {
        QList<QGraphicsLineItem*> lines = this->map_of_auxiliary_lines[midicode];
        for (int i=0; i < lines.length(); i++)
        {
            if (show)
            {
                lines.at(i)->show();
            }
            else
            {
                lines.at(i)->hide();
            }
        }
    }
}
void KeyboardNotes::allKeysUp()
{
    QList<int> keys_full = this->map_of_notes_full.keys();
    for (int i=0; i < keys_full.length(); i++)
    {
        this->map_of_notes_full[keys_full.at(i)]->hide();
    }
    
    QList<int> keys_sharps = this->map_of_sharps.keys();
    for (int i=0; i < keys_sharps.length(); i++)
    {
        this->map_of_sharps[keys_sharps.at(i)]->hide();
    }
    
    QList<int> keys_flats = this->map_of_flats.keys();
    for (int i=0; i < keys_flats.length(); i++)
    {
        this->map_of_flats[keys_sharps.at(i)]->hide();
    }
    
    QList<int> keys_lines = this->map_of_auxiliary_lines.keys();
    for (int i=0; i < keys_lines.length(); i++)
    {
        QList<QGraphicsLineItem*> lines = this->map_of_auxiliary_lines[keys_lines.at(i)];
        for (int j=0; j < lines.length(); j++)
        {
            lines.at(j)->hide();
        }
    }
}
