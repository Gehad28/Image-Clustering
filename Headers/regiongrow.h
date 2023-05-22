#ifndef REGIONGROW_H
#define REGIONGROW_H

#include <iostream>
#include <stack>
#include <cassert>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class RegionGrow
{
public:
    Mat applyGrow(Mat &src);

    int regions;
};

#endif // REGIONGROW_H
