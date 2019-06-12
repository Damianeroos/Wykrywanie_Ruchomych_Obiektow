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

    int tresholdValue = 0; /**< TODO: describe */
    bool fillHoles = false; /**< TODO: describe */
    cv::Size kernelSize; /**< TODO: describe */
signals:
 /**
  * @brief
  *
  * @param position
  */
 void kernelSizeChanged(int position);
 /**
  * @brief
  *
  * @param position
  */
 void tresholdChanged(int position);
 /**
  * @brief
  *
  * @param option
  */
 void gaussianFilterSet(bool option);
 /**
  * @brief
  *
  * @param option
  */
 void fillHolesSet(bool option);
};

#endif // PARAMWINDOW_H
