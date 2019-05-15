#ifndef PARAMWINDOW_H
#define PARAMWINDOW_H

#include <QWidget>
#include <opencv4/opencv2/opencv.hpp>

namespace Ui {
class ParamWindow;
}

/**
 * @brief
 *
 */
class ParamWindow : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief
     *
     * @param parent
     */
    explicit ParamWindow(QWidget *parent = nullptr);
    /**
     * @brief
     *
     */
    ~ParamWindow();

private slots:
    /**
     * @brief
     *
     * @param position
     */
    void on_TreshildSlider_sliderMoved(int position);

    /**
     * @brief
     *
     * @param position
     */
    void on_KernelSlider_sliderMoved(int position);

    /**
     * @brief
     *
     * @param checked
     */
    void on_setFilter_toggled(bool checked);


    /**
     * @brief
     *
     * @param checked
     */
    void on_setFillHoles_toggled(bool checked);

private:
    Ui::ParamWindow *ui; /**< TODO: describe */

    int TresholdValue = 0; /**< TODO: describe */
    bool FillHoles = false; /**< TODO: describe */
    cv::Size kernelSize; /**< TODO: describe */
signals:
 /**
  * @brief
  *
  * @param position
  */
 void KernelSizeChanged(int position);
 /**
  * @brief
  *
  * @param position
  */
 void TresholdChanged(int position);
 /**
  * @brief
  *
  * @param option
  */
 void GaussianFilterSet(bool option);
 /**
  * @brief
  *
  * @param option
  */
 void FillHolesSet(bool option);
};

#endif // PARAMWINDOW_H
