#include <opencv2/dpm.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/videoio/videoio_c.h>
#include <opencv2/shape/shape_transformer.hpp>
#include <stdio.h>
#include <iostream>

using namespace cv;
using namespace cv::dpm;
using namespace std;

const char* winName = "lustro";

int main() {

    VideoCapture capture(0);
    capture.set(CV_CAP_PROP_FRAME_WIDTH, 600);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, 400);

    Mat frame;
    namedWindow(winName, WINDOW_NORMAL);

    std::vector< std::pair< Point > > points;


    std::vector<cv::Point> iP, iiP;

    iP.push_back(cv::Point(0,0));
    iP.push_back(cv::Point(100,100));
    iP.push_back(cv::Point(0,100));
    iP.push_back(cv::Point(100,0));
    iP.push_back(cv::Point(256,256));
    iP.push_back(cv::Point(150,256));

    iiP.push_back(cv::Point(70,70));
    iiP.push_back(cv::Point(430,60));
    iiP.push_back(cv::Point(60,410));
    iiP.push_back(cv::Point(430,420));
    iiP.push_back(cv::Point(220,280));
    iiP.push_back(cv::Point(180,240));

    Mat transformingShape(iP);
    Mat targetShape(iiP);
    transpose(transformingShape, transformingShape);
    transpose(targetShape, targetShape);


    std::vector<DMatch> matches;
    for (int i = 0; i < iP.size(); ++i) {
        matches.push_back(DMatch(i, i, 0));
    }

     // create thin plate spline object and put the vectors into the constructor
    Ptr<ThinPlateSplineShapeTransformer> tps = createThinPlateSplineShapeTransformer(0);
    tps->estimateTransformation(transformingShape, targetShape, matches);


    while(capture.read(frame)) {

        Mat dstImage;
        tps->warpImage(frame, dstImage);
        // dstImage = frame;

        flip(dstImage, dstImage, 1);
        imshow(winName, dstImage);
        waitKey(1);
    }

}