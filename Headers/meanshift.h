#ifndef MEANSHIFT_H
#define MEANSHIFT_H

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <vector>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

class Point5D{
    public:
        float x, y;
        float R, G, B;

        Point5D();
        ~Point5D();

        void accumaltePoint(Point5D);
        void copyPoint(Point5D);
        float colorDistance(Point5D);
        float spatialDistance(Point5D);
        void scalePoint(float);
        void setPoint(float, float, float, float, float);
        };

class MeanShift{
    public:
        vector<Mat> IMGChannels;
        MeanShift();
//        void MeanShift_Filtering(Mat& Img, float spatialBW, float colorBW);
        Mat MeanShift_Segmentation(Mat& Img, float spatialBW, float colorBW);

};

#endif // MEANSHIFT_H
