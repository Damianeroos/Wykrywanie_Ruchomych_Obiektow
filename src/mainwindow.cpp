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

    file_name = QString(); //ustawiamy stringa na NULL
}

/**
 * @brief
 *
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief
 *
 * @param event
 */
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

bool MainWindow::ComputeAverageBacgroundFrame(unsigned int NumberOfFrames, cv::Mat &avrFrame)
{
    cv::Mat tempFrame;

    if(NumberOfFrames == 0 || NumberOfFrames > 128){ //maksymalnie możemy wybrac 128 ramek
        return false;
    }
    if(file_name.isNull()){ //nie podano nazwy pliku video
        return false;
    }
    if(!video.open(file_name.toStdString())){ //nie można otworzyć pliku
        return false;
    }

    video >> tempFrame;
    avrFrame = tempFrame.clone();
    avrFrame.convertTo(avrFrame,CV_32F);
    cv::accumulate(tempFrame,avrFrame);
    for(unsigned int i = 0 ; i < NumberOfFrames - 1 ; i++){
        video >> tempFrame;
        if(tempFrame.empty()){
            tempFrame.release();
            return false;
        }
        else{
            cv::accumulate(tempFrame,avrFrame);
        }
    }

    avrFrame.convertTo(avrFrame,CV_8U,1.0/NumberOfFrames);
    tempFrame.release();
    video.release();

    return true;
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

    frame.release();
    video.release();
}

/**
 * @brief
 *
 */
void MainWindow::on_PlayButton_clicked()
{
    cv::Mat frame,tempframe,frame2;

    if(!ComputeAverageBacgroundFrame(30,tempframe)){
        qDebug("gówno");
        return;
    }

    if(!video.isOpened()){
        video.open(file_name.toStdString());
    }


    video>> frame;
   // tempframe = frame.clone();
    while(video.isOpened()){
        video >> frame;
        ui->leftView->fitInView(&leftPixmap,Qt::KeepAspectRatioByExpanding);
        ui->rightView->fitInView(&rightPixmap,Qt::KeepAspectRatioByExpanding);
        if(!frame.empty()){
            /*przetwarzamy i wyswietlamy ramki*/
            QImage qimg(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
            leftPixmap.setPixmap(QPixmap::fromImage(qimg.rgbSwapped()) );

            //cv::subtract(tempframe,frame,frame2);
            //frame2 = frame - tempframe;
            //cv::subtract(frame,tempframe,frame2);
//            cv::GaussianBlur(frame2,frame2, cv::Size(5,5),0);
//            cv::cvtColor(frame2, frame2, cv::COLOR_BGR2GRAY);
//            cv::threshold(frame2,frame2,30,255,cv::THRESH_BINARY);
            frame2 = tempframe;
            //cv::cvtColor(frame2,frame2,cv::COLOR_GRAY2BGR);
            QImage qimg2(frame2.data, frame2.cols, frame2.rows, frame2.step, /*QImage::Format_Grayscale8*/ QImage::Format_RGB888);
            rightPixmap.setPixmap(QPixmap::fromImage(qimg2.rgbSwapped()) );

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

