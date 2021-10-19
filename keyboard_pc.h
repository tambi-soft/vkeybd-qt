#ifndef KEYBOARDPC_H
#define KEYBOARDPC_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QList>
#include <QDebug>

class KeyboardPC : public QWidget
{
    Q_OBJECT
public:
    explicit KeyboardPC(QWidget *parent = nullptr);
    
    float button_scale = .799;
    
private:
    QList<QList<int>> getButtonSizes();
    QList<QList<QString>> getButtonColors();
    QList<QList<QString>> getButtonLabels();
    void drawButtons();
    float calculateOffset(float row_offset, int row, int col, QList<QList<int>> sizes);
    
signals:
    
};

#endif // KEYBOARDPC_H
