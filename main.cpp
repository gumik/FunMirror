#include <opencv2/dpm.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/videoio/videoio_c.h>

#include <stdio.h>
#include <iostream>


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
    capture.set(CV_CAP_PROP_FRAME_WIDTH, 1600);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, 1000);

    Mat frame;
    namedWindow(winName, 1);
    // the color of the rectangle
    Scalar color(0, 255, 255); // yellow

    while( capture.read(frame) ) {

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
    	undistort(frame, dstImage, intrCamParamsMat, dist4Coeff);

		imshow(winName, dstImage);
		waitKey(30);
	}

}