#include "inc/mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
/**
 * @brief
 *
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->leftView->setScene(new QGraphicsScene(this));
    ui->rightView->setScene(new QGraphicsScene(this));
    ui->leftView->scene()->addItem(&leftPixmap);
    ui->rightView->scene()->addItem(&rightPixmap);

    ui->PlayButton->setEnabled(false);
    ui->StopButton->setEnabled(false);
}

/**
 * @brief
 *
 */
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(video.isOpened()){
        QMessageBox::warning(this,"Uwaga!","Nie zamknięto video!");
        event->ignore();
    }
    else{
        event->accept();
    }
}

/**
 * @brief
 *
 */
void MainWindow::on_OpenFile_clicked()
{
    cv::Mat frame;

    file_name = QFileDialog::getOpenFileName(this,"Otwórz plik video","/home/damian/Wideo");

    if(!video.open(file_name.toStdString())){
        QMessageBox::critical(this,"Nie wczytno pliku video!","Upewnij się czy wybrany plik jest plikiem video.");
        return;
    }
    ui->PlayButton->setEnabled(true);
    ui->StopButton->setEnabled(true);

    //wyświetlamy pierwszą ramkę i zmienamy rozmiar okna
    video >> frame;
    QImage qimg(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
    leftPixmap.setPixmap(QPixmap::fromImage(qimg.rgbSwapped()) );
    ui->leftView->fitInView(&leftPixmap,Qt::KeepAspectRatioByExpanding);
}

/**
 * @brief
 *
 */
void MainWindow::on_PlayButton_clicked()
{
    cv::Mat frame;
    cv::Mat background_frame;

    if(!video.isOpened()){
        video.open(file_name.toStdString());
    }

    //pobieramy 30 pierwszysch ramek i tworzymy średni obraz tła
    while(video.isOpened()){
        for(int i = 0 ; i < 30 ; i++){
            video >> frame;
            background_frame += frame;
        }
    }
    background_frame /= 30;
    video.release();
    if(!video.isOpened()){
        video.open(file_name.toStdString());
    }
    //***********************************************
    while(video.isOpened()){
        video >> frame;
        ui->leftView->fitInView(&leftPixmap,Qt::KeepAspectRatioByExpanding);
        if(!frame.empty()){
            /*przetwarzamy i wyswietlamy ramki*/
            QImage qimg(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
            leftPixmap.setPixmap(QPixmap::fromImage(qimg.rgbSwapped()) );


            if(cv::waitKey(30) >= 0) break;
        }
        else{
            video.release();
        }
        qApp->processEvents();
    }


}

/**
 * @brief
 *
 */
void MainWindow::on_StopButton_clicked()
{
    video.release();
}

