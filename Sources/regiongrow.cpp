#include "regiongrow.h"

void Grow(Mat& src, Mat& dest, Mat& mask, Point seed, int threshold);
int SeedGrow(Mat& src, Mat& dest, int threshold);

// parameters
const uchar THRESHOLD = 50;
const uchar MAX_REGION_NUM = 100;
const double MIN_REGION_AREA_FACTOR = 0.01;
const Point PointShift2D[8] =
    {
        Point(1, 0),
        Point(1, -1),
        Point(0, -1),
        Point(-1, -1),
        Point(-1, 0),
        Point(-1, 1),
        Point(0, 1),
        Point(1, 1)
};

int SeedGrow(Mat& src, Mat& dest, int threshold) {
    int minRegionarea = int(MIN_REGION_AREA_FACTOR * src.cols * src.rows);
    uchar padding = 1;

    Mat mask = Mat::zeros(src.rows, src.cols, CV_8UC1);

    int regionCount = 0;

    for (int x = 0; x < src.cols; ++x) {
        for (int y = 0; y < src.rows; ++y) {
            if (dest.at<uchar>(Point(x, y)) == 0) {
                Grow(src, dest, mask, Point(x, y), threshold);

                int mask_area = (int)sum(mask).val[0];
                if (mask_area > minRegionarea) {
                    ++regionCount;

                    dest = dest + mask * padding;
                    if(++padding > MAX_REGION_NUM) { return -1; }
                } else {
                    dest = dest + mask * 255;
                }
                mask = mask - mask;
            }
        }
    }
    return regionCount;
}

void Grow(Mat& src, Mat& dest, Mat& mask, Point seed, int threshold) {
    stack<Point> point_stack;
    point_stack.push(seed);

    while(!point_stack.empty()) {
        Point center = point_stack.top();
        mask.at<uchar>(center) = 1;
        point_stack.pop();

        for (int i = 0; i < 8; ++i) {
            Point estimating_point = center + PointShift2D[i];
            if (estimating_point.x < 0
                || estimating_point.x > src.cols-1
                || estimating_point.y < 0
                || estimating_point.y > src.rows-1) {
                continue;
            } else {
                int delta = int(pow(src.at<Vec3b>(center)[0] - src.at<Vec3b>(estimating_point)[0], 2)
                                + pow(src.at<Vec3b>(center)[1] - src.at<Vec3b>(estimating_point)[1], 2)
                                + pow(src.at<Vec3b>(center)[2] - src.at<Vec3b>(estimating_point)[2], 2));
                if (dest.at<uchar>(estimating_point) == 0
                    && mask.at<uchar>(estimating_point) == 0
                    && delta < threshold) {
                    mask.at<uchar>(estimating_point) = 1;
                    point_stack.push(estimating_point);
                }
            }
        }
    }
}


Mat RegionGrow::applyGrow(Mat& src){
    assert(!src.empty());
    if (src.cols > 500 || src.rows > 500) {
        cv::resize(src, src, cv::Size(0, 0), 0.5, 0.5);
    }

    Mat temp = Mat::zeros(src.rows, src.cols, CV_8UC1);
    Mat dest = Mat::zeros(src.rows, src.cols, CV_8UC1);

    regions = SeedGrow(src, dest, THRESHOLD);

    Mat result_image = Mat::zeros(src.rows, src.cols, CV_8UC3);
    for (int x=0; x<dest.cols; ++x) {
        for (int y = 0; y < dest.rows; ++y) {
            if (dest.at<uchar>(Point(x, y)) == 1) {
                result_image.at<Vec3b>(Point(x, y))[0] = 255;
                result_image.at<Vec3b>(Point(x, y))[1] = 0;
                result_image.at<Vec3b>(Point(x, y))[2] = 0;
            } else if (dest.at<uchar>(Point(x, y)) == 2) {
                result_image.at<Vec3b>(Point(x, y))[0] = 0;
                result_image.at<Vec3b>(Point(x, y))[1] = 255;
                result_image.at<Vec3b>(Point(x, y))[2] = 0;
            } else if (dest.at<uchar>(Point(x, y)) == 3) {
                result_image.at<Vec3b>(Point(x, y))[0] = 0;
                result_image.at<Vec3b>(Point(x, y))[1] = 0;
                result_image.at<Vec3b>(Point(x, y))[2] = 255;
            } else if (dest.at<uchar>(Point(x, y)) == 4) {
                result_image.at<Vec3b>(Point(x, y))[0] = 255;
                result_image.at<Vec3b>(Point(x, y))[1] = 0;
                result_image.at<Vec3b>(Point(x, y))[2] = 255;
            } else if (dest.at<uchar>(Point(x, y)) == 5) {
                result_image.at<Vec3b>(Point(x, y))[0] = 255;
                result_image.at<Vec3b>(Point(x, y))[1] = 230;
                result_image.at<Vec3b>(Point(x, y))[2] = 200;
            } else if (dest.at<uchar>(Point(x, y)) == 6) {
                result_image.at<Vec3b>(Point(x, y))[0] = 255;
                result_image.at<Vec3b>(Point(x, y))[1] = 255;
                result_image.at<Vec3b>(Point(x, y))[2] = 0;
            } else if (dest.at<uchar>(Point(x, y)) == 7) {
                result_image.at<Vec3b>(Point(x, y))[0] = 120;
                result_image.at<Vec3b>(Point(x, y))[1] = 120;
                result_image.at<Vec3b>(Point(x, y))[2] = 0;
            } else if (dest.at<uchar>(Point(x, y)) == 8) {
                result_image.at<Vec3b>(Point(x, y))[0] = 0;
                result_image.at<Vec3b>(Point(x, y))[1] = 120;
                result_image.at<Vec3b>(Point(x, y))[2] = 120;
            } else if (dest.at<uchar>(Point(x, y)) == 9) {
                result_image.at<Vec3b>(Point(x, y))[0] = 120;
                result_image.at<Vec3b>(Point(x, y))[1] = 0;
                result_image.at<Vec3b>(Point(x, y))[2] = 120;
            } else if (dest.at<uchar>(Point(x, y)) == 10) {
                result_image.at<Vec3b>(Point(x, y))[0] = 120;
                result_image.at<Vec3b>(Point(x, y))[1] = 120;
                result_image.at<Vec3b>(Point(x, y))[2] = 120;
            } else if (dest.at<uchar>(Point(x, y)) == 11) {
                result_image.at<Vec3b>(Point(x, y))[0] = 120;
                result_image.at<Vec3b>(Point(x, y))[1] = 0;
                result_image.at<Vec3b>(Point(x, y))[2] = 0;
            } else if (dest.at<uchar>(Point(x, y)) == 12) {
                result_image.at<Vec3b>(Point(x, y))[0] = 0;
                result_image.at<Vec3b>(Point(x, y))[1] = 120;
                result_image.at<Vec3b>(Point(x, y))[2] = 0;
            } else if (dest.at<uchar>(Point(x, y)) == 13) {
                result_image.at<Vec3b>(Point(x, y))[0] = 0;
                result_image.at<Vec3b>(Point(x, y))[1] = 0;
                result_image.at<Vec3b>(Point(x, y))[2] = 120;
            } else if (dest.at<uchar>(Point(x, y)) == 255) {
                result_image.at<Vec3b>(Point(x, y))[0] = 20;
                result_image.at<Vec3b>(Point(x, y))[1] = 20;
                result_image.at<Vec3b>(Point(x, y))[2] = 20;
            }
        }
    }

    return result_image;
}














