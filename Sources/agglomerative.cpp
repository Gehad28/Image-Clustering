#include "agglomerative.h"

double euclideanDist(const std::vector<int> p1, const std::vector<int> p2) {
    double sum = 0.0;
    for (int i = 0; i < p1.size(); ++i) {
        sum += pow(p1[i] - p2[i], 2.0);
    }
    return sqrt(sum);
}

// find mean distance betwen two clusters
double distance(vector<vector<vector<int>>> Ci, vector<vector<vector<int>>> Cj) {

    vector<double> dis;
    for (auto i : Ci) {
        for (auto j : Cj) {
            dis.push_back(euclideanDist(i[0], j[0]));
        }
    }
    std::sort(dis.begin(), dis.end());
    auto last = unique(dis.begin(), dis.end());
    dis.erase(last, dis.end());
    double sum = 0;
    for (double d : dis) {
        sum += d;
    }
    return sum / dis.size();
}

// find two clusters having minimum distance
double find_Min(vector<vector<double>> M, int& x, int& y) {
    double min;
    min = INFINITY;
    for (int i = 0; i < M.size(); i++) {
        for (int j = 0; j < M[i].size(); j++) {
            if (M[i][j] < min) {
                min = M[i][j];
                x = i;
                y = j+i;
            }
        }
    }
    return min;
}

// Define the Agglomerative function
int  agglomerative_clustering(Mat& img, vector<vector<vector<vector<int>>>>& clusters, int distance_threshold) {

    vector<vector<vector<int>>> dataset((img.rows*img.cols));
    int k = 0;
    for (int i = 0; i < img.rows; ++i) {
        for (int j = 0; j < img.cols; ++j){

            if (img.channels() == 3) {
                dataset[k] = { { (int)(img.at<Vec3b>(i, j)[0]),(int)(img.at<Vec3b>(i, j)[1]),(int)(img.at<Vec3b>(i, j)[2]) },{i,j} };
            }
            else {
                dataset[k] = { {(int)(img.at<uchar>(i, j)) } ,{i,j} };
            }
            k += 1;
        }
    }
    vector<vector<double>> M;

    // initializing clusters
    for (auto i : dataset) {
        vector<vector<vector<int>>>  Ci;
        Ci.push_back(i);
        clusters.push_back(Ci);
    }

    for (int i = 0; i < clusters.size(); i++) {
        vector<double> Mi;
        for (int j = i + 1; j < clusters.size(); j++) {

            Mi.push_back(distance(clusters[i], clusters[j]));
        }
        M.push_back(Mi);
    }

    int q = dataset.size();
    // decreasing number of clusters
    while (q > 4) {
        int x, y;
        double min;
        min= find_Min(M, x, y);

        if (min > distance_threshold) {
            break;
        }
        clusters[x].insert(clusters[x].end(), clusters[y].begin(), clusters[y].end());
        clusters.erase(clusters.begin() + y);
        M.clear();
        for (int i = 0; i < clusters.size(); i++) {

            vector<double> Mi;

            for (int j = i + 1; j < clusters.size(); j++) {

                Mi.push_back(distance(clusters[i], clusters[j]));
            }
            M.push_back(Mi);
        }
        q--;
    }
    return q;
}



Mat agglomerative_seg(Mat& img, vector<vector<vector<vector<int>>>>& clusters) {


    Mat segmented_img = img;

    for (int i = 0; i < clusters.size(); ++i) {
        for (int j = 0; j < clusters[i].size(); ++j) {
            int x = clusters[i][j][1][0];
            int y = clusters[i][j][1][1];
            if (img.channels() == 3)  {
                (segmented_img.at<Vec3b>(x, y)[0]) = (unsigned char)(i % clusters.size());
                (segmented_img.at<Vec3b>(x, y)[1]) = (unsigned char)((i % clusters.size() )*2);
                (segmented_img.at<Vec3b>(x, y)[2]) = (unsigned char)((i % clusters.size()) * 3);
            }
            else {
                (segmented_img.at<uchar>(x, y)) = (unsigned char)(i);
            }
        }
    }
    return segmented_img = img;

}


