#ifndef VIRTUALGATE_H
#define VIRTUALGATE_H

#include <QWidget>

namespace Ui {
class VirtualGate;
}

class VirtualGate : public QWidget
{
    Q_OBJECT

public:
    explicit VirtualGate(QWidget *parent = nullptr);
    ~VirtualGate();

private:
    Ui::VirtualGate *ui;
    unsigned int x1,x2,y1,y2;
};

#endif // VIRTUALGATE_H
