#include "inc/paramwindow.h"
#include "ui_paramwindow.h"

ParamWindow::ParamWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParamWindow)
{
    ui->setupUi(this);

    tresholdValue = 30;
    kernelSize.width = 7;
    kernelSize.height = 7;

    ui->TreshildSlider->setValue(tresholdValue);
    ui->showTreshVal->setNum(tresholdValue);
    ui->KernelSlider->setValue(kernelSize.width);
    ui->showKernValue->setNum(kernelSize.width);
}

ParamWindow::~ParamWindow()
{
    delete ui;
}

void ParamWindow::on_TreshildSlider_sliderMoved(int position)
{
    tresholdValue = position;
    ui->showTreshVal->setNum(tresholdValue);
    emit tresholdChanged(position);
}

void ParamWindow::on_KernelSlider_sliderMoved(int position)
{
    kernelSize.height = kernelSize.width = position;
    ui->showKernValue->setNum(position);
    emit kernelSizeChanged(position);
}


void ParamWindow::on_setFilter_toggled(bool checked)
{
    emit gaussianFilterSet(checked);
}

void ParamWindow::on_setFillHoles_toggled(bool checked)
{
    emit fillHolesSet(checked);
}
