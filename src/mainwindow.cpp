#include "inc/mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <opencv2/video/background_segm.hpp>
#include <sstream>

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

    fileName = QString(); //ustawiamy stringa na NULL
    //wartości inicjalizacyjne
    tresholdValue = 30;
    kernelSize.width = 7;
    kernelSize.height = 7;

    ui->PlayButton->setText("");
    ui->StopButton->setText("");
    ui->OpenFile->setText("");
    ui->paramButton->setText("");
    ui->CameraButton->setText("");
    ui->upButton->setText("");
    ui->downButton->setText("");
    ui->leftButton->setText("");
    ui->rightButton->setText("");
    ui->rotateButton->setText("");
    ui->thinButton->setText("");
    ui->clearCounterButton->setText("");
    ui->PlayButton->setIconSize(QSize(50,50));
    ui->StopButton->setIconSize(QSize(50,50));
    ui->OpenFile->setIconSize(QSize(50,50));
    ui->paramButton->setIconSize(QSize(50,50));
    ui->CameraButton->setIconSize(QSize(50,50));
    ui->PlayButton->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_MediaPlay));
    ui->StopButton->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_MediaStop));
    ui->OpenFile->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_DirOpenIcon));
    ui->paramButton->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_FileDialogDetailedView));
    ui->CameraButton->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_ComputerIcon));
    ui->upButton->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_ArrowUp));
    ui->downButton->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_ArrowDown));
    ui->leftButton->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_ArrowLeft));
    ui->rightButton->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_ArrowRight));
    ui->rotateButton->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_BrowserReload));
    ui->thinButton->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_TitleBarMinButton));
    ui->clearCounterButton->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_DialogResetButton));

    connect(&parWin,&ParamWindow::kernelSizeChanged,this,&MainWindow::paramWindow_KernelSize_set);
    connect(&parWin,&ParamWindow::tresholdChanged,this,&MainWindow::paramWindow_Treshold_set);
    connect(&parWin,&ParamWindow::gaussianFilterSet,this,&MainWindow::paramWindow_GaussFilter_set);
    connect(&parWin,&ParamWindow::fillHolesSet,this,&MainWindow::paramWindow_FillHoles_set);

    xGate = 0;
    yGate = 0;
    length_gate = 0;
    rotation = false;
    gateCounter = 0;
    gateIsEmpty = true;
}


MainWindow::~MainWindow()
{
    parWin.close();
    delete ui;

}


void MainWindow::closeEvent(QCloseEvent *event)
{
    if(video.isOpened()==true){
        QMessageBox::warning(this,"Uwaga!","Nie zamknięto video!");
        event->ignore();
    }
    else{
        event->accept();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event){

    switch (event->key()) {
    case Qt::Key_R:{
        on_rotateButton_clicked();
        break;
    }
    case Qt::Key_T:{
        on_thinButton_clicked();
        break;
    }
    case Qt::Key_W:{
        on_upButton_clicked();
        break;
    }
    case Qt::Key_S:{
        on_downButton_clicked();
        break;
    }
    case Qt::Key_A:{
        on_leftButton_clicked();
        break;
    }
    case Qt::Key_D:{
        on_rightButton_clicked();
        break;
    }
    default:{
        ;//tu jest pusto
    }
    }
}

bool MainWindow::computeAverageBacgroundFrame(unsigned int NumberOfFrames
                                              ,cv::Mat &avrFrame)
{
    cv::Mat tempFrame;
    //za mało ramek
    if(NumberOfFrames == 0 || NumberOfFrames > MAX_FRAMES){
        return false;
    }
    if(fileName.isNull()==true){ //nie podano nazwy pliku video
        return false;
    }
    if(video.open(fileName.toStdString())==false){ //nie można otworzyć pliku
        return false;
    }
    video >> tempFrame;
    avrFrame = tempFrame.clone();
    avrFrame.convertTo(avrFrame,CV_32F);//zwiększamy maks. wartość piksela
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

cv::Mat MainWindow::equalizeIntensity(const cv::Mat &inputImage)
{
    //jeżeli obraz składa się z 3 kanałów to jest kolorowy
    if(inputImage.channels() >= 3)
    {
        cv::Mat ycrcb;
        cv::cvtColor(inputImage,ycrcb,cv::COLOR_BGR2YCrCb);
        std::vector<cv::Mat> channels;
        cv::Mat result;

        split(ycrcb,channels);
        cv::equalizeHist(channels[0], channels[0]);
        merge(channels,ycrcb);
        cv::cvtColor(ycrcb,result,cv::COLOR_YCrCb2BGR);

        return result;
    }

    return cv::Mat();
}

int MainWindow::playVideo()
{
    cv::Mat originalFrame,referenceFrame,binaryFrame,finalFrame;
    cv::Mat kernel;
    std::vector<std::vector<cv::Point> > finalContours,filledContours;
    cv::Mat fgMaskMOG2; //fg mask fg mask generated by MOG2 method
    cv::Ptr<cv::BackgroundSubtractorMOG2> pMOG2;
    pMOG2 = cv::createBackgroundSubtractorMOG2(); //MOG2 Background subtractor

    if(!computeAverageBacgroundFrame(MIN_FRAMES,referenceFrame)){
        qDebug("nie można obliczyć obrazu referencyjnego");
        return 0;
    }
    if(!video.isOpened()){
        video.open(fileName.toStdString());
    }
    video>> originalFrame;
    while(video.isOpened()){
        //sprawdzamy czy jest pauza
        if(!setPause){
            video >> originalFrame;
            pMOG2->apply(originalFrame,fgMaskMOG2);
        }
        ui->leftView->fitInView(&leftPixmap,Qt::KeepAspectRatioByExpanding);
        ui->rightView->fitInView(&rightPixmap,Qt::KeepAspectRatioByExpanding);
        if(!originalFrame.empty()){
            /*przetwarzamy i wyswietlamy ramki*/
            binaryFrame = fgMaskMOG2.clone();
            if(setGaussianFilter && !binaryFrame.empty()){
                cv::GaussianBlur(binaryFrame,binaryFrame, cv::Size(5,5),20);
            }
            if(kernelSize.width > 0 && !binaryFrame.empty()){
                kernel.release();
                kernel  = cv::getStructuringElement(cv::MORPH_RECT,kernelSize);
                cv::erode(binaryFrame,binaryFrame,kernel);//erozja
                // zamkniecie (dylatacja -> erozja)
                cv::morphologyEx(binaryFrame,binaryFrame,cv::MORPH_CLOSE,kernel);
            }
            //znajdujemy, łączymy i rysujemy krawędzie
            if(fillHoles && !binaryFrame.empty()){
                cv::findContours(binaryFrame,filledContours,cv::RETR_EXTERNAL
                                 ,cv::CHAIN_APPROX_SIMPLE);
                for(size_t i = 0 ; i < filledContours.size() ; i++){
                    cv::drawContours(binaryFrame,filledContours,int(i)
                                     ,cv::Scalar(255,255,255),cv::FILLED);
                }
            }
            cv::findContours(binaryFrame,finalContours,cv::RETR_EXTERNAL
                             ,cv::CHAIN_APPROX_SIMPLE);
            QImage qimg2(binaryFrame.data, binaryFrame.cols, binaryFrame.rows
                         ,static_cast<int>(binaryFrame.step)
                         , QImage::Format_Grayscale8);
            rightPixmap.setPixmap(QPixmap::fromImage(qimg2.rgbSwapped()) );
            finalFrame = originalFrame.clone();
            cv::drawContours(finalFrame,finalContours,-1,cv::Scalar(0,255,0),1);
            std::vector<std::vector<cv::Point> >hull( finalContours.size() );
            std::vector<cv::Point2f>centers( finalContours.size() );
            std::vector<float>radius( finalContours.size() );
            for( size_t i = 0; i < finalContours.size(); i++ )
            {
                approxPolyDP( finalContours[i], hull[i], 3, true );
                minEnclosingCircle( hull[i], centers[i], radius[i] );
            }
            for( size_t i = 0; i< finalContours.size(); i++ )
            {
                circle( finalFrame, centers[i]
                        , static_cast<int>(radius[i]*static_cast<float>(1.1))
                        ,cv::Scalar(134,3,255), 2 );

            }
            drawGate(finalFrame,centers);
            QImage qimg(finalFrame.data, finalFrame.cols, finalFrame.rows
                        ,static_cast<int>(finalFrame.step)
                        ,QImage::Format_RGB888);
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
    return 1;
}

void MainWindow::drawGate(cv::Mat &image, std::vector<cv::Point2f> &centers)
{  
    if(length_gate>0){
        cv::Size imageSize = image.size();
        int x0, y0,x1,y1;

        x0 = int(imageSize.width*xGate); //lewy górny róg bramki
        y0 = int(imageSize.height*yGate);
        //obliczamy prawy dolny róg bramki
        if(!rotation){ //bramka pozioma, wiec y1 to grubość
            y1 = int(y0+0.05*imageSize.width);
            x1 = int(x0+imageSize.width*length_gate);
            checkGateAndCounter( x0,  y0,  x1,  y1,  centers);
            if(gateIsEmpty){
                cv::rectangle(image,cv::Point(x0,y0),cv::Point(x1,y1)
                              ,cv::Scalar(255,0,0),3);
            }
            else{
                cv::rectangle(image,cv::Point(x0,y0),cv::Point(x1,y1)
                              ,cv::Scalar(0,255,0),3);
            }
        }
        else{ //bramka pionowa, wiec x1 to grubość
            x1 = int(x0+0.05*imageSize.width);
            y1 = int(y0+imageSize.height*length_gate);
            checkGateAndCounter( x0,  y0,  x1,  y1,  centers);

            if(gateIsEmpty){
                cv::rectangle(image,cv::Point(x0,y0),cv::Point(x1,y1)
                              ,cv::Scalar(255,0,0),3);
            }
            else{
                cv::rectangle(image,cv::Point(x0,y0),cv::Point(x1,y1)
                              ,cv::Scalar(0,255,0),3);
            }
        }
    }
}

void MainWindow::checkGateAndCounter(const int x0, const int y0, const int x1
                                     , const int y1
                                     , const std::vector<cv::Point2f> &centers)
{
    bool objectInGate = false;
    int xp,yp;
    for( size_t i = 0; i< centers.size(); i++ )
    {
        xp = int(centers[i].x);
        yp = int(centers[i].y);

        if(x0 < xp && xp < x1 && y0 < yp && yp < y1) {
            objectInGate = true;
        }
    }

    if(objectInGate && gateIsEmpty){ //nowy obiekt w pustej ramce
        gateIsEmpty = false;
    }

    if(!objectInGate  && !gateIsEmpty){//nalezy zwiekszyć licznik
        gateCounter += 1;
        ui->lcdCounter->display(int(gateCounter));
        gateIsEmpty = true;
    }
}


void MainWindow::on_OpenFile_clicked()
{
    cv::Mat frame;



    fileName = QFileDialog::getOpenFileName(this,"Otwórz plik video"
                                            ,"/home/damian/Wideo");

    if(!video.open(fileName.toStdString())){
        QMessageBox::critical(this,"Nie wczytno pliku video!"
                              ,"Upewnij się czy wybrany plik jest plikiem video.");
        return;
    }
    ui->PlayButton->setEnabled(true);
    ui->StopButton->setEnabled(true);


    //wyświetlamy pierwszą ramkę i zmienamy rozmiar okna
    video >> frame;
    QImage qimg(frame.data, frame.cols, frame.rows,  static_cast<int>(frame.step)
                , QImage::Format_RGB888);
    leftPixmap.setPixmap(QPixmap::fromImage(qimg.rgbSwapped()) );
    ui->leftView->fitInView(&leftPixmap,Qt::KeepAspectRatioByExpanding);

    frame.release();
    video.release();
    setPause = true;
    playVideo();

    ui->PlayButton->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_MediaPlay));
    ui->StopButton->setEnabled(false);
    ui->PlayButton->setEnabled(false);
}


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






void MainWindow::on_paramButton_clicked()
{
    parWin.show();
}

void MainWindow::paramWindow_KernelSize_set(int position)
{
    kernelSize.width = kernelSize.height = position;
}

void MainWindow::paramWindow_Treshold_set(int position)
{
    tresholdValue = position;
}

void MainWindow::paramWindow_GaussFilter_set(bool option)
{
    setGaussianFilter = option;
}

void MainWindow::paramWindow_FillHoles_set(bool option)
{
    fillHoles = option;
}



void MainWindow::on_PlayButton_clicked()
{
    if(setPause){
        setPause = false;
        ui->PlayButton->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_MediaPause));
    }
    else {
        setPause = true;
        ui->PlayButton->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_MediaPlay));
    }
}

void MainWindow::on_CameraButton_clicked()
{
    if(video.isOpened()){
        video.release();
    }
    ui->StopButton->setEnabled(true);

    cv::Mat originalFrame,referenceFrame,binaryFrame,finalFrame;
    cv::Mat kernel;
    std::vector<std::vector<cv::Point> > finalContours,filledContours;
    cv::Mat fgMaskMOG2; //fg mask fg mask generated by MOG2 method
    cv::Ptr<cv::BackgroundSubtractorMOG2> pMOG2;
    pMOG2 = cv::createBackgroundSubtractorMOG2(); //MOG2 Background subtractor

    cv::VideoCapture cap(0); //przechwytujemy strumien z kamery

    if(!cap.isOpened()){  // check if we succeeded
        qDebug("Nie znaleziono kamery.");
        return ;
    }
    video = cap;
    video >> originalFrame;
    while(video.isOpened()){
        //sprawdzamy czy jest pauza
        if(!setPause){
            video >> originalFrame;
            pMOG2->apply(originalFrame,fgMaskMOG2);
        }

        ui->leftView->fitInView(&leftPixmap,Qt::KeepAspectRatioByExpanding);
        ui->rightView->fitInView(&rightPixmap,Qt::KeepAspectRatioByExpanding);
        if(!originalFrame.empty()){

            /*przetwarzamy i wyswietlamy ramki*/
            binaryFrame = fgMaskMOG2.clone();
            if(setGaussianFilter){
                cv::GaussianBlur(binaryFrame,binaryFrame, cv::Size(5,5),20);
            }

            if(kernelSize.width > 0){
                kernel.release();
                kernel  = cv::getStructuringElement(cv::MORPH_RECT,kernelSize);
                cv::erode(binaryFrame,binaryFrame,kernel);//erozja
                // zamkniecie (dylatacja -> erozja)
                cv::morphologyEx(binaryFrame,binaryFrame,cv::MORPH_CLOSE,kernel);
            }

            //znajdujemy, łączymy i rysujemy krawędzie
            if(fillHoles){
                cv::findContours(binaryFrame,filledContours,cv::RETR_EXTERNAL
                                 ,cv::CHAIN_APPROX_SIMPLE);
                for(size_t i = 0 ; i < filledContours.size() ; i++){
                    cv::drawContours(binaryFrame,filledContours,int(i)
                                     ,cv::Scalar(255,255,255),cv::FILLED);
                }
            }
            cv::findContours(binaryFrame,finalContours,cv::RETR_EXTERNAL
                             ,cv::CHAIN_APPROX_SIMPLE);

            QImage qimg2(binaryFrame.data, binaryFrame.cols, binaryFrame.rows
                         ,static_cast<int>(binaryFrame.step)
                         ,QImage::Format_Grayscale8);
            rightPixmap.setPixmap(QPixmap::fromImage(qimg2.rgbSwapped()));


            finalFrame = originalFrame.clone();
            cv::drawContours(finalFrame,finalContours,-1,cv::Scalar(0,255,0),1);
            std::vector<std::vector<cv::Point> >hull( finalContours.size() );
            for( size_t i = 0; i < finalContours.size(); i++ )
            {
                cv::convexHull( finalContours[i], hull[i] );
            }

            cv::drawContours(finalFrame,hull,-1,cv::Scalar(134,3,255),1);


            QImage qimg(finalFrame.data, finalFrame.cols, finalFrame.rows
                        , static_cast<int>(finalFrame.step)
                        , QImage::Format_RGB888);
            leftPixmap.setPixmap(QPixmap::fromImage(qimg.rgbSwapped()));

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
    return;
}

void MainWindow::on_upButton_clicked()
{
    yGate -= STEP_GATE_POSITION;
    if(yGate <= 0 ){
        yGate = 0;
    }
}

void MainWindow::on_downButton_clicked()
{
    yGate += STEP_GATE_POSITION;
    if(yGate >= 1){
        yGate = 1;
    }
}

void MainWindow::on_leftButton_clicked()
{
    xGate -= STEP_GATE_POSITION;
    if(xGate < 0){
        xGate = 0;
    }
}

void MainWindow::on_rightButton_clicked()
{
    xGate += STEP_GATE_POSITION;
    if(xGate > 1){
        xGate = 1;
    }
}

void MainWindow::on_thinButton_clicked()
{
    length_gate += STEP_GATE_POSITION;
    if(length_gate > 1){
        length_gate = 0;
    }
}

void MainWindow::on_rotateButton_clicked()
{
    if(rotation){
        rotation = false;
    }
    else{
        rotation = true;
    }
}

void MainWindow::on_clearCounterButton_clicked()
{
    gateCounter = 0;
    ui->lcdCounter->display(0);
}
