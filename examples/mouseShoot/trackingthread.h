#ifndef TRACKINGTHREAD_H
#define TRACKINGTHREAD_H

#include <QThread>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>

using namespace std;
using namespace cv;

class TrackingThread : public QThread
{
    Q_OBJECT

protected:
    void run();

public:
    TrackingThread();

private:
    void searchForMovement(Mat thresholdImage, Mat &cameraFeed);
    string intToString(int number);

signals:
    void sig_changeAngle(int);
    void sig_mousePosition(int,int);
};

#endif // TRACKINGTHREAD_H
