#ifndef KEYBOARDNOTES_H
#define KEYBOARDNOTES_H

#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsSvgItem>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsColorizeEffect>
#include <QBrush>
#include <QList>
#include <QKeyEvent>

#include <QDebug>

class KeyboardNotes : public QWidget
{
    Q_OBJECT
public:
    explicit KeyboardNotes(QWidget *parent = nullptr);
    
    void keyPressed(int midicode);
    void keyReleased(int midicode);
    void allKeysUp();
    
private:
    QGraphicsView *view;
    QGraphicsScene *scene;
    QList<int> list_notes_full;
    QList<int> list_notes_half;
    QList<int> list_notes_full_treble;
    QList<int> list_notes_half_treble;
    QList<int> list_notes_full_contra;
    QList<int> list_notes_half_contra;
    QMap<int, QGraphicsEllipseItem*> map_of_notes_full;
    QMap<int, QGraphicsSvgItem*> map_of_sharps;
    QMap<int, QGraphicsSvgItem*> map_of_flats;
    QMap<int, QList<QGraphicsLineItem*>> map_of_auxiliary_lines;
    //QMap<int, QGraphicsEllipseItem*> map_of_notes_half;
    
    qreal NOTE_SIZE = 10;
    
    void drawGUI();
    void drawSharps(qreal pos_x, qreal pos_y, int note, Qt::GlobalColor color);
    void drawFlats(qreal pos_x, qreal pos_y, int note, Qt::GlobalColor color);
    void drawAuxiliaryLines(int note, int pos_x, int pos_y, int i);
    void showHideAuxiliaryLines(int midicode, bool show);
    void showAll();
    bool show_all = false;
    
protected:
    //virtual void keyPressEvent(QKeyEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    
signals:
    
};

#endif // KEYBOARDNOTES_H
