#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QLabel>
#include <QString>
#include <opencv2/opencv.hpp>
#include "regiongrow.h"
#include "kmeans.h"
#include "tresholding.h"
#include "meanshift.h"
#include "meanshiftdialogue.h"

using namespace cv;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void readImage(QLabel *img_lbl, int x);
    void displayImage(QLabel *img_lbl, Mat img);
    Mat image;
    Mat outputImg;
    Mat imageRGB;
    int w = 500;
    int h = 400;
    QString file_name;
    String PATH = "..\\savedImages\\";

    QPixmap pixmap;
    QPainter *painter;

private slots:
    void on_Browse2_clicked();

    void on_regionGrow_clicked();

    void on_kmeans_clicked();

    void on_Browse_clicked();

    void on_Ostu_clicked();

    void on_spectral_clicked();

    void on_optimal_clicked();

    void on_meanShift_clicked();

    void on_Otsu_local_clicked();

    void on_globalSpect_clicked();

    void on_optimalLocal_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
