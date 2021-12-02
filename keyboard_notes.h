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
#include <QBrush>
#include <QList>

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
    QList<int> list_notes_half;
    QMap<int, QGraphicsEllipseItem*> map_of_notes_full;
    QMap<int, QGraphicsSvgItem*> map_of_sharps;
    QMap<int, QGraphicsSvgItem*> map_of_flats;
    //QMap<int, QGraphicsEllipseItem*> map_of_notes_half;
    
    void drawGUI();
    
signals:
    
};

#endif // KEYBOARDNOTES_H
