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
#include <QKeyEvent>

#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

#define STEP 0.1 //krok o jaki zmieniamy połozenie bramki

#include <vector>

#include "paramwindow.h"

namespace Ui {
class MainWindow;
}

/**
 * @brief Modeluje ekran główny aplikacji
 *
 * Klasa zawiera wszystkie obiekty ekranu głównego
 *
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor
     *
     * @param wskaźnik na rodzica
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destruktor
     *
     */
    ~MainWindow();
    /**
    * @brief closeEvent
    *
    * Funkcja wywołuje destruktor obiektu ParamWindow. Przed zamknięciem
    * ekranu głównoego należy pozamykać wszystkie potomka.
    *
    * @param event
    */
void closeEvent(QCloseEvent *event);
/**
 * @brief keyPressEvent reagujemy na klawisze
 * @param event
 */
void keyPressEvent(QKeyEvent * event);

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

/**
 * @brief Zwiększa intensywność barw obrazu
 *
 * @param obraz wejściowy, który mam zmodyfikować
 *
 * @return zwraca zmodyfikowany obraz
 */
cv::Mat equalizeIntensity(const cv::Mat& inputImage);
/**
 * @brief Mapy pikselowe, na których wyswietlamy obrazy
 */
QGraphicsPixmapItem leftPixmap,rightPixmap;

/**
 * @brief Uchwyt na plik video, który otwieramy lub bierzemy z kamery
 */
cv::VideoCapture video;
/**
 * @brief poziom progowania (od 0 do 255)
 */
int TresholdValue = 0;
/**
 * @brief Określa czy film jest spauzowany
 */
bool SetPause = false;
/**
 * @brief Określa czy wypełnionio dziury
 */
bool FillHoles = false;
/**
 * @brief Określa czy obraz filtrujemy (filtr Gaussa)
 */
bool setGaussianFilter = false;
/**
 * @brief Rozmiar jądra operacji morfologicznych (dylatacja i erozja)
 */
cv::Size kernelSize;
/**
 * @brief Okno parametrów
 */
ParamWindow parWin; /**< TODO: describe */
/**
 * @brief Nazwa pliku wideo, który otwieramy
 */
QString file_name;
/**
 * @brief Odtawrza film, przetwarza go i wyświetla
 * @return 0 jeżeli sukces; -1 jeżeli błąd
 */
int PlayVideo(void);
/**
 * @brief x_gate Współrzędna pozioma bramki w zakresie od 0.0 do 1.0
 */
double x_gate;
/**
 * @brief y_gate Współrzędna pionowa bramki w zakresie od 0.0 do 1.0
 */
double y_gate;
/**
 * @brief length_gate Długość bramki w zakresie od 0.0 do 1.0
 */
double length_gate;
/**
 * @brief rotation Jezeli false to pionowo. Jezeleni true to poziomo
 */
bool rotation;
/**
 * @brief drawGate funkcja syruje bramkę na obrazie
 */
void drawGate(cv::Mat &image);





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

    /**
     * @brief on_BinaryView_clicked
     * @param checked
     */
    void on_BinaryView_clicked(bool checked);






    /**
     * @brief
     *
     */
    void on_paramButton_clicked();

    /**
     * @brief
     *
     * @param position
     */
    void on_paramWindow_KernelSize_set(int position);
    /**
     * @brief
     *
     * @param position
     */
    void on_paramWindow_Treshold_set(int position);
    /**
     * @brief
     *
     * @param option
     */
    void on_paramWindow_GaussFilter_set(bool option);
    /**
     * @brief
     *
     * @param option
     */
    void on_paramWindow_FillHoles_set(bool option);



    /**
     * @brief
     *
     */
    void on_PlayButton_clicked();

    /**
     * @brief
     *
     */
    void on_CameraButton_clicked();

    void on_upButton_clicked();

    void on_downButton_clicked();

    void on_leftButton_clicked();

    void on_rightButton_clicked();

    void on_thinButton_clicked();

    void on_rotateButton_clicked();

private:
    Ui::MainWindow *ui; /**< TODO: describe */

};



#endif // MAINWINDOW_H
