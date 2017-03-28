#include <opencv2/dpm.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/videoio/videoio_c.h>
#include <opencv2/shape/shape_transformer.hpp>
#include <stdio.h>
#include <iostream>
#include <map>
#include <boost/assign/list_of.hpp>

using namespace cv;
using namespace cv::dpm;
using namespace std;
using boost::assign::list_of;

const char* winName = "lustro";

Point adjustPoint(const Point& point, size_t width, size_t height) {
    return Point(static_cast<float>(point.x) / 100 * width,
                 static_cast<float>(point.y) / 100 * height);
}

typedef vector< pair< Point, Point > > Points;

class ITransformation {
public:
    virtual void Transform(const Mat& input, Mat& output) = 0;
};


class MirrorTransformation : public ITransformation {
public:
    MirrorTransformation(size_t width, size_t height) : width(width), height(height) {
        points = {
            make_pair(Point(0, 0), Point(0, 0)),
            make_pair(Point(100, 100), Point(100, 100)),
            make_pair(Point(0, 100), Point(0, 100)),
            make_pair(Point(100, 0), Point(100, 0)),

            make_pair(Point(50, 0), Point(50, 0)),
            make_pair(Point(0, 50), Point(0, 50)),
            make_pair(Point(100, 50), Point(100, 50)),
            make_pair(Point(50, 100), Point(50, 100)),
        };
    }

    void Transform(const Mat& input, Mat& output) override {
        tps->warpImage(input, output);
    }

protected:
    void Init() {
        vector<Point> transformingShapeVector;
        vector<Point> targetShapeVector;
        for (int i = 0; i < points.size(); ++i) {
            transformingShapeVector.push_back(adjustPoint(points[i].first, width, height));
            targetShapeVector.push_back(adjustPoint(points[i].second, width, height));
        }

        Mat transformingShape(transformingShapeVector);
        Mat targetShape(targetShapeVector);
        transpose(transformingShape, transformingShape);
        transpose(targetShape, targetShape);

        cout << transformingShape << endl;
        cout << targetShape << endl;

        std::vector<DMatch> matches;
        for (int i = 0; i < points.size(); ++i) {
            matches.push_back(DMatch(i, i, 0));
        }

        tps = createThinPlateSplineShapeTransformer(0);
        tps->estimateTransformation(transformingShape, targetShape, matches);
    }

    Ptr<ThinPlateSplineShapeTransformer> tps;
    Points points;
    const size_t width;
    const size_t height;
};

class FunMirror1Transform : public MirrorTransformation {
public:
    FunMirror1Transform(size_t width, size_t height) : MirrorTransformation(width, height) {
        points.insert(points.end(), {
            (make_pair(Point(48, 70), Point(40, 70))),
            (make_pair(Point(52, 70), Point(60, 70))),
            (make_pair(Point(40, 10), Point(48, 10))),
            (make_pair(Point(60, 90), Point(52, 90)))
        });
        Init();
    }
};

class FunMirror2Transform : public MirrorTransformation {
public:
    FunMirror2Transform(size_t width, size_t height) : MirrorTransformation(width, height) {
        points.insert(points.end(), {
            (make_pair(Point(40, 20), Point(60, 20))),
            (make_pair(Point(60, 40), Point(40, 40))),
            (make_pair(Point(40, 60), Point(60, 60))),
            (make_pair(Point(60, 80), Point(40, 80)))
        });
        Init();
    }
};

class FunMirror3Transform : public MirrorTransformation {
public:
    FunMirror3Transform(size_t width, size_t height) : MirrorTransformation(width, height) {
        points.insert(points.end(), {
            (make_pair(Point(51, 20), Point(55, 20))),
            (make_pair(Point(49, 40), Point(45, 40))),
            (make_pair(Point(51, 60), Point(55, 60))),
            (make_pair(Point(49, 80), Point(45, 80)))
        });
        Init();
    }
};



int main() {

    VideoCapture capture(0);
    capture.set(CV_CAP_PROP_FRAME_WIDTH, 600);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, 400);

    Mat frame;
    capture.read(frame);
    namedWindow(winName, WINDOW_NORMAL);

    const Size videoSize = frame.size();
    const size_t width = videoSize.width;
    const size_t height = videoSize.height;
    cout << width << ", " << height << endl;

    vector<ITransformation*> transformations = {
        new FunMirror1Transform(width, height),
        new FunMirror2Transform(width, height),
        new FunMirror3Transform(width, height),
    };
    vector<ITransformation*>::iterator transformIt = transformations.begin();

    FunMirror3Transform tr(width, height);

    while(capture.read(frame)) {

        Mat dstImage;
        (*transformIt)->Transform(frame, dstImage);
        flip(dstImage, dstImage, 1);

        imshow(winName, dstImage);
        if (waitKey(1) == ' ') {
            if (++transformIt == transformations.end()) {
                transformIt = transformations.begin();
            }
        }
    }

}