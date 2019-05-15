#include "inc/paramwindow.h"
#include "ui_paramwindow.h"

ParamWindow::ParamWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParamWindow)
{
    ui->setupUi(this);

    TresholdValue = 30;
    kernelSize.width = 7;
    kernelSize.height = 7;

    ui->TreshildSlider->setValue(TresholdValue);
    ui->showTreshVal->setNum(TresholdValue);
    ui->KernelSlider->setValue(kernelSize.width);
    ui->showKernValue->setNum(kernelSize.width);
}

ParamWindow::~ParamWindow()
{
    delete ui;
}

void ParamWindow::on_TreshildSlider_sliderMoved(int position)
{
    TresholdValue = position;
    ui->showTreshVal->setNum(TresholdValue);
    emit TresholdChanged(position);
}

void ParamWindow::on_KernelSlider_sliderMoved(int position)
{
    kernelSize.height = kernelSize.width = position;
    ui->showKernValue->setNum(position);
    emit KernelSizeChanged(position);
}


void ParamWindow::on_setFilter_toggled(bool checked)
{
    emit GaussianFilterSet(checked);
}

void ParamWindow::on_setFillHoles_toggled(bool checked)
{
    emit FillHolesSet(checked);
}
