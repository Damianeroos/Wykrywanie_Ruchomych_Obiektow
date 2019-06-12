#ifndef PARAMWINDOW_H
#define PARAMWINDOW_H
/*!
  * \file
  * \brief Definicja klasy ParamWindow
  *
  * Plik zawiera definicję klasy MainWindow.
  */
#include <QWidget>
#include <opencv4/opencv2/opencv.hpp>

namespace Ui {
class ParamWindow;
}

/**
 * @brief Modeluje okno z parametrami algorytmu odejmowania tła.
 *
 * Klasa opisuje okno, które daje możliwość ustawienia parametrów algorytmu
 * modelowania tła. Opcję, które można ustawić: wypełnianie dziur, rozmiar jądra
 * , wartość progowania, filtrowanie gaussa.
 *
 */
class ParamWindow : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor, w którym ustawiane są wartości początkowe
     *
     * @param parent wskaźnik na rodzica widżetu
     */
    explicit ParamWindow(QWidget *parent = nullptr);
    /**
     * @brief destruktor klasy
     *
     */
    ~ParamWindow();

private slots:
    /**
     * @brief zapisuje wartość, którą ustawiono na suwaku progowania i emituje
     * odpowiedni sygnał
     *
     * @param position pozycja suwaka
     */
    void on_TreshildSlider_sliderMoved(int position);

    /**
     * @brief zapisuje wartość, którą ustawiono na suwaku rozmiaru jądra i emituje
     * odpowiedni sygnał
     *
     * @param position pozycja suwaka
     */
    void on_KernelSlider_sliderMoved(int position);

    /**
     * @brief zapisuje opcję filtrowania obrazu, którą ustawiono w okienku i emituje
     * odpowiedni sygnał
     *
     * @param checked stan przełącznika filtrowania
     */
    void on_setFilter_toggled(bool checked);


    /**
     * @brief zapisuje opcję wypełniania dziur obrazu, którą ustawiono
     * w okienku i emituje odpowiedni sygnał
     *
     * @param checked stan przełącznika wypełniania dziur
     */
    void on_setFillHoles_toggled(bool checked);

private:
    /**
     * @brief ui Pole modeluje wygląd graficzny widżetu
     */
    Ui::ParamWindow *ui;
    /**
     * @brief tresholdValue wartość progowania
     */
    int tresholdValue = 0;
    /**
     * @brief fillHoles opcja wypełniania dziur
     */
    bool fillHoles = false;
    /**
     * @brief kernelSize parametr rozmiaru jądra w algorytmie MOG2
     */
    cv::Size kernelSize;
signals:
 /**
  * @brief przekazuje informację o zmianie wartości rozmiaru jądra
  * do głównego okna aplikacji
  *
  * @param position rozmiar jądra
  */
 void kernelSizeChanged(int position);
 /**
  * @brief przekazuje informację o zmianie wartości progowania
  * do głównego okna aplikacji
  *
  * @param position rozmiar progowania
  */
 void tresholdChanged(int position);
 /**
  * @brief przekazuje informację o operacji filtrowania do głównego
  * okna aplikacji
  *
  * @param option  stan filtrowania
  */
 void gaussianFilterSet(bool option);
 /**
  * @brief przekazuje informację o operacji wypełniania dziur do głównego
  * okna aplikacji
  *
  * @param option stan operacji filtrowania dziur
  */
 void fillHolesSet(bool option);
};

#endif // PARAMWINDOW_H
