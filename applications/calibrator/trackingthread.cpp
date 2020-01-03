#include "trackingthread.h"
#include <QDebug>

#include "Blob.h"

#define SHOW_STEPS            // un-comment or comment this line to show steps or not

// global variables ///////////////////////////////////////////////////////////////////////////////
const cv::Scalar SCALAR_BLACK = cv::Scalar(0.0, 0.0, 0.0);
const cv::Scalar SCALAR_WHITE = cv::Scalar(255.0, 255.0, 255.0);
const cv::Scalar SCALAR_YELLOW = cv::Scalar(0.0, 255.0, 255.0);
const cv::Scalar SCALAR_GREEN = cv::Scalar(0.0, 200.0, 0.0);
const cv::Scalar SCALAR_RED = cv::Scalar(0.0, 0.0, 255.0);

//default capture width and height
const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;

// function prototypes ////////////////////////////////////////////////////////////////////////////
void matchCurrentFrameBlobsToExistingBlobs(std::vector<Blob> &existingBlobs, std::vector<Blob> &currentFrameBlobs);
void addBlobToExistingBlobs(Blob &currentFrameBlob, std::vector<Blob> &existingBlobs, int &intIndex);
void addNewBlob(Blob &currentFrameBlob, std::vector<Blob> &existingBlobs);
double distanceBetweenPoints(cv::Point point1, cv::Point point2);
void drawAndShowContours(cv::Size imageSize, std::vector<std::vector<cv::Point> > contours, std::string strImageName);
void drawAndShowContours(cv::Size imageSize, std::vector<Blob> blobs, std::string strImageName);
void drawBlobInfoOnImage(std::vector<Blob> &blobs, cv::Mat &imgFrame2Copy);
void findTargetBlob(std::vector<Blob> &blobs);

static bool lockedOnABlob = false;
static cv::Point crossHair;
static unsigned int currentLockedBlob;


#if 0
void createTrackbars()
{
    //create window for trackbars

    namedWindow(trackbarWindowName,0);
    //create memory to store trackbar name on window
    char TrackbarName[50];
    sprintf( TrackbarName, "H_MIN", H_MIN);
    sprintf( TrackbarName, "H_MAX", H_MAX);
    sprintf( TrackbarName, "S_MIN", S_MIN);
    sprintf( TrackbarName, "S_MAX", S_MAX);
    sprintf( TrackbarName, "V_MIN", V_MIN);
    sprintf( TrackbarName, "V_MAX", V_MAX);
    //create trackbars and insert them into window
    //3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
    //the max value the trackbar can move (eg. H_HIGH),
    //and the function that is called whenever the trackbar is moved(eg. on_trackbar)
    //                                  ---->    ---->     ---->
    createTrackbar( "H_MIN", trackbarWindowName, &H_MIN, H_MAX, on_trackbar );
    createTrackbar( "H_MAX", trackbarWindowName, &H_MAX, H_MAX, on_trackbar );
    createTrackbar( "S_MIN", trackbarWindowName, &S_MIN, S_MAX, on_trackbar );
    createTrackbar( "S_MAX", trackbarWindowName, &S_MAX, S_MAX, on_trackbar );
    createTrackbar( "V_MIN", trackbarWindowName, &V_MIN, V_MAX, on_trackbar );
    createTrackbar( "V_MAX", trackbarWindowName, &V_MAX, V_MAX, on_trackbar );
}

void drawObject(int x, int y,Mat &frame){

    //use some of the openCV drawing functions to draw crosshairs
    //on your tracked image!

    //UPDATE:JUNE 18TH, 2013
    //added 'if' and 'else' statements to prevent
    //memory errors from writing off the screen (ie. (-25,-25) is not within the window!)

    circle(frame,Point(x,y),20,Scalar(0,255,0),2);
    if(y-25>0)
    line(frame,Point(x,y),Point(x,y-25),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(x,0),Scalar(0,255,0),2);
    if(y+25<FRAME_HEIGHT)
    line(frame,Point(x,y),Point(x,y+25),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(x,FRAME_HEIGHT),Scalar(0,255,0),2);
    if(x-25>0)
    line(frame,Point(x,y),Point(x-25,y),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(0,y),Scalar(0,255,0),2);
    if(x+25<FRAME_WIDTH)
    line(frame,Point(x,y),Point(x+25,y),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(FRAME_WIDTH,y),Scalar(0,255,0),2);

    putText(frame,intToString(x)+","+intToString(y),Point(x,y+30),1,1,Scalar(0,255,0),2);

}
void morphOps(Mat &thresh){

    //create structuring element that will be used to "dilate" and "erode" image.
    //the element chosen here is a 3px by 3px rectangle

    Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
    //dilate with larger element so make sure object is nicely visible
    Mat dilateElement = getStructuringElement( MORPH_RECT,Size(8,8));

    erode(thresh,thresh,erodeElement);
    erode(thresh,thresh,erodeElement);


    dilate(thresh,thresh,dilateElement);
    dilate(thresh,thresh,dilateElement);



}

void trackFilteredObject(int &x, int &y, Mat threshold, Mat &cameraFeed)
{
    Mat temp;
    threshold.copyTo(temp);
    //these two vectors needed for output of findContours
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;
    //find contours of filtered image using openCV findContours function
    findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
    //use moments method to find our filtered object
    double refArea = 0;
    bool objectFound = false;
    if (hierarchy.size() > 0) {
        int numObjects = hierarchy.size();
        //if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
        if(numObjects<MAX_NUM_OBJECTS){
            for (int index = 0; index >= 0; index = hierarchy[index][0]) {

                Moments moment = moments((cv::Mat)contours[index]);
                double area = moment.m00;

                //if the area is less than 20 px by 20px then it is probably just noise
                //if the area is the same as the 3/2 of the image size, probably just a bad filter
                //we only want the object with the largest area so we safe a reference area each
                //iteration and compare it to the area in the next iteration.
                if(area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA && area>refArea){
                    x = moment.m10/area;
                    y = moment.m01/area;
                    objectFound = true;
                    refArea = area;
                }else objectFound = false;


            }
            //let user know you found an object
            if(objectFound ==true){
                putText(cameraFeed,"Tracking Object",Point(0,50),2,1,Scalar(0,255,0),2);
                //draw object location on screen
                drawObject(x,y,cameraFeed);}

        }else putText(cameraFeed,"TOO MUCH NOISE! ADJUST FILTER",Point(0,50),1,2,Scalar(0,0,255),2);
    }
}
#endif

TrackingThread::TrackingThread(int camID)
{
    crossHair.x = 0;
    crossHair.y = 0;

    trackingEnabled = false;
    showCamera = true;
    cameraDevice = camID;
    active = true;

    qDebug() << "Starting TrackingThread";
    start(NormalPriority);
}

void TrackingThread::StartTracking()
{
    trackingEnabled = true;
}

void TrackingThread::StopTracking()
{
    trackingEnabled = false;
}

void TrackingThread::ShowCamera(bool yesno)
{
    showCamera = yesno;
}

#if 0
void onMouse(int event, int x, int y, int flags, void* param)
{
    circle(storedFrame,Point(x,y),20,Scalar(0,255,0),2);

    _newMousePos = true;
    mX = x;
    mY = y;

    imshow("Frame1",storedFrame);
}



double hue_min=20;
double hue_max=160;
double sat_min=100;
double sat_max=255;
double val_min=200;
double val_max=256;

void threshold_image(Mat &channel, int chanNum)
{
    Mat temp;
    double minimum;
    double maximum ;

    if (channel == 0) {
        minimum = hue_min;
        maximum = hue_max;
    } else if (channel == 1) {
        minimum = sat_min;
        maximum = sat_max;
    } else if (channel == 2) {
        minimum = val_min;
        maximum = val_max;
    }

    cv::threshold(channel, // input array (single-channel, 8-bit or 32-bit floating point)
                  temp,    // output array of the same size and type as src
                  maximum, // threshold value
                  0, // maximum value to use with the THRESH_BINARY and THRESH_BINARY_INV thresholding types
                  THRESH_TOZERO_INV // thresholding type
                  );

    cv::threshold(temp, // input array (single-channel, 8-bit or 32-bit floating point)
                  channel,    // output array of the same size and type as src
                  minimum, // threshold value
                  255, // maximum value to use with the THRESH_BINARY and THRESH_BINARY_INV thresholding types
                  THRESH_BINARY // thresholding type
                  );

    if (channel == 0) {
        // only works for filtering red color because the range for the hue is split
        cv::bitwise_not(channel, channel);
    }

}

void detect(Mat &frame)
{
    Mat hsv_img ;
    Mat laser;
    vector<Mat> channels;

    cv::resize(frame, frame, 600);

    cv::cvtColor(frame, hsv_img ,COLOR_BGR2HSV);

    cv::split(hsv_img, channels);

    // Threshold ranges of HSV components; storing the results in place
    threshold_image(channels[0], 0); // hue
    threshold_image(channels[1], 1); // saturation
    threshold_image(channels[2], 2); // value

    // Perform an AND on HSV components to identify the laser!
    cv::bitwise_and(channels[0], channels[2], laser);
    cv::bitwise_and(channels[1], laser, laser);

    // Merge the HSV components back together
    cv::merge(channels, hsv_img);

    Mat temp;
    laser.copyTo(temp);
    //these two vectors needed for output of findContours
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;
    //find contours of filtered image using openCV findContours function
    cv::findContours(temp, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    // only proceed if at least one contour was found
    if (contours.size() > 0) {
        max()
    }

}
#endif

void TrackingThread::exitThread()
{
    active = false;
}

void TrackingThread::run()
{
    qDebug() << "TrackingThread Started";

   //qDebug("OpenCV: %s", cv::getBuildInformation().c_str());

    cv::VideoCapture capVideo;
    cv::Mat imgFrame1;
    cv::Mat imgFrame2;
    std::vector<Blob> blobs;

    capVideo.open(cameraDevice);  //open capture object at location zero (default location for webcam)
    //set height and width of capture frame
    capVideo.set(CV_CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);
    capVideo.set(CV_CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);

    if (!capVideo.isOpened()) {                                                 // if unable to open video file
        std::cout << "error reading video file" << std::endl << std::endl;      // show error message
        return;                                                           // and exit program
    }

    capVideo.read(imgFrame1);

    bool blnFirstFrame = true;
    int frameCount = 2;

    while(active) {

        if(trackingEnabled) {

            capVideo.read(imgFrame2);

            std::vector<Blob> currentFrameBlobs;

            cv::Mat imgFrame1Copy = imgFrame1.clone();
            cv::Mat imgFrame2Copy = imgFrame2.clone();

            cv::Mat imgDifference;
            cv::Mat imgThresh;

            cv::cvtColor(imgFrame1Copy, imgFrame1Copy, CV_BGR2GRAY);
            cv::cvtColor(imgFrame2Copy, imgFrame2Copy, CV_BGR2GRAY);

            cv::GaussianBlur(imgFrame1Copy, imgFrame1Copy, cv::Size(5, 5), 0);
            cv::GaussianBlur(imgFrame2Copy, imgFrame2Copy, cv::Size(5, 5), 0);

            cv::absdiff(imgFrame1Copy, imgFrame2Copy, imgDifference);

            cv::threshold(imgDifference, imgThresh, 30, 255.0, CV_THRESH_BINARY);

            //cv::imshow("imgThresh", imgThresh);

            cv::Mat structuringElement3x3 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
            cv::Mat structuringElement5x5 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
            cv::Mat structuringElement7x7 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7));
            cv::Mat structuringElement9x9 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9, 9));

            cv::dilate(imgThresh, imgThresh, structuringElement5x5);
            cv::dilate(imgThresh, imgThresh, structuringElement5x5);
            cv::erode(imgThresh, imgThresh, structuringElement5x5);

            cv::Mat imgThreshCopy = imgThresh.clone();

            std::vector<std::vector<cv::Point> > contours;
            cv::findContours(imgThreshCopy, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

            //drawAndShowContours(imgThresh.size(), contours, "imgContours");
            std::vector<std::vector<cv::Point> > convexHulls(contours.size());

            for (unsigned int i = 0; i < contours.size(); i++) {
                cv::convexHull(contours[i], convexHulls[i]);
            }

            //drawAndShowContours(imgThresh.size(), convexHulls, "imgConvexHulls");

            for (auto &convexHull : convexHulls) {
                Blob possibleBlob(convexHull);
                if (possibleBlob.currentBoundingRect.area() > 100 &&
                        possibleBlob.dblCurrentAspectRatio >= 0.2 &&
                        possibleBlob.dblCurrentAspectRatio <= 1.25 &&
                        possibleBlob.currentBoundingRect.width > 20 &&
                        possibleBlob.currentBoundingRect.height > 20 &&
                        possibleBlob.dblCurrentDiagonalSize > 30.0 &&
                        (cv::contourArea(possibleBlob.currentContour) / (double)possibleBlob.currentBoundingRect.area()) > 0.40) {
                    currentFrameBlobs.push_back(possibleBlob);
                }
            }

            //drawAndShowContours(imgThresh.size(), currentFrameBlobs, "imgCurrentFrameBlobs");

            if (blnFirstFrame == true) {
                for (auto &currentFrameBlob : currentFrameBlobs) {
                    blobs.push_back(currentFrameBlob);
                }
            }
            else {
                matchCurrentFrameBlobsToExistingBlobs(blobs, currentFrameBlobs);
            }
            //drawAndShowContours(imgThresh.size(), blobs, "imgBlobs");

            imgFrame2Copy = imgFrame2.clone();          // get another copy of frame 2 since we changed the previous frame 2 copy in the processing above

            findTargetBlob(blobs);
            if (showCamera) {
                drawBlobInfoOnImage(blobs, imgFrame2Copy);
                cv::imshow("cameraview", imgFrame2Copy);
            }

            currentFrameBlobs.clear();
            imgFrame1 = imgFrame2.clone();           // move frame 1 up to where frame 2 is

            //capVideo.read(imgFrame2);
            if (lockedOnABlob) {
                emit sig_newBlobPos(crossHair.x, crossHair.y);
            }

            blnFirstFrame = false;
            frameCount++;

        } else {
            if (showCamera) {
                cv::imshow("cameraview", imgFrame1);
            }

            capVideo.read(imgFrame1);
        }

        waitKey(1);
    }

    qDebug() << "Leaving tracking thread";
    destroyAllWindows();

}

///////////////////////////////////////////////////////////////////////////////////////////////////
void matchCurrentFrameBlobsToExistingBlobs(std::vector<Blob> &existingBlobs, std::vector<Blob> &currentFrameBlobs) {

    for (auto &existingBlob : existingBlobs) {

        existingBlob.blnCurrentMatchFoundOrNewBlob = false;

        existingBlob.predictNextPosition();
    }

    for (auto &currentFrameBlob : currentFrameBlobs) {

        int intIndexOfLeastDistance = 0;
        double dblLeastDistance = 100000.0;

        for (unsigned int i = 0; i < existingBlobs.size(); i++) {
            if (existingBlobs[i].blnStillBeingTracked == true) {
                double dblDistance = distanceBetweenPoints(currentFrameBlob.centerPositions.back(), existingBlobs[i].predictedNextPosition);

                if (dblDistance < dblLeastDistance) {
                    dblLeastDistance = dblDistance;
                    intIndexOfLeastDistance = i;
                }
            }
        }

        if (dblLeastDistance < currentFrameBlob.dblCurrentDiagonalSize * 1.15) {
            addBlobToExistingBlobs(currentFrameBlob, existingBlobs, intIndexOfLeastDistance);
        }
        else {
            addNewBlob(currentFrameBlob, existingBlobs);
        }

    }

    for (auto &existingBlob : existingBlobs) {

        if (existingBlob.blnCurrentMatchFoundOrNewBlob == false) {
            existingBlob.intNumOfConsecutiveFramesWithoutAMatch++;
        }

        if (existingBlob.intNumOfConsecutiveFramesWithoutAMatch >= 5) {
            existingBlob.blnStillBeingTracked = false;
        }

    }

}

///////////////////////////////////////////////////////////////////////////////////////////////////
void addBlobToExistingBlobs(Blob &currentFrameBlob, std::vector<Blob> &existingBlobs, int &intIndex) {

    existingBlobs[intIndex].currentContour = currentFrameBlob.currentContour;
    existingBlobs[intIndex].currentBoundingRect = currentFrameBlob.currentBoundingRect;

    existingBlobs[intIndex].centerPositions.push_back(currentFrameBlob.centerPositions.back());

    existingBlobs[intIndex].dblCurrentDiagonalSize = currentFrameBlob.dblCurrentDiagonalSize;
    existingBlobs[intIndex].dblCurrentAspectRatio = currentFrameBlob.dblCurrentAspectRatio;

    existingBlobs[intIndex].blnStillBeingTracked = true;
    existingBlobs[intIndex].blnCurrentMatchFoundOrNewBlob = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void addNewBlob(Blob &currentFrameBlob, std::vector<Blob> &existingBlobs) {

    currentFrameBlob.blnCurrentMatchFoundOrNewBlob = true;

    existingBlobs.push_back(currentFrameBlob);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
double distanceBetweenPoints(cv::Point point1, cv::Point point2) {

    int intX = abs(point1.x - point2.x);
    int intY = abs(point1.y - point2.y);

    return(sqrt(pow(intX, 2) + pow(intY, 2)));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void drawAndShowContours(cv::Size imageSize, std::vector<std::vector<cv::Point> > contours, std::string strImageName) {
    cv::Mat image(imageSize, CV_8UC3, SCALAR_BLACK);

    cv::drawContours(image, contours, -1, SCALAR_WHITE, -1);

    cv::imshow(strImageName, image);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void drawAndShowContours(cv::Size imageSize, std::vector<Blob> blobs, std::string strImageName) {

    cv::Mat image(imageSize, CV_8UC3, SCALAR_BLACK);

    std::vector<std::vector<cv::Point> > contours;

    for (auto &blob : blobs) {
        if (blob.blnStillBeingTracked == true) {
            contours.push_back(blob.currentContour);
        }
    }

    cv::drawContours(image, contours, -1, SCALAR_WHITE, -1);

    cv::imshow(strImageName, image);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void drawBlobInfoOnImage(std::vector<Blob> &blobs, cv::Mat &imgFrame2Copy) {

    for (unsigned int i = 0; i < blobs.size(); i++) {

        if (blobs[i].blnStillBeingTracked == true) {

            if ((lockedOnABlob) && (i == currentLockedBlob)) {
                cv::rectangle(imgFrame2Copy, blobs[i].currentBoundingRect, SCALAR_RED, 2);
            } else {
                cv::rectangle(imgFrame2Copy, blobs[i].currentBoundingRect, SCALAR_GREEN, 2);
            }

            int intFontFace = CV_FONT_HERSHEY_SIMPLEX;
            double dblFontScale = blobs[i].dblCurrentDiagonalSize / 60.0;
            int intFontThickness = (int)std::round(dblFontScale * 1.0);

            cv::putText(imgFrame2Copy, std::to_string(i), blobs[i].centerPositions.back(), intFontFace, dblFontScale, SCALAR_GREEN, intFontThickness);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void findTargetBlob(std::vector<Blob> &blobs)
{
    if (!lockedOnABlob) {

        qDebug() << "!lockedOnABlob";

        // getCurrentCrossHairPos()

        // if have blobs
        //      find distances to other blobs, loop through blobs

        double shortestdistance;
        bool haveFirst = false;
        for (unsigned int i = 0; i < blobs.size(); i++) {
            if (blobs[i].blnStillBeingTracked == true) {
                Point center_of_blob = (blobs[i].currentBoundingRect.br() + blobs[i].currentBoundingRect.tl())*0.5;

                double res = cv::norm(center_of_blob - crossHair);//Euclidian distance                

                if (!haveFirst) {
                    shortestdistance = abs(res);
                    haveFirst = true;
                }
                qDebug() << "distances" << shortestdistance << res << abs(res) << i;

                if (abs(res) <= shortestdistance) {
                    shortestdistance = abs(res);
                    currentLockedBlob = i;
                    lockedOnABlob = true;

                    qDebug() << "lockedOnABlob" << currentLockedBlob;
                }
            }
        }
    }

    if (lockedOnABlob) {

        if (blobs[currentLockedBlob].blnStillBeingTracked == false) {
            lockedOnABlob = false;
        } else {
            // set crossHair to blob centre
            crossHair = (blobs[currentLockedBlob].currentBoundingRect.br() + blobs[currentLockedBlob].currentBoundingRect.tl())*0.5;

            //
            //emit sig_newBlobPos(crossHair.x, crossHair.y);
        }


    }


}
