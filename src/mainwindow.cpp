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
    // tempFrame = equalizeIntensity(tempFrame);
    avrFrame = tempFrame.clone();
    avrFrame.convertTo(avrFrame,CV_32F);
    cv::accumulate(tempFrame,avrFrame);
    for(unsigned int i = 0 ; i < NumberOfFrames - 1 ; i++){
        video >> tempFrame;
        //  tempFrame = equalizeIntensity(tempFrame);
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

cv::Mat MainWindow::equalizeIntensity(const cv::Mat &inputImage)
{
    if(inputImage.channels() >= 3)
    {
        cv::Mat ycrcb;

        cv::cvtColor(inputImage,ycrcb,cv::COLOR_BGR2YCrCb);

        std::vector<cv::Mat> channels;
        split(ycrcb,channels);

        cv::equalizeHist(channels[0], channels[0]);

        cv::Mat result;
        merge(channels,ycrcb);
        cv::cvtColor(ycrcb,result,cv::COLOR_YCrCb2BGR);

        return result;
    }

    return cv::Mat();
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
    cv::Mat frame,tempframe,frame2,frame3;
    std::vector<std::vector<cv::Point> > contours;


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
        //  frame = equalizeIntensity(frame);
        ui->leftView->fitInView(&leftPixmap,Qt::KeepAspectRatioByExpanding);
        ui->rightView->fitInView(&rightPixmap,Qt::KeepAspectRatioByExpanding);
        if(!frame.empty()){
            /*przetwarzamy i wyswietlamy ramki*/

            //cv::subtract(frame,tempframe,frame2);
            cv::absdiff(frame,tempframe,frame2);
            cv::GaussianBlur(frame2,frame2, cv::Size(5,5),20);
            //cv::medianBlur(frame2,frame2,3);
            cv::cvtColor(frame2, frame2, cv::COLOR_BGR2GRAY);
            cv::threshold(frame2,frame2,50,255,cv::THRESH_BINARY);
            QImage qimg2(frame2.data, frame2.cols, frame2.rows, frame2.step, QImage::Format_Grayscale8);
            rightPixmap.setPixmap(QPixmap::fromImage(qimg2.rgbSwapped()) );

            //znajdujemy, łączymy i rysujemy krawędzie
            cv::findContours(frame2,contours,cv::RETR_TREE,cv::CHAIN_APPROX_SIMPLE);
            cv::drawContours(frame,contours,-1,cv::Scalar(0,255,0),5);
            std::vector<std::vector<cv::Point> >hull( contours.size() );
            for( size_t i = 0; i < contours.size(); i++ )
            {
                cv::convexHull( contours[i], hull[i] );
            }

                cv::drawContours(frame,hull,-1,cv::Scalar(134,3,255),5);


            QImage qimg(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
            leftPixmap.setPixmap(QPixmap::fromImage(qimg.rgbSwapped()) );


            if(cv::waitKey(30) >= 0) break;
        }
        else{
            video.release();
        }
        qApp->processEvents();
    }

    frame.release();
    frame2.release();
    tempframe.release();
}

/**
 * @brief
 *
 */
void MainWindow::on_StopButton_clicked()
{
    video.release();
}

