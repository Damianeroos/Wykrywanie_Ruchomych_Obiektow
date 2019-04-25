#include "virtualgate.h"
#include "ui_virtualgate.h"

VirtualGate::VirtualGate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VirtualGate)
{
    ui->setupUi(this);
}

VirtualGate::~VirtualGate()
{
    delete ui;
}
