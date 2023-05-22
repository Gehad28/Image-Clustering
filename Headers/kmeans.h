#ifndef KMEANS_H
#define KMEANS_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iterator>
#include <numeric>
using namespace std;
using namespace cv;


class Kmeans
{
public:

    void KMeans(Mat& img, vector<vector<int>>& centroids, vector<int>& labels, int n_clusters = 2, int max_iter = 8);
    Mat segment(Mat& img, vector<vector<int>>& centroids, vector<int>& labels);
};

#endif // KMEANS_H
