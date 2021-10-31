#include "keyboard_pc.h"

KeyboardPC::KeyboardPC(QWidget *parent) : QWidget(parent)
{
    drawButtons();
    
    setMinimumSize(200, 119);
}

void KeyboardPC::drawButtons()
{
    QList<QList<int>> sizes = getButtonSizes();
    QList<QList<QString>> colors = getButtonColors();
    QList<QList<QString>> labels = getButtonLabels();
    
    for (int row = 0; row < sizes.length(); row++)
    {
        QList<QPushButton*> list_of_buttons_row;
        float offset = 0;
        for (int col = 0; col < sizes.at(row).length(); col++)
        {
            offset = calculateOffset(
                            offset,
                            row,
                            col,
                            sizes
                        );
            
            QPushButton *button = new QPushButton(this);
            button->setText(labels.at(row).at(col));
            
            button->resize(
                            sizes.at(row).at(col) * this->button_scale,
                            30 * this->button_scale
                        );
            
            float xpos = (static_cast<float>(col) + static_cast<float>(offset)) * static_cast<float>(30);
            button->move(
                            xpos * this->button_scale,
                            row * 30 * this->button_scale
                        );
            
            button->show();
            
            QString color = "black";
            QString background_color = "white";
            if (colors.at(row).at(col) == "b")
            {
                color = "white";
                background_color = "black";
            }
            else if (colors.at(row).at(col) == "g")
            {
                color = "black";
                background_color = "grey";
            }
            QString stylesheet = "QPushButton {"
                                 "  color: "+ color +";"
                                 "  background-color: "+ background_color + ";"
                                 "  font-size: 5pt;"
                                 "  border-style: outset;"
                                 "  border-width: 1px;"
                                 "  border-color: black;"
                                 "  border-radius: 0px;"
                                 "}"
                                 "QPushButton:pressed {"
                                 "  color: white;"
                                 "  background-color: blue;"
                                 "}";
            
            button->setStyleSheet(stylesheet);
            
            list_of_buttons_row.append(button);
        }
        this->list_of_buttons.append(list_of_buttons_row);
    }
    //qDebug() << this->list_of_buttons;
}

float KeyboardPC::calculateOffset(float row_offset, int row, int col, QList<QList<int>> sizes)
{
    if (col > 0)
    {
        int size_a = sizes.at(row).at(col-1);
        int size_b = sizes.at(row).at(col);
        
        if (size_a > size_b)
        {
            if (size_b >= 30)
            {
                float offset = static_cast<float>(size_a) / static_cast<float>(size_b);
                row_offset = static_cast<float>(row_offset) + static_cast<float>(offset) - static_cast<float>(1);
            }
        }
    }
    return row_offset;
}

QList<QList<int>> KeyboardPC::getButtonSizes()
{
    QList<QList<int>> list;
    list = {{30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 60},
            {40, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 50},
            {50, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 70},
            {40, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 80},
            {30, 30, 30, 150, 30, 30, 30, 30, 30, 30, 30}};
    return list;
}

QList<QList<QString>> KeyboardPC::getButtonColors()
{
    QList<QList<QString>> list;
    list = {{"b", "w", "w", "w", "b", "w", "w", "w", "b", "w", "w", "w", "b", "w"},
            {"w", "w", "b", "b", "w", "w", "b", "b", "w", "w", "b", "b", "w", "g"},
            {"b", "b", "w", "w", "b", "b", "w", "w", "b", "b", "w", "w", "g"},
            {"b", "w", "w", "w", "b", "w", "w", "w", "b", "w", "w", "w", "b"},
            {"g", "g", "g", "g", "g", "g", "g", "g", "g", "g", "g"}};
    return list;
}

QList<QList<QString>> KeyboardPC::getButtonLabels()
{
    QList<QList<QString>> list;
    list = {{"Ab|G#", "B", "D", "F", "Ab|G#", "B", "D", "F", "Ab|G#", "B", "D", "F", "Ab|Gis", "B"},
            {"A", "C", "Eb|D#", "Gb|F#", "A", "C", "Eb|D#", "Gb|F#", "A", "C", "Eb|D#", "Gb|F#", "A", "C"},
            {"Bb|A#", "Db|C#", "E", "G", "Bb|A#", "Db|C#", "E", "G", "Bb|A#", "Db|C#", "E", "G", "Bb|A#"},
            {"Ab|G#", "B", "D", "F", "Ab|G#", "B", "D", "F", "Ab|G#", "B", "D" ,"F", "Ab|G#"},
            {"ctrl", "\u2318", "alt", " ", "←", "↓", "↑", "→", "alt", "\u2325", "ctrl"}};
    return list;
}

QList<QList<int>> KeyboardPC::getButtonKeycodes()
{
    QList<QList<int>> list;
    list = {{16781906, 49, 50, 51, 52, 53, 54, 55, 56, 57, 48, 45, 16781904, 16777219},
            {16777217, 88, 86, 76, 67, 87, 75, 72, 71, 70, 81, 223, 16781905, 16777220},
            {16781571, 85, 73, 65, 69, 79, 83, 78, 82, 84, 68, 89, 16781571},
            {16777252, 0, 220, 214, 196, 80, 90, 66, 77, 44, 46, 74, 16777252},
            {}};
    return list;
}

QList<QList<int>> KeyboardPC::getMIDICodes()
{
    QList<QList<int>> list;
    list = {{8, 11, 14, 17, 20, 23, 26, 29, 32, 35, 38, 41, 44, 47},
            {9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48},
            {10, 13, 16, 19, 22, 25, 28, 31, 34, 37, 40, 43, 46},
            {8, 11, 14, 17, 20, 23, 26, 29, 32, 35, 38, 41, 44},
            {}};
    return list;
}

void KeyboardPC::keyDown(int keycode)
{
    QList<QList<int>> keycodes = getButtonKeycodes();
    for (int row=0; row < keycodes.length(); row++)
    {
        for (int col=0; col < keycodes.at(row).length(); col++)
        {
            if (keycodes.at(row).at(col) == keycode)
            {
                this->list_of_buttons.at(row).at(col)->setDown(true);
                
                QList<QList<int>> midicodes = getMIDICodes();
                emit MIDIPress(midicodes.at(row).at(col));
            }
        }
    }
}
void KeyboardPC::keyUp(int keycode)
{
    QList<QList<int>> keycodes = getButtonKeycodes();
    for (int row=0; row < keycodes.length(); row++)
    {
        for (int col=0; col < keycodes.at(row).length(); col++)
        {
            if (keycodes.at(row).at(col) == keycode)
            {
                this->list_of_buttons.at(row).at(col)->setDown(false);
                
                QList<QList<int>> midicodes = getMIDICodes();
                emit MIDIRelease(midicodes.at(row).at(col));
            }
        }
    }
}

void KeyboardPC::allKeysUp()
{
    QList<QList<int>> keycodes = getButtonKeycodes();
    for (int row=0; row < keycodes.length(); row++)
    {
        for (int col=0; col < keycodes.at(row).length(); col++)
        {
            this->list_of_buttons.at(row).at(col)->setDown(false);
        }
    }
}
