#include "tresholding.h"


vector<int> calculateHist(Mat& img){
    int histSize = 256;
    vector<int> hist(histSize, 0);
    for (int x = 0; x < img.rows; x++) {
        for (int y = 0; y < img.cols; y++) {
            int intensity = (int) img.at<uchar>(x, y);
            hist[intensity]++;
        }
    }
    return hist;
}

double optimal_threshold(Mat& img, vector<int> hist){
    double sum, sumB, max_var, threshold, P0, P1 = 0;
    double m0, m1, v;

    int histSize = 256;
    for(int i = 0; i < histSize; i++){
        sum += i * hist[i];
    }
    for(int i = 0; i < histSize; i++){
        P0 += hist[i];
        if(P0 == 0)
            continue;
        P1 = (img.cols * img.rows) - P0;
        if(P1 == 0)
            break;

        sumB += i * hist[i];
        m0 = sumB / P0;
        m1 = (sum - sumB) / P1;
        v = P0 * P1 * (pow((m0 - m1), 2));
        if(v > max_var){
            max_var = v;
            threshold = i;
        }
    }

    return threshold;
}

void thresholding(Mat& inputImg, Mat& outputImg, double thresh){
    for(int i =0; i < inputImg.rows; i++){
        for(int j = 0 ; j<inputImg.cols ; j++){
            double pixel = inputImg.at<uchar>(i,j);
            uchar newPixel = 0;

            if(pixel > thresh){
                newPixel = 255;
            }
            outputImg.at<uchar>(i,j) = newPixel;
        }
    }
}

Mat tresholding::Otsu_Local(Mat& img){
    Mat binary_image = Mat::zeros(img.rows, img.cols, CV_8UC1);

    int block_size = 11; // Size of the local window
    double C = 2.0; // Constant subtracted from the mean

    // Loop over all pixels in the img
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {

            // Extract local window around the current pixel
            Rect window(j - block_size / 2, i - block_size / 2, block_size, block_size);

            // Check that the window is within the bounds of the img
            if (window.x >= 0 && window.y >= 0 && window.x + window.width <= img.cols && window.y + window.height <= img.rows) {
                Mat window_img = img(window);

                // Calculate Otsu threshold for the local window
                vector<int> hist = calculateHist(window_img);
                double local_threshold = optimal_threshold(window_img, hist);

                // Apply threshold to the current pixel
                if (img.at<uchar>(i, j) > local_threshold - C) {
                    binary_image.at<uchar>(i, j) = 255;
                }
            }
        }
    }
    return binary_image;
}


Mat tresholding::Otsu_Global(Mat& img){
    vector<int> hist = calculateHist(img);
    double t = optimal_threshold(img, hist);
    Mat outputImg = img.clone();
    thresholding(img, outputImg, t);

    return outputImg;
}



Mat tresholding::specthresholdImage( Mat& inputImage, int blockSize, double offset, bool localflag)
{
    Mat  thresholdedImage;

    // Determine the three threshold values using spectral thresholding
    int numThresholds = 3;
    vector<int> pixelValues;

    for (int i = 0; i < inputImage.rows; i++)
    {
        for (int j = 0; j < inputImage.cols; j++)
        {
            pixelValues.push_back(inputImage.at<uchar>(i, j));
        }
    }

    sort(pixelValues.begin(), pixelValues.end());

    int numPixelsPerThreshold = pixelValues.size() / numThresholds;

    vector<int> thresholds;
    for (int i = 0; i < numThresholds; i++)
    {
        int thresholdIndex = i * numPixelsPerThreshold + numPixelsPerThreshold / 2;
        thresholds.push_back(pixelValues[thresholdIndex]);
    }

    // Apply local thresholding to the grayscale image
    thresholdedImage = Mat::zeros(inputImage.size(), CV_8UC1);

    for (int i = 0; i < inputImage.rows; i++)
    {
        for (int j = 0; j < inputImage.cols; j++)
        {
            // Compute the local mean intensity

            // Apply the three thresholds to the thresholded pixel value
            uchar pixelValue = inputImage.at<uchar>(i, j);

            if (pixelValue <= thresholds[0])
            {
                thresholdedImage.at<uchar>(i, j) = 0;
            }
            else if (pixelValue > thresholds[0] && pixelValue <= thresholds[1])
            {
                thresholdedImage.at<uchar>(i, j) = 127;
            }
            else if (pixelValue > thresholds[1] && pixelValue <= thresholds[2])
            {
                thresholdedImage.at<uchar>(i, j) = 200;
            }
            else if (pixelValue > thresholds[2])
            {
                thresholdedImage.at<uchar>(i, j) = 255;
            }

            // Apply local thresholding
            if(localflag){
                double sum = 0.0;
                int numPixels = 0;

                for (int k = i - blockSize / 2; k <= i + blockSize / 2; k++)
                {
                    for (int l = j - blockSize / 2; l <= j + blockSize / 2; l++)
                    {
                        if (k >= 0 && k < inputImage.rows && l >= 0 && l < inputImage.cols)
                        {
                            sum += inputImage.at<uchar>(k, l);
                            numPixels++;
                        }
                    }
                }

                double localMean = sum / numPixels;

                // Compute the local threshold
                double localThreshold = localMean + offset;

                if (pixelValue < localThreshold)
                {
                    thresholdedImage.at<uchar>(i, j) = 0;
                }
                else
                {
                    thresholdedImage.at<uchar>(i, j) = 255;
                }
            }

        }
    }

    return thresholdedImage;
}




Mat tresholding::OptimalGlobalThresholding(Mat& inputImg, Mat &outputImg){
    int rows= inputImg.rows;
    int cols = inputImg.cols;
    outputImg = Mat::zeros(inputImg.size(), inputImg.type());

    // Consider the 4 corner pixels as the background
    int pixelTopLeft=inputImg.at<uchar>(0,0);
    int pixelTopRight=inputImg.at<uchar>(0,cols-1);
    int pixelBottomLeft=inputImg.at<uchar>(rows-1,0);
    int pixelBottomRight=inputImg.at<uchar>(rows-1,cols-1);

    // Calculate the Threshold by calculating the mean of the 4 pixels
    double thresholdBackground = (pixelTopLeft + pixelTopRight + pixelBottomLeft + pixelBottomRight)/4;

    // Consider the rest of the image as the foreground or the object
    int sum = 0;
    int numPixels = 0;
        for (int i = 0; i < inputImg.rows; i++) {
            for (int j = 0; j < cols; j++) {
                double pixel = inputImg.at<uchar>(i, j);
                if (pixel != pixelTopLeft && pixel != pixelTopRight && pixel != pixelBottomLeft && pixel != pixelBottomRight) {
                    sum += pixel;
                    numPixels++;
                }
            }
        }
    // Calculate the Threshold by calculating the mean of the of all pixels except the
    //  4 corner pixels
    double thresholdObject = sum / numPixels;

    // let the optimal threshold be the mean of the two thresholds (foreground and background)
    double meanThreshold = (thresholdBackground + thresholdObject) / 2;
    int sumObject = 0;
    int numPixelsObject = 0;
    int sumBackground =0;
    int numPixelsBackGround = 0;
    double newMeanThreshold = 0;
    int exit = 1;

    // loop until the difference between the threshold of the previous iteration and
    // the next iteration are equal or becomes so close
    while(exit == 1){
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            double pixel = inputImg.at<uchar>(i, j);
            if(pixel >= meanThreshold ){
                sumObject += pixel;
                numPixelsObject++;
            }
            else{
                sumBackground +=pixel;
                numPixelsBackGround ++;
            }
        }
    }
    thresholdObject = sumObject / numPixelsObject;
    thresholdBackground = sumBackground / numPixelsBackGround;
    newMeanThreshold = (thresholdObject + thresholdBackground)/2;

    std::cout<<newMeanThreshold<<"   ";
    if((newMeanThreshold == meanThreshold) || abs(newMeanThreshold - meanThreshold) <= 0.01){
        exit = 0;
    }
    else{
        sumObject = 0;
        numPixelsObject = 0;
        sumBackground =0;
        numPixelsBackGround = 0;
        meanThreshold = newMeanThreshold;
        std::cout<<meanThreshold<<" ";
        std::cout<<newMeanThreshold<<"   ";
        }
    }

    // Then put the new pixels value according to the threshold calculated in the output image
    for(int i =0;i < rows; i++){
        for(int j = 0 ; j<cols ; j++){
            uchar pixel = inputImg.at<uchar>(i,j);
            uchar newPixel = 0;

            if(pixel >= newMeanThreshold){
                newPixel = 255;
                outputImg.at<uchar>(i,j) = newPixel;
            }
            else{

                newPixel =0;
                outputImg.at<uchar>(i,j) = newPixel;
            }
        }

    }
    return outputImg;
}


Mat tresholding :: OptimalLocalThresholding(Mat& inputImg, Mat &outputImg){
    int block_size = 11;
    int rows = inputImg.rows;
    int cols = inputImg.cols;
    double sum = 0;
    double thresholdObject =0;
    double sumObject = 0;
    int numPixelsObject = 0;
    double sumBackground =0;
    int numPixelsBackGround = 0;
    double newMeanThreshold = 0;
    double meanThreshold = 0;
    int i,j; // we need these variables for loops to be global so for the loops i can deterimne
            // the starting point of my block
    int pixelTopLeft,pixelTopRight,pixelBottomLeft,pixelBottomRight;
    outputImg = Mat::zeros(inputImg.size(), inputImg.type());
    for (i = 0; i < rows; i += block_size) {
        for (j = 0; j < cols; j += block_size) {
            Mat block = inputImg(Rect(j, i, block_size, block_size));
            // Consider the 4 corner pixels as the background
            pixelTopLeft=block.at<uchar>(0,0);
            pixelTopRight=block.at<uchar>(0,block_size-1);
            pixelBottomLeft=block.at<uchar>(block_size-1,0);
            pixelBottomRight=block.at<uchar>(block_size-1,block_size-1);
            // Calculate the Threshold by calculating the mean of the 4 pixels
            double thresholdBackground = (pixelTopLeft + pixelTopRight + pixelBottomLeft + pixelBottomRight)/4;
            sum = 0;
            // Consider the rest of the block as the foreground or the object
            int numPixels = 0;
                for (int i = 0; i < block_size; i++) {
                    for (int j = 0; j < block_size; j++) {
                        double pixel = inputImg.at<uchar>(i, j);
                        if (pixel != pixelTopLeft && pixel != pixelTopRight && pixel != pixelBottomLeft && pixel != pixelBottomRight) {
                            sum += pixel;
                            numPixels++;
                        }
                    }
                }
            // Calculate the Threshold by calculating the mean of the of all pixels except the
            //  4 corner pixels

            thresholdObject = sum / numPixels;
            // let the optimal threshold be the mean of the two thresholds (foreground and background)
            meanThreshold = (thresholdBackground + thresholdObject) / 2;
            sumObject = 0;
            numPixelsObject = 0;
            sumBackground =0;
            numPixelsBackGround = 0;
            newMeanThreshold = 0;
            int exit = 1;

            // loop until the difference between the threshold of the previous iteration and
            // the next iteration are equal or becomes so close
            while(exit == 1){
                // loop for all blocks and to stop if it exceeds block size or more than rows
                // or cols length
                for (int i1 = i; i1 < i + block_size && i1 < rows; i1++) {
                    for (int j1 = j; j1 < j + block_size && j1 < cols; j1++) {
                    double pixel = inputImg.at<uchar>(i1, j1);
                    if(pixel >= meanThreshold ){
                        sumObject += pixel;
                        numPixelsObject++;
                    }
                    else{
                        sumBackground +=pixel;
                        numPixelsBackGround ++;
                    }
                }
            }
            std::cout<<meanThreshold<<"  ";
            // condition because i may encounter no pixels to consider it an object

            if(numPixelsObject !=0 ){
                thresholdObject = sumObject / numPixelsObject;
            }
            else{
                thresholdObject = 0;
            }
            // condition because i may encounter no pixels to consider it a background
            if(numPixelsBackGround !=0){
                thresholdBackground = sumBackground / numPixelsBackGround;

            }
            else{
                thresholdBackground = 0;
            }
               newMeanThreshold = (thresholdBackground + thresholdObject) /2;

            if((newMeanThreshold == meanThreshold) || abs(newMeanThreshold - meanThreshold) <= 0.1){
                exit = 0;
            }
            else{
                sumObject = 0;
                numPixelsObject = 0;
                sumBackground =0;
                numPixelsBackGround = 0;
                meanThreshold = newMeanThreshold;

                }
            }
            // fill the output image with the block
            for (int i1 = i; i1 < i + block_size && i1 < rows; i1++) {
                for (int j1 = j; j1 < j + block_size && j1 < cols; j1++) {

                    uchar pixel = inputImg.at<uchar>(i1,j1);
                    uchar newPixel = 0;

                    if(pixel >= newMeanThreshold){
                        newPixel = 255;
                        outputImg.at<uchar>(i1,j1) = newPixel;
                    }
                    else{

                        newPixel =0;
                        outputImg.at<uchar>(i1,j1) = newPixel;
                    }
                }

            }
            std::cout<<"processed block at ("<<i<<","<<j<<")\n";



        }
        // repeat the process for each block
    }
    return outputImg;
}

