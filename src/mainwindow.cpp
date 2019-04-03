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
 */
void MainWindow::on_OpenFile_clicked()
{

    file_name = QFileDialog::getOpenFileName(this,"Otwórz plik video","/home/damian/Wideo");
}

/**
 * @brief
 *
 */
void MainWindow::on_PlayButton_clicked()
{

}

/**
 * @brief
 *
 */
void MainWindow::on_StopButton_clicked()
{

}
