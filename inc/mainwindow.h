//testy funkcjonalne
//coding convetions


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QPixmap>
#include <QCloseEvent>
#include <QMessageBox>
#include <QStyle>

#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>



#include <vector>

#include "paramwindow.h"

namespace Ui {
class MainWindow;
}

/**
 * @brief
 *
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief
     *
     * @param parent
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /**
     * @brief
     *
     */
    ~MainWindow();
void closeEvent(QCloseEvent *event);

private:
/**
 * @brief Funkcja wylicza średni obraz tła z pliku video.
 *
 * @param NumberOfFrames liczba ramek, z których obliczymy średni obraz tła.
 * @param avrFrame wyliczony średni obraz tła
 * @return bool stan zakończenia działania metody
 *  true  - sukces
 *  false - nie udało się wyliczyć średniej ramki tła
 */
bool ComputeAverageBacgroundFrame(unsigned int NumberOfFrames,cv::Mat & avrFrame);

cv::Mat equalizeIntensity(const cv::Mat& inputImage);

QGraphicsPixmapItem leftPixmap,rightPixmap; /**< TODO: describe */
cv::VideoCapture video; /**< TODO: describe */
int TresholdValue = 0;
bool SetPause = false;
bool FillHoles = false;
bool setGaussianFilter = false;
cv::Size kernelSize;
ParamWindow parWin;
/**
 * @brief file_name
 */
QString file_name;
int PlayVideo(void);


private slots:
    /**
     * @brief
     *
     */
    void on_OpenFile_clicked();



    /**
     * @brief
     *
     */
    void on_StopButton_clicked();

    void on_BinaryView_clicked(bool checked);






    void on_paramButton_clicked();

    void on_paramWindow_KernelSize_set(int position);
    void on_paramWindow_Treshold_set(int position);
    void on_paramWindow_GaussFilter_set(bool option);
    void on_paramWindow_FillHoles_set(bool option);



    void on_PlayButton_clicked();

    void on_CameraButton_clicked();

private:
    Ui::MainWindow *ui; /**< TODO: describe */

};

#endif // MAINWINDOW_H
