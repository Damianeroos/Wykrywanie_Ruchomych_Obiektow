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
const double STEP_GATE_POSITION  = 0.1;
const unsigned int MAX_FRAMES = 128;
const unsigned int MIN_FRAMES = 30;
/**
 * @brief Funkcja wylicza średni obraz tła z pliku video.
 *
 * @param NumberOfFrames liczba ramek, z których obliczymy średni obraz tła.
 * @param avrFrame wyliczony średni obraz tła
 * @return bool stan zakończenia działania metody
 *  true  - sukces
 *  false - nie udało się wyliczyć średniej ramki tła
 */
bool computeAverageBacgroundFrame(unsigned int numberOfFrames
                                  ,cv::Mat & avrFrame);

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
QGraphicsPixmapItem leftPixmap;
QGraphicsPixmapItem rightPixmap;

/**
 * @brief Uchwyt na plik video, który otwieramy lub bierzemy z kamery
 */
cv::VideoCapture video;
/**
 * @brief poziom progowania (od 0 do 255)
 */
int tresholdValue = 0;
/**
 * @brief Określa czy film jest spauzowany
 */
bool setPause = false;
/**
 * @brief Określa czy wypełnionio dziury
 */
bool fillHoles = false;
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
QString fileName;
/**
 * @brief Odtawrza film, przetwarza go i wyświetla
 * @return 0 jeżeli sukces; -1 jeżeli błąd
 */
int playVideo(void);
/**
 * @brief x_gate Współrzędna pozioma bramki w zakresie od 0.0 do 1.0
 */
double xGate;
/**
 * @brief y_gate Współrzędna pionowa bramki w zakresie od 0.0 do 1.0
 */
double yGate;
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
void drawGate(cv::Mat &image, std::vector<cv::Point2f> &centers);
/**
 * @brief gateIsEmpty jeżeli jakiś obiekt jest w bramce to false, intaczej true
 */
bool gateIsEmpty;
/**
 * @brief CheckGateAndCounter sprawdza czy jakiś obiekt jest w bramce, aktualizauje zmienna globalna gateIsEmpty i uaktualnia licznik
 * @param x0 lewy górny róg bramki
 * @param y0 lewy górny róg bramki
 * @param x1 prawy dolny róg bramki
 * @param y1 prawy dolny róg bramki
 * @param centers środki okręgów opisanych na wykrytych obiketach
 */
void checkGateAndCounter(const int x0, const int y0,const int x1, const int y1
                         ,const std::vector<cv::Point2f> &centers);
/**
 * @brief GateCounter licznik obiektów, które zostały wykryte w bramce
 */
unsigned int gateCounter;


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
    void paramWindow_KernelSize_set(int position);
    /**
     * @brief
     *
     * @param position
     */
    void paramWindow_Treshold_set(int position);
    /**
     * @brief
     *
     * @param option
     */
    void paramWindow_GaussFilter_set(bool option);
    /**
     * @brief
     *
     * @param option
     */
    void paramWindow_FillHoles_set(bool option);



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

    /**
     * @brief
     *
     */
    void on_downButton_clicked();

    /**
     * @brief
     *
     */
    void on_leftButton_clicked();

    /**
     * @brief
     *
     */
    void on_rightButton_clicked();

    /**
     * @brief
     *
     */
    void on_thinButton_clicked();

    /**
     * @brief
     *
     */
    void on_rotateButton_clicked();

    /**
     * @brief
     *
     */
    void on_clearCounterButton_clicked();

private:
    Ui::MainWindow *ui; /**< TODO: describe */

};



#endif // MAINWINDOW_H
