#include "keyboard_notes.h"

// https://newt.phys.unsw.edu.au/jw/notes.html

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
    this->list_notes_half = {13, 15, 18, 20, 22, 25, 27, 30, 32, 34,
                             37, 39, 42, 44, 46, 49, 51, 54, 56, 58, 61, 63, 66, 68, 70, 73, 75, 78, 80, 82,
                             85, 87, 90, 92, 94, 97, 99, 102, 104, 106};
    this->list_notes_full_treble = {86, 88, 89, 91, 93, 95, 96, 98, 100, 101, 103, 105, 107, 108};
    this->list_notes_half_treble = {85, 87, 90, 92, 94, 97, 99, 102, 104, 106};
    this->list_notes_full_contra = {12, 14, 16, 17, 19, 21, 23, 24, 26, 28, 29, 31, 33, 35};
    this->list_notes_half_contra = {13, 15, 18, 20, 22, 25, 27, 30, 32, 34};
    
    qreal line_y = 0;
    for (int i=0; i < 11; i++)
    {
        if (i != 5)
        {
            QGraphicsLineItem *line = new QGraphicsLineItem(0, line_y, 300, line_y);
            this->scene->addItem(line);
        }
        line_y += NOTE_SIZE;
    }
    
    QGraphicsSvgItem *clef_violin = new QGraphicsSvgItem(":clef_violin");
    clef_violin->setScale(.5);
    clef_violin->setPos(-5, -17);
    this->scene->addItem(clef_violin);
    
    QGraphicsSvgItem *clef_bass = new QGraphicsSvgItem(":clef_bass");
    clef_bass->setScale(.07);
    clef_bass->setPos(-8, 40);
    this->scene->addItem(clef_bass);
    
    QGraphicsSvgItem *octave_treble = new QGraphicsSvgItem(":octave_treble");
    octave_treble->setScale(NOTE_SIZE*.03);
    octave_treble->setPos(NOTE_SIZE*4, NOTE_SIZE*-3);
    this->scene->addItem(octave_treble);
    
    QGraphicsSvgItem *octave_contrabass = new QGraphicsSvgItem(":octave_contrabass");
    octave_contrabass->setScale(NOTE_SIZE*.03);
    octave_contrabass->setPos(NOTE_SIZE*15, NOTE_SIZE*11);
    this->scene->addItem(octave_contrabass);
    
    qreal pos_x = NOTE_SIZE/2;
    qreal pos_y = NOTE_SIZE * 11 + NOTE_SIZE/2;
    for (int i=0; i < this->list_notes_full.length(); i++)
    {
        int note_midi = this->list_notes_full.at(i);
        
        // Draw Note Bass & Violin
        QGraphicsEllipseItem *note = new QGraphicsEllipseItem(QRectF(pos_x, pos_y, NOTE_SIZE, NOTE_SIZE));
        note->setBrush(QBrush(Qt::black));
        this->scene->addItem(note);
        
        note->hide();
        
        this->map_of_notes_full[note_midi] = note;
        
        this->drawAuxiliaryLines(this->list_notes_full.at(i), pos_x, pos_y, i);
        this->drawSharps(pos_x, pos_y, note_midi, Qt::black);
        this->drawFlats(pos_x, pos_y, note_midi, Qt::black);
        
        pos_x += NOTE_SIZE;
        pos_y -= NOTE_SIZE/2;
    }
    
    qreal pos_x_treble = NOTE_SIZE*5;
    qreal pos_y_treble = NOTE_SIZE * 4;
    for (int i=0; i < this->list_notes_full_treble.length(); i++)
    {
        int note_midi = this->list_notes_full_treble.at(i);
        
        QGraphicsEllipseItem *note = new QGraphicsEllipseItem(QRectF(pos_x_treble, pos_y_treble, NOTE_SIZE, NOTE_SIZE));
        note->setBrush(QBrush(Qt::darkGreen));
        this->scene->addItem(note);
        
        note->hide();
        
        this->map_of_notes_full[note_midi] = note;
        
        this->drawSharps(pos_x_treble, pos_y_treble, note_midi, Qt::darkGreen);
        this->drawFlats(pos_x_treble, pos_y_treble, note_midi, Qt::darkGreen);
        
        pos_x_treble += NOTE_SIZE;
        pos_y_treble -= NOTE_SIZE/2;
    }
    
    qreal pos_x_contra = NOTE_SIZE*11;
    qreal pos_y_contra = NOTE_SIZE * 11 + NOTE_SIZE/2;
    for (int i=0; i < this->list_notes_full_contra.length(); i++)
    {
        int note_midi = this->list_notes_full_contra.at(i);
        
        QGraphicsEllipseItem *note = new QGraphicsEllipseItem(QRectF(pos_x_contra, pos_y_contra, NOTE_SIZE, NOTE_SIZE));
        note->setBrush(QBrush(Qt::darkCyan));
        this->scene->addItem(note);
        
        note->hide();
        
        this->map_of_notes_full[note_midi] = note;
        
        this->drawSharps(pos_x_contra, pos_y_contra, note_midi, Qt::darkCyan);
        this->drawFlats(pos_x_contra, pos_y_contra, note_midi, Qt::darkCyan);
        
        pos_x_contra += NOTE_SIZE;
        pos_y_contra -= NOTE_SIZE/2;
    }
}

void KeyboardNotes::drawAuxiliaryLines(int note, int pos_x, int pos_y, int i)
{
    //qDebug() << pos_x;
    
    //if (note >= 81)
    /*
    if (pos_y < -10 || pos_y == NOTE_SIZE*5 || pos_y > NOTE_SIZE*10)
    {
        qreal y = pos_y+NOTE_SIZE/2;
        if (pos_y % 10 == 0)
        {
            QGraphicsLineItem *line = new QGraphicsLineItem(pos_x, y, pos_x+15, y);
            qDebug() << pos_x << " " << y << " " << i;
            //line->setPos(pos_x, pos_y);
            line->hide();
            this->scene->addItem(line);
            
            QList<QGraphicsLineItem*> lines;
            lines.append(line);
            this->map_of_auxiliary_lines[note] = lines;
        }
    }
    */
    if (note == 81)
    {
        QGraphicsLineItem *line = new QGraphicsLineItem(0, 0, 15, 0);
        line->setPos(262, -10);
        line->hide();
        this->scene->addItem(line);
        
        QList<QGraphicsLineItem*> lines;
        lines.append(line);
        this->map_of_auxiliary_lines[81] = lines;
    }
    else if (note == 83)
    {
        QGraphicsLineItem *line = new QGraphicsLineItem(0, 0, 15, 0);
        line->setPos(272, -10);
        line->hide();
        this->scene->addItem(line);
        
        QList<QGraphicsLineItem*> lines;
        lines.append(line);
        this->map_of_auxiliary_lines[83] = lines;
    }
    else if (note == 84)
    {
        QGraphicsLineItem *line_a = new QGraphicsLineItem(0, 0, 15, 0);
        line_a->setPos(282, -10);
        line_a->hide();
        this->scene->addItem(line_a);
        
        QGraphicsLineItem *line_b = new QGraphicsLineItem(0, 0, 15, 0);
        line_b->setPos(282, -20);
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
        line->setPos(142, 50);
        line->hide();
        this->scene->addItem(line);
        
        QList<QGraphicsLineItem*> lines;
        lines.append(line);
        this->map_of_auxiliary_lines[60] = lines;
    }
    else if (note == 40)
    {
        QGraphicsLineItem *line = new  QGraphicsLineItem(0, 0, 15, 0);
        line->setPos(22, 110);
        line->hide();
        this->scene->addItem(line);
        
        QList<QGraphicsLineItem*> lines;
        lines.append(line);
        this->map_of_auxiliary_lines[40] = lines;
    }
    else if (note == 38)
    {
        QGraphicsLineItem *line = new QGraphicsLineItem(0, 0, 15, 0);
        line->setPos(12, 110);
        line->hide();
        this->scene->addItem(line);
        
        QList<QGraphicsLineItem*> lines;
        lines.append(line);
        this->map_of_auxiliary_lines[38] = lines;
    }
    else if (note == 36)
    {
        QGraphicsLineItem *line_a = new QGraphicsLineItem(0, 0, 15, 0);
        line_a->setPos(2, 110);
        line_a->hide();
        this->scene->addItem(line_a);
        
        QGraphicsLineItem *line_b = new QGraphicsLineItem(0, 0, 15, 0);
        line_b->setPos(2, 120);
        line_b->hide();
        this->scene->addItem(line_b);
        
        QList<QGraphicsLineItem*> lines;
        lines.append(line_a);
        lines.append(line_b);
        this->map_of_auxiliary_lines[36] = lines;
    }
}

void KeyboardNotes::drawSharps(qreal pos_x, qreal pos_y, int note, Qt::GlobalColor color)
{
    QGraphicsSvgItem *sharp = new QGraphicsSvgItem(":sharp");
    QGraphicsColorizeEffect *effect = new QGraphicsColorizeEffect;
    effect->setColor(color);
    sharp->setGraphicsEffect(effect);
    sharp->setScale(NOTE_SIZE*.1);
    sharp->setPos(pos_x-NOTE_SIZE*2,
                  pos_y-NOTE_SIZE/2);
    this->scene->addItem(sharp);
    
    sharp->hide();
    
    this->map_of_sharps[note] = sharp;
}

void KeyboardNotes::drawFlats(qreal pos_x, qreal pos_y, int note, Qt::GlobalColor color)
{
    QGraphicsSvgItem *flat = new QGraphicsSvgItem(":flat");
    QGraphicsColorizeEffect *effect = new QGraphicsColorizeEffect;
    effect->setColor(color);
    flat->setGraphicsEffect(effect);
    // magic number to scale the svg to proper size
    flat->setScale(NOTE_SIZE*.007);
    flat->setPos(pos_x-NOTE_SIZE*2-NOTE_SIZE,
                 pos_y-NOTE_SIZE/2-NOTE_SIZE*.8);
    this->scene->addItem(flat);
    
    flat->hide();
    
    this->map_of_flats[note] = flat;
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

void KeyboardNotes::showAll()
{
    QList<int> keys_full = this->map_of_notes_full.keys();
    for (int i=0; i < keys_full.length(); i++)
    {
        if (this->show_all)
        {
            this->map_of_notes_full[keys_full.at(i)]->show();
        }
        else
        {
            this->map_of_notes_full[keys_full.at(i)]->hide();
        }
    }
    
    QList<int> keys_sharps = this->map_of_sharps.keys();
    for (int i=0; i < keys_sharps.length(); i++)
    {
        if (this->show_all)
        {
            this->map_of_sharps[keys_sharps.at(i)]->show();
        }
        else
        {
            this->map_of_sharps[keys_sharps.at(i)]->hide();
        }
    }
    
    QList<int> keys_flats = this->map_of_flats.keys();
    for (int i=0; i < keys_flats.length(); i++)
    {
        if (this->show_all)
        {
            this->map_of_flats[keys_sharps.at(i)]->show();
        }
        else
        {
            this->map_of_flats[keys_sharps.at(i)]->hide();
        }
    }
    
    QList<int> keys_lines = this->map_of_auxiliary_lines.keys();
    for (int i=0; i < keys_lines.length(); i++)
    {
        QList<QGraphicsLineItem*> lines = this->map_of_auxiliary_lines[keys_lines.at(i)];
        for (int j=0; j < lines.length(); j++)
        {
            if (this->show_all)
            {
                lines.at(j)->show();
            }
            else
            {
                lines.at(j)->hide();
            }
        }
    }
}

void KeyboardNotes::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    
    if (this->show_all)
    {
        this->show_all = false;
    }
    else
    {
        this->show_all = true;
    }
    
    this->showAll();
}
