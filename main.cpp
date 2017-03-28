#include <opencv2/dpm.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/videoio/videoio_c.h>
#include <opencv2/shape/shape_transformer.hpp>
#include <stdio.h>
#include <iostream>

#include "CThinPlateSpline.h"

using namespace cv;
using namespace cv::dpm;
using namespace std;

const char* winName = "lustro";
static int _r = 49;   // Radial transform. 50 in range 0..100
static int _tX = 51;  // Tangetial coef in X directio
static int _tY = 49;  // Tangetial coef in Y directio
static int allRange = 50;

int main() {

    VideoCapture capture(0);
    capture.set(CV_CAP_PROP_FRAME_WIDTH, 400);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, 400);

    Mat frame;
    namedWindow(winName, 1);

    // generate some generic points
    // usually you would use a interest point detector such as SURF or SIFT
    std::vector<cv::Point> iP, iiP;

    // push some points into the vector for the source image
    iP.push_back(cv::Point(50,50));
    iP.push_back(cv::Point(400,50));
    iP.push_back(cv::Point(50,400));
    iP.push_back(cv::Point(400,400));
    iP.push_back(cv::Point(256,256));
    iP.push_back(cv::Point(150,256));

    // push some point into the vector for the dst image
    iiP.push_back(cv::Point(70,70));
    iiP.push_back(cv::Point(430,60));
    iiP.push_back(cv::Point(60,410));
    iiP.push_back(cv::Point(430,420));
    iiP.push_back(cv::Point(220,280));
    iiP.push_back(cv::Point(180,240));


    // Mat transformingShape = (Mat_<int>(2, 6) << 70, 70,
    //                                             430, 60,)

     // create thin plate spline object and put the vectors into the constructor
    // Ptr<ThinPlateSplineShapeTransformer> tps = createThinPlateSplineShapeTransformer();
    CThinPlateSpline tps(iP, iiP);


    while(capture.read(frame)) {

        Mat intrCamParamsMat = (Mat_<double>(3, 3, CV_64F) <<
                587.1769751432448200/2.0, 0., 319.5000000000000000/2.0+0,
                0., 591.3189722549362800/2.0, 239.5000000000000000/2.0+0,
                0., 0., 1.);

        Mat dist4Coeff = (Mat_<double>(1, 4, CV_64F) <<
            (_r-allRange*1.0)/allRange*1.0,
            (_r-allRange*1.0)/allRange*1.0,
            (_tY-allRange*1.0)/allRange*1.0,
            (_tX-allRange*1.0)/allRange*1.0);

        Mat dstImage;
        // undistort(frame, dstImage, intrCamParamsMat, dist4Coeff);
        // tps->estimateTransformation()
        // tps->warpImage(frame,dstImage,0.01,cv::INTER_NEAREST,FORWARD_WARP);
        tps.warpImage(frame, dstImage, 0.01, cv::INTER_NEAREST, BACK_WARP);



        imshow(winName, dstImage);
        waitKey(30);
    }

}