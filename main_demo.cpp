#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "colotracker.h"
#include "region.h"
#include <string>

using namespace cv;

cv::Point g_topLeft(0,0);
cv::Point g_botRight(0,0);
cv::Point g_botRight_tmp(0,0);
bool plot = false;
bool g_trackerInitialized = false;
ColorTracker * g_tracker = NULL;

static void onMouse(int event, int x, int y, int, void* param) {
    cv::Mat img = ((cv::Mat *)param)->clone();
    if (event == EVENT_LBUTTONDOWN && !g_trackerInitialized) {
        std::cout << "DOWN " << std::endl;
        g_topLeft = Point(x, y);
        plot = true;
    } else if (event == EVENT_LBUTTONUP && !g_trackerInitialized) {
        std::cout << "UP " << std::endl;
        g_botRight = Point(x, y);
        plot = false;
        if (g_tracker != NULL)
            delete g_tracker;
        g_tracker = new ColorTracker();
        g_tracker->init(*(cv::Mat *)param, g_topLeft.x, g_topLeft.y, g_botRight.x, g_botRight.y);
        g_trackerInitialized = true;
    } else if (event == EVENT_MOUSEMOVE && !g_trackerInitialized) {
        g_botRight_tmp = Point(x, y);
    }
}

int main(int argc, char **argv) {
    BBox * bb = NULL;
    cv::Mat img;
    int captureDevice = 0;
    if (argc > 1)
        captureDevice = atoi(argv[1]);

    cv::VideoCapture webcam = cv::VideoCapture(captureDevice);
    webcam.set(CAP_PROP_FRAME_WIDTH, 640);  // Modern OpenCV constant
    webcam.set(CAP_PROP_FRAME_HEIGHT, 480); // Modern OpenCV constant
    if (!webcam.isOpened()) {
        webcam.release();
        std::cerr << "Error during opening capture device!" << std::endl;
        return 1;
    }

    cv::namedWindow("output", 0);
    cv::setMouseCallback("output", onMouse, &img);

    for (;;) {
        webcam >> img;

        int c = waitKey(10);
        if ((c & 255) == 27) { // Press 'Esc' to exit
            std::cout << "Exiting ..." << std::endl;
            break;
        }

        switch ((char)c) {
        case 'i':  // Reset tracker if 'i' is pressed
            g_trackerInitialized = false;
            g_topLeft = cv::Point(0, 0);
            g_botRight_tmp = cv::Point(0, 0);
            break;
        default:
            break;
        }

        if (g_trackerInitialized && g_tracker != NULL) {
            bb = g_tracker->track(img);
        }

        if (!g_trackerInitialized && plot && g_botRight_tmp.x > 0) {
            cv::rectangle(img, g_topLeft, g_botRight_tmp, cv::Scalar(0, 255, 0), 2);
        }

        if (bb != NULL) {
            cv::rectangle(img, Point2i(bb->x, bb->y), Point2i(bb->x + bb->width, bb->y + bb->height), Scalar(255, 0, 0), 3);
            delete bb;
            bb = NULL;
        }

        cv::imshow("output", img);
    }

    if (g_tracker != NULL)
        delete g_tracker;
    return 0;
}