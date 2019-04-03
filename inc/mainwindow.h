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

#include <opencv2/opencv.hpp>

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
     * @brief file_name
     */
    QString file_name;
    /**
     * @brief
     *
     */
    ~MainWindow();


private slots:
    /**
     * @brief
     *
     */
    void on_OpenFile_clicked();

    void on_PlayButton_clicked();

    void on_StopButton_clicked();

private:
    Ui::MainWindow *ui; /**< TODO: describe */
};

#endif // MAINWINDOW_H
