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
    QMap<int, QGraphicsEllipseItem*> map_of_notes_full;
    
    void drawGUI();
    
signals:
    
};

#endif // KEYBOARDNOTES_H
