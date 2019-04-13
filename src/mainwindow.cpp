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

    ui->rightView->setVisible(false);

    ui->PlayButton->setEnabled(false);
    ui->StopButton->setEnabled(false);
    ui->pauseButton->setEnabled(false);

    file_name = QString(); //ustawiamy stringa na NULL
    TresholdValue = 30;
    ui->TreshholdSlider->setValue(TresholdValue);
    kernelSize.width = 0;
    kernelSize.height = 0;
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
    ui->pauseButton->setEnabled(true);

    //wyświetlamy pierwszą ramkę i zmienamy rozmiar okna
    video >> frame;
    QImage qimg(frame.data, frame.cols, frame.rows,  static_cast<int>(frame.step), QImage::Format_RGB888);
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
    cv::Mat originalFrame,referenceFrame,binaryFrame,finalFrame;
    cv::Mat kernel;
    std::vector<std::vector<cv::Point> > finalContours,filledContours;


    if(!ComputeAverageBacgroundFrame(30,referenceFrame)){
        qDebug("gówno");
        return;
    }

    if(!video.isOpened()){
        video.open(file_name.toStdString());
    }


    //video>> frame;
    // tempframe = frame.clone();
    while(video.isOpened()){
        //sprawdzamy czy jest pauza
        if(!SetPause){
            video >> originalFrame;
        }
        //  frame = equalizeIntensity(frame);
        ui->leftView->fitInView(&leftPixmap,Qt::KeepAspectRatioByExpanding);
        ui->rightView->fitInView(&rightPixmap,Qt::KeepAspectRatioByExpanding);
        if(!originalFrame.empty()){

            /*przetwarzamy i wyswietlamy ramki*/


            cv::absdiff(originalFrame,referenceFrame,binaryFrame);
            //  cv::GaussianBlur(binaryFrame,binaryFrame, cv::Size(5,5),20);
            cv::cvtColor(binaryFrame, binaryFrame, cv::COLOR_BGR2GRAY);
            cv::threshold(binaryFrame,binaryFrame,TresholdValue,255,cv::THRESH_BINARY);
            if(kernelSize.width > 0){
                kernel.release();
                kernel  = cv::getStructuringElement(cv::MORPH_RECT,kernelSize);
                cv::erode(binaryFrame,binaryFrame,kernel);//erozja
                cv::morphologyEx(binaryFrame,binaryFrame,cv::MORPH_CLOSE,kernel); // zamkniecie (dylatacja -> erozja)
            }

            //znajdujemy, łączymy i rysujemy krawędzie

            if(FillHoles){
                cv::findContours(binaryFrame,filledContours,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);
                for(size_t i = 0 ; i < filledContours.size() ; i++){
                    cv::drawContours(binaryFrame,filledContours,int(i),cv::Scalar(255,255,255),cv::FILLED);
                }
            }
            cv::findContours(binaryFrame,finalContours,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);

            QImage qimg2(binaryFrame.data, binaryFrame.cols, binaryFrame.rows,  static_cast<int>(binaryFrame.step), QImage::Format_Grayscale8);
            rightPixmap.setPixmap(QPixmap::fromImage(qimg2.rgbSwapped()) );


            finalFrame = originalFrame.clone();
            cv::drawContours(finalFrame,finalContours,-1,cv::Scalar(0,255,0),5);
            std::vector<std::vector<cv::Point> >hull( finalContours.size() );
            for( size_t i = 0; i < finalContours.size(); i++ )
            {
                cv::convexHull( finalContours[i], hull[i] );
            }

            cv::drawContours(finalFrame,hull,-1,cv::Scalar(134,3,255),5);


            QImage qimg(finalFrame.data, finalFrame.cols, finalFrame.rows, static_cast<int>(finalFrame.step), QImage::Format_RGB888);
            leftPixmap.setPixmap(QPixmap::fromImage(qimg.rgbSwapped()) );


            if(cv::waitKey(30) >= 0) break;
        }
        else{
            video.release();
        }

        qApp->processEvents();
    }

    originalFrame.release();
    binaryFrame.release();
    finalFrame.release();
    referenceFrame.release();
    kernel.release();
}

/**
 * @brief
 *
 */
void MainWindow::on_StopButton_clicked()
{
    video.release();
}


void MainWindow::on_BinaryView_clicked(bool checked)
{
    if(checked){
        ui->rightView->setVisible(true);
    }
    else{
        ui->rightView->setVisible(false);
    }
}

void MainWindow::on_TreshholdSlider_sliderMoved(int position)
{
    TresholdValue = position;
}

void MainWindow::on_pauseButton_clicked()
{
    if(SetPause){
        SetPause = false;
    }
    else{
        SetPause = true;
    }
}

void MainWindow::on_kernelSlider_sliderMoved(int position)
{
    kernelSize = cv::Size(position,position);
}

void MainWindow::on_fillHoles_clicked(bool checked)
{
    FillHoles = checked;
}
