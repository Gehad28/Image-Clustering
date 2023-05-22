#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readImage(QLabel *img_lbl, int x){
    if(x == 0){
        image = imread(file_name.toStdString(), IMREAD_GRAYSCALE);
    }
    else if(x == 1){
        image = imread(file_name.toStdString(), IMREAD_COLOR);
    }
    displayImage(img_lbl, image);
}

void MainWindow::displayImage(QLabel *img_lbl, Mat img){
    cvtColor(img, imageRGB, COLOR_BGR2RGB);
    img_lbl->setPixmap(QPixmap::fromImage(QImage(imageRGB.data, imageRGB.cols, imageRGB.rows, imageRGB.step,
                                                 QImage::Format_RGB888)).scaled(w,h,Qt::KeepAspectRatio));
    img_lbl->repaint();
}

//void MainWindow::showWindow(int x){
//    if(x == 1){
//        threshold.setModal(true);
//        threshold.exec();
//    }

//    if(x == 2){
//        circle.setModal(true);
//        circle.exec();
//    }
//}

void MainWindow::on_Browse_clicked()
{
    file_name = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(), tr("Images (*.png *.xpm *.jpg)"));
    if (!file_name.isEmpty()){
        readImage(ui->inputImage, 0);
    }
}


void MainWindow::on_Browse2_clicked()
{
    file_name = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(), tr("Images (*.png *.xpm *.jpg)"));
    if (!file_name.isEmpty()){
        readImage(ui->inputImage2, 1);
    }
}


void MainWindow::on_regionGrow_clicked()
{
    RegionGrow RG;
    Mat result = RG.applyGrow(image);
    displayImage(ui->outputImage2, result);
    ui->regions->setText(QString::number(RG.regions, 10));
}


void MainWindow::on_kmeans_clicked()
{
    vector<int> labels(image.rows * image.cols);
    int n_clusters=4;
    n_clusters = ui->nClusters->value();
    vector<vector<int>> centroids (n_clusters);

    Kmeans k;

    k.KMeans(image, centroids, labels, n_clusters = n_clusters);
    Mat segmented_image = k.segment(image, centroids, labels);
    displayImage(ui->outputImage2, segmented_image);
}


void MainWindow::on_meanShift_clicked()
{
    MeanShift MS;

    MeanShiftDialogue MSDialogue;
    MSDialogue.setModal(true);
    MSDialogue.exec();

    Mat MSresult = MS.MeanShift_Segmentation(image, MSDialogue.spatialBW, MSDialogue.colorBW);
    displayImage(ui->outputImage2, MSresult);
}

void MainWindow::on_Ostu_clicked()
{
    tresholding t;
    Mat ostuImage = t.Otsu_Global(image);
    displayImage(ui->outputImage, ostuImage);
}

void MainWindow::on_Otsu_local_clicked()
{
    tresholding t;
    Mat output = t.Otsu_Local(image);
    displayImage(ui->outputImage, output);
}



void MainWindow::on_spectral_clicked()
{
    tresholding t;
    Mat img = t.specthresholdImage(image, 11, 12,1);
    displayImage(ui->outputImage, img);
}

void MainWindow::on_globalSpect_clicked()
{
    tresholding t;
    Mat img = t.specthresholdImage(image, 11, 12,0);
    displayImage(ui->outputImage, img);
}


void MainWindow::on_optimal_clicked()
{
    tresholding t;
    Mat outputImage = t.OptimalGlobalThresholding(image,outputImg);
    displayImage(ui->outputImage, outputImage);

}



void MainWindow::on_optimalLocal_clicked()
{
    tresholding t;
    Mat outputImage = t.OptimalLocalThresholding(image,outputImg);
    displayImage(ui->outputImage, outputImage);
}

