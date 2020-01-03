#include "trackingthread.h"
#include <QDebug>

using namespace cv;

//default capture width and height
const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;

//initial min and max HSV filter values.
//these will be changed using trackbars
int H_MIN = 0;
int H_MAX = 256;
int S_MIN = 0;
int S_MAX = 256;
int V_MIN = 0;
int V_MAX = 256;
//default capture width and height
//const int FRAME_WIDTH = 640;
//const int FRAME_HEIGHT = 480;
//max number of objects to be detected in frame
const int MAX_NUM_OBJECTS=50;
//minimum and maximum object area
const int MIN_OBJECT_AREA = 20*20;
const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH/1.5;
//names that will appear at the top of each window
const string windowName = "Original Image";
const string windowName1 = "HSV Image";
const string windowName2 = "Thresholded Image";
const string windowName3 = "After Morphological Operations";
const string trackbarWindowName = "Trackbars";

Mat storedFrame;

static bool _newMousePos = false;
static int mX;
static int mY;

void on_trackbar( int, void* )
{//This function gets called whenever a
    // trackbar position is changed
}

string intToString(int number)
{
    std::stringstream ss;
    ss << number;
    return ss.str();
}

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

TrackingThread::TrackingThread()
{
    qDebug() << "Starting TrackingThread";
    start(NormalPriority);
}

void onMouse(int event, int x, int y, int flags, void* param)
{
    circle(storedFrame,Point(x,y),20,Scalar(0,255,0),2);

    _newMousePos = true;
    mX = x;
    mY = y;

    imshow("Frame1",storedFrame);
}


#if 0
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

void TrackingThread::run()
{
    qDebug() << "TrackingThread Started";  

    qDebug("OpenCV: %s", cv::getBuildInformation().c_str());   

    //Mat cameraFeedFrame;
    //Mat storedFrame;
   // bool haveFirstFrame = false;
    bool debugMode = false;
    bool trackingEnabled = false;    
    bool pause = false;

   // bool trackObjects = true;
    //bool useMorphOps = false;

    //matrix storage for HSV image
    //Mat HSV;

    //matrix storage for binary threshold image
    //Mat threshold;
    //x and y values for the location of the object
    //int x=0, y=0;
    //create slider bars for HSV filtering
    //createTrackbars();

    //namedWindow(windowName);

    //cv::setMouseCallback(windowName, onMouse, 0);

    VideoCapture capture;            
    capture.open(0);  //open capture object at location zero (default location for webcam)

    if(!capture.isOpened()){
        qDebug() << "ERROR ACQUIRING VIDEO FEED\n";
        return;
    }

    //set height and width of capture frame
    capture.set(CV_CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);


    // creating various necessary staff
    cv::Mat matOriginal;
    cv::Mat matOriginalHSV;
    cv::Mat matProcessed;

    std::vector<cv::Vec3f> vecCircles;
    std::vector<cv::Vec3f>::iterator itrCircles;


    cv::namedWindow("Original", CV_WINDOW_AUTOSIZE);
    cv::namedWindow("Processed", CV_WINDOW_AUTOSIZE);

    // creating trackbars
    cv::namedWindow("Trackbar", CV_WINDOW_AUTOSIZE);
    int hmin = 0, smin = 0, vmin = 0,
            hmax = 180, smax = 256, vmax = 256;
    cv::createTrackbar("H min:", "Trackbar", &hmin, hmax);
    cv::createTrackbar("H max:", "Trackbar", &hmax, hmax);
    cv::createTrackbar("S min:", "Trackbar", &smin, smax);
    cv::createTrackbar("S max:", "Trackbar", &smax, smax);
    cv::createTrackbar("V min:", "Trackbar", &vmin, vmax);
    cv::createTrackbar("V max:", "Trackbar", &vmax, vmax);


    while(1) {

        /*if (_newMousePos) {
            emit sig_mousePosition(mX, mY);
            _newMousePos = false;
        }*/
#if 0
        capture.read(cameraFeedFrame);

        //convert frame from BGR to HSV colorspace
        cvtColor(cameraFeedFrame,HSV,COLOR_BGR2HSV);

        //filter HSV image between values and store filtered image to
        //threshold matrix
        inRange(HSV,Scalar(H_MIN,S_MIN,V_MIN),Scalar(H_MAX,S_MAX,V_MAX),threshold);

        //perform morphological operations on thresholded image to eliminate noise
        //and emphasize the filtered object(s)
        if(useMorphOps) {
            morphOps(threshold);
        }

        //pass in thresholded frame to our object tracking function
        //this function will return the x and y coordinates of the
        //filtered object
        if(trackObjects) {
            trackFilteredObject(x,y,threshold,cameraFeedFrame);
        }

        //if (haveFirstFrame) {

        //show frames
        imshow(windowName2,threshold);
        imshow(windowName,cameraFeedFrame);
        imshow(windowName1,HSV);
#endif

        capture.read(matOriginal);

        // converting to HSV
        cv::cvtColor(matOriginal, matOriginalHSV, CV_BGR2HSV);

        // spliting channels, changing any channel from "matOriginalHSVchannels[0/1/2]" to "matZero" to remove it
        cv::Mat matZero;
        cv::Mat matOriginalHS;
        matZero = cv::Mat::zeros(cv::Size(matOriginalHSV.cols, matOriginalHSV.rows), CV_8UC1);
        vector<cv::Mat> matOriginalHSVchannels(3);
        cv::split(matOriginalHSV, matOriginalHSVchannels);
        vector<cv::Mat> channels;
        channels.push_back(matOriginalHSVchannels[0]);
        channels.push_back(matOriginalHSVchannels[1]);
        channels.push_back(matZero);
        cv::merge(channels, matOriginalHS);

        // search of laser spot
        cv::inRange(matOriginalHS, cv::Scalar(hmin, smin, vmin), cv::Scalar(hmax, smax, vmax), matProcessed);

        // blur
        cv::GaussianBlur(matProcessed,
                         matProcessed,
                         cv::Size(5, 5),
                         2.2);

        // circles search
        cv::HoughCircles(matProcessed,
                         vecCircles,
                         CV_HOUGH_GRADIENT,
                         1,
                         matProcessed.rows / 8,
                         100,
                         50,
                         7,
                         20);

        // draw circles
        for (itrCircles = vecCircles.begin(); itrCircles != vecCircles.end(); itrCircles++) {
            std::cout << "position x = " << (*itrCircles)[0]
                      << ", y = " << (*itrCircles)[1]
                      << ", z = " << (*itrCircles)[2] << "\n";

            cv::circle(matOriginalHS,
                       cv::Point((int)(*itrCircles)[0], (int)(*itrCircles)[1]),
                    1,
                    cv::Scalar(255, 0, 0),
                    CV_FILLED);

            cv::circle(matOriginalHS,
                       cv::Point((int)(*itrCircles)[0], (int)(*itrCircles)[1]),
                    (int)(*itrCircles)[2],
                    cv::Scalar(255, 255, 0),
                    2);

            // set mouse cursor on laser coordinates
            int screenx = (int)((*itrCircles)[0] * 2.2);
            int screeny = (int)((*itrCircles)[1] * 1.2);
            drawObject(screenx, screeny, matOriginalHS);

        }
        // show windows
        cv::imshow("Original", matOriginalHS);
        cv::imshow("Processed", matProcessed);



        //imshow("Frame1",storedFrame);

        //check to see if a button has been pressed.
        //this 10ms delay is necessary for proper operation of this program
        //if removed, frames will not have enough time to referesh and a blank
        //image will appear.
        switch(waitKey(10)){

        case 27: //'esc' key has been pressed, exit program.
            //return 0;
            return;
        case 116: //'t' has been pressed. this will toggle tracking
            trackingEnabled = !trackingEnabled;
            if(trackingEnabled == false) cout<<"Tracking disabled."<<endl;
            else cout<<"Tracking enabled."<<endl;
            break;
        case 100: //'d' has been pressed. this will debug mode
            debugMode = !debugMode;
            if(debugMode == false) cout<<"Debug mode disabled."<<endl;
            else cout<<"Debug mode enabled."<<endl;
            break;
        case 112: //'p' has been pressed. this will pause/resume the code.
            pause = !pause;
            if(pause == true){ cout<<"Code paused, press 'p' again to resume"<<endl;
                while (pause == true){
                    //stay in this loop until
                    switch (waitKey()){
                    //a switch statement inside a switch statement? Mind blown.
                    case 112:
                        //change pause back to false
                        pause = false;
                        cout<<"Code resumed."<<endl;
                        break;
                    }
                }
            }
        }

        /*} else {
            haveFirstFrame = true;
        }*/

        //cameraFeedFrame.copyTo(storedFrame);
    }
}
