#ifndef PARAMWINDOW_H
#define PARAMWINDOW_H

#include <QWidget>
#include <opencv4/opencv2/opencv.hpp>

namespace Ui {
class ParamWindow;
}

class ParamWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ParamWindow(QWidget *parent = nullptr);
    ~ParamWindow();

private slots:
    void on_TreshildSlider_sliderMoved(int position);

    void on_KernelSlider_sliderMoved(int position);

    void on_setFilter_toggled(bool checked);


    void on_setFillHoles_toggled(bool checked);

private:
    Ui::ParamWindow *ui;

    int TresholdValue = 0;
    bool FillHoles = false;
    cv::Size kernelSize;
signals:
 void KernelSizeChanged(int position);
 void TresholdChanged(int position);
 void GaussianFilterSet(bool option);
 void FillHolesSet(bool option);
};

#endif // PARAMWINDOW_H
