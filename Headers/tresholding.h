
#ifndef TRESHOLDING_H
#define TRESHOLDING_H

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;



class tresholding
{
public:
    Mat Otsu_Local(Mat& img);
    Mat Otsu_Global(Mat& img);
    Mat specthresholdImage(Mat& inputImage, int blockSize, double offset, bool localflag);
    Mat localOptimal(Mat img);
    Mat OptimalGlobalThresholding(cv::Mat& inputImg, cv::Mat &outputImg);
    Mat OptimalLocalThresholding(Mat& inputImg, Mat &outputImg);
};

#endif // TRESHOLDING_H
