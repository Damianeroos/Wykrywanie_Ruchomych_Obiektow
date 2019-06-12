#ifndef MAINWINDOW_H
#define MAINWINDOW_H
/*!
  * \file
  * \brief Definicja klasy MainWindow
  *
  * Plik zawiera definicję klasy MainWindow.
  */
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
 * Klasa  steruje wszystkimi obiektami ekranu głównego oraz odpowiada
 * za poprawne przetwarzane sekwencji obrazów i wyświetlenie ich
 *
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor
     *
     * Konstruktor modyfikuje przyciski (dodaje ikony). Ustawiane są wartości
     *  początkowe.
     *
     * @param parent wskaźnik na rodzica
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
    * ekranu głównoego należy pozamykać ekran doadtkowy ekran z parametrami.
    *
    * @param event sygnał zamknięcia okna
    */
void closeEvent(QCloseEvent *event);
/**
 * @brief keyPressEvent Opisuje reakcje na wciśnięcie klawiszy
 *
 * Funkcja emituje sygnały na podstawie przyciśniętego klawisza kalawiatury
 *
 * @param event sygnał wciśnięcia jakiegoś przycisku
 */
void keyPressEvent(QKeyEvent * event);

private:
/**
 * @brief STEP_GATE_POSITION Maksymalny krok zmiany pozycji bramki
 *
 * Określa maksymalny krok o jaki bramka może się przemiścić w stosunku do
 * długości i szerokości
 */
const double STEP_GATE_POSITION  = 0.1;
/**
 * @brief MAX_FRAMES Maksymalna liczba ramek modelujących obraz tła
 */
const unsigned int MAX_FRAMES = 128;

/**
 * @brief MIN_FRAMES Minimalny obraz ramek modelujących obraz tła
 */

const unsigned int MIN_FRAMES = 30;
/**
 * @brief Funkcja wylicza średni obraz tła z pliku video.
 *
 * Funkcja wylicza średni obraz tła na średniej wartości piklesa z minimalnej
 * liczby ramek.
 *
 * @param numberOfFrames liczba ramek, z których obliczymy średni obraz tła.
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
 * @param inputImage obraz wejściowy, który zostaje zmodyfikowany
 *
 * Funkcja zwieksza nasycenie kolorów na podstawie operacji rozciągnięcia
 * histogramu
 *
 * @return zwraca zmodyfikowany obraz
 */
cv::Mat equalizeIntensity(const cv::Mat& inputImage);
/**
 * @brief Mapa pikselowa wyswietlanego obrazu po lewej stronie aplikacji
 */
QGraphicsPixmapItem leftPixmap;
/**
 * @brief Mapa pikselowa wyswietlanego obrazu po prawej stronie aplikacji
 */
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
 * @brief Określa czy film jest wstrzymany
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
 * @brief Okno umożliwiające ustawianie paramterów aplikacji
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
 * @brief rotation Jeżeli false, to bramka jest pionowa. Jeżeli true, to
 * bramka jest  pozioma
 */
bool rotation;
/**
 * @brief drawGate funkcja rysuje bramkę na obrazie
 *
 * @param image obraz, na którym rysowana jest bramka
 * @param centers punkty opisujące położenie środka wykrytych obiektów
 */
void drawGate(cv::Mat &image, std::vector<cv::Point2f> &centers);
/**
 * @brief gateIsEmpty jeżeli  obiekt jest w bramce to false, intaczej true
 */
bool gateIsEmpty;
/**
 * @brief CheckGateAndCounter sprawdza czy  obiekt jest w bramce,
 *  aktualizauje zmienna globalna gateIsEmpty i uaktualnia licznik.
 *
 * Jeżeli zostanie wykryte przejście przez obiekt to funkcja zwiększa licznik
 * bramki wirtualnej.
 *
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
     * @brief Otwiera eksplorer wczytytywania plików, zapisuje wybrany plik
     *  i sprawdza poprawność formatu.
     *
     */
    void on_OpenFile_clicked();



    /**
     * @brief Zatrzymuje odtwarzanie pliku video i umożliwia zamknięcie aplikacji
     *
     */
    void on_StopButton_clicked();

    /**
     * @brief Otwiera po prawej stronie aplikacji obraz binarny
     * modyfikowanego obrazu
     * @param checked
     */
    void on_BinaryView_clicked(bool checked);
    /**
     * @brief Otwiera okno z parametrami algorytmu
     *
     */
    void on_paramButton_clicked();

    /**
     * @brief Zapisuje parametr rozmiaru jądra, który został ustawiony w oknie
     *  z paramterami.
     *
     * @param position wartość rozmiaru jądra
     */
    void paramWindow_KernelSize_set(int position);
    /**
     * @brief Zapisuje parametr progowania, który został ustawiony w oknie
     *  z paramterami.
     *
     * @param position wartość progowania
     */
    void paramWindow_Treshold_set(int position);
    /**
     * @brief Zapisuje akutalne ustawienie opcji gaussowskiego filtrowania tła,
     * które zostało zmienione w oknie z parametrami
     *
     * @param option stan parametru filtrowania
     */
    void paramWindow_GaussFilter_set(bool option);
    /**
     * @brief Zapisuje akutalne ustawienie opcji wypełniania dziur,
     * które zostało zmienione w oknie z parametrami
     *
     * @param option stan paramteru wypełniania dziur
     */
    void paramWindow_FillHoles_set(bool option);
    /**
     * @brief Wywołuje funkcję przetwarzania i odtwarzania wybranego pliku video
     *
     */
    void on_PlayButton_clicked();

    /**
     * @brief Wywołuje funkcję przetwarzania i odtwarzania obrazu z kamery
     *
     */
    void on_CameraButton_clicked();


    /**
     * @brief on_upButton_clicked Przesuwa wirtualną bramkę w górę
     */
    void on_upButton_clicked();

    /**
     * @brief Przesuwa wirtualną bramkę w dół
     *
     */
    void on_downButton_clicked();

    /**
     * @brief Przesuwa wirtualną bramkę w lewo
     *
     */
    void on_leftButton_clicked();

    /**
     * @brief Przesuwa wirtualną bramkę w prawo
     *
     */
    void on_rightButton_clicked();

    /**
     * @brief Zmienia rozmiar wirtualnej bramki
     *
     */
    void on_thinButton_clicked();

    /**
     * @brief Obraca wirtualną  bramkę o 90 stopni
     *
     */
    void on_rotateButton_clicked();

    /**
     * @brief Zeruje licznik wirtualnej bramki
     *
     */
    void on_clearCounterButton_clicked();

private:
    /**
     * @brief ui Pole modeluje wygląd graficzny aplikacji
     */
    Ui::MainWindow *ui;

};



#endif // MAINWINDOW_H
