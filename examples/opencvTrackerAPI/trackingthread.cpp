#include "trackingthread.h"
#include <QDebug>

//our sensitivity value to be used in the threshold() function
const static int SENSITIVITY_VALUE = 20;
//size of blur used to smooth the image to remove possible noise and
//increase the size of the object we are trying to track. (Much like dilate and erode)
const static int BLUR_SIZE = 10;
//we'll have just one object to search for
//and keep track of its position.
int theObject[2] = {0,0};
//bounding rectangle of the object, we will use the center of this as its position.
Rect objectBoundingRectangle = Rect(0,0,0,0);

TrackingThread::TrackingThread()
{
    qDebug() << "Starting TrackingThread";
    start(NormalPriority);
}

//int to string helper function
string TrackingThread::intToString(int number){

    //this function has a number input and string output
    std::stringstream ss;
    ss << number;
    return ss.str();
}

void TrackingThread::searchForMovement(Mat thresholdImage, Mat &cameraFeed)
{
    //notice how we use the '&' operator for the cameraFeed. This is because we wish
    //to take the values passed into the function and manipulate them, rather than just working with a copy.
    //eg. we draw to the cameraFeed in this function which is then displayed in the main() function.
    bool objectDetected=false;
    Mat temp;
    thresholdImage.copyTo(temp);
    //these two vectors needed for output of findContours
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;
    //find contours of filtered image using openCV findContours function
    //findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );// retrieves all contours
    findContours(temp,contours,hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE );// retrieves external contours

    //if contours vector is not empty, we have found some objects
    if(contours.size()>0)objectDetected=true;
    else objectDetected = false;

    if(objectDetected){
        //the largest contour is found at the end of the contours vector
        //we will simply assume that the biggest contour is the object we are looking for.
        vector< vector<Point> > largestContourVec;
        largestContourVec.push_back(contours.at(contours.size()-1));
        //make a bounding rectangle around the largest contour then find its centroid
        //this will be the object's final estimated position.
        objectBoundingRectangle = boundingRect(largestContourVec.at(0));
        int xpos = objectBoundingRectangle.x+objectBoundingRectangle.width/2;
        int ypos = objectBoundingRectangle.y+objectBoundingRectangle.height/2;



        //update the objects positions by changing the 'theObject' array values
        theObject[0] = xpos , theObject[1] = ypos;

        //sig_changeAngle( (300 - ypos) *  .0153994);
    } /*else {
        qDebug() << "no object";
    }*/
    //make some temp x and y variables so we dont have to type out so much
    int x = theObject[0];
    int y = theObject[1];

    rectangle(cameraFeed, objectBoundingRectangle, Scalar(0,255,0));

    //draw some crosshairs on the object
    circle(cameraFeed,Point(x,y),20,Scalar(0,255,0),2);
    line(cameraFeed,Point(x,y),Point(x,y-25),Scalar(0,255,0),2);
    line(cameraFeed,Point(x,y),Point(x,y+25),Scalar(0,255,0),2);
    line(cameraFeed,Point(x,y),Point(x-25,y),Scalar(0,255,0),2);
    line(cameraFeed,Point(x,y),Point(x+25,y),Scalar(0,255,0),2);
    putText(cameraFeed,"Tracking object at (" + intToString(x)+","+intToString(y)+")",Point(x,y),1,1,Scalar(255,0,0),2);

}

//default capture width and height
const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;

void TrackingThread::run()
{
    qDebug() << "TrackingThread Started";  

    qDebug("OpenCV: %s", cv::getBuildInformation().c_str());

    cv::Ptr<cv::Tracker> tracker = cv::Tracker::create("KCF");

    Mat cameraFeedFrame;
    Mat storedFrame;
    bool haveFirstFrame = false;

    //some boolean variables for added functionality
    //bool objectDetected = false;
    //these two can be toggled by pressing 'd' or 't'
    bool debugMode = false;
    bool trackingEnabled = false;
    //pause and resume code
    bool pause = false;
    //set up the matrices that we will need
    //the two frames we will be comparing
    Mat frame1,frame2;
    //their grayscale images (needed for absdiff() function)
    Mat grayImage1,grayImage2;
    //resulting difference image
    Mat differenceImage;
    //thresholded difference image (for use in findContours() function)
    Mat thresholdImage;
    //video capture object.
    VideoCapture capture;

    //open capture object at location zero (default location for webcam)
    capture.open(0);

    if(!capture.isOpened()){
        qDebug() << "ERROR ACQUIRING VIDEO FEED\n";
        return;
    }

    //set height and width of capture frame
    capture.set(CV_CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);

    //start an infinite loop where webcam feed is copied to cameraFeed matrix
    //all of our operations will be performed within this loop

    while(1){

        //store image to matrix
        capture.read(cameraFeedFrame);

        if (haveFirstFrame) {

            //convert frame1 to gray scale for frame differencing
            cv::cvtColor(storedFrame, grayImage1, COLOR_BGR2GRAY);

            //convert frame2 to gray scale for frame differencing
            cv::cvtColor(cameraFeedFrame, grayImage2, COLOR_BGR2GRAY);
            //perform frame differencing with the sequential images. This will output an "intensity image"
            //do not confuse this with a threshold image, we will need to perform thresholding afterwards.
            cv::absdiff(grayImage1, grayImage2, differenceImage);
            //threshold intensity image at a given sensitivity value
            cv::threshold(differenceImage, thresholdImage, SENSITIVITY_VALUE, 255, THRESH_BINARY);

            if(debugMode==true){
                //show the difference image and threshold image
                cv::imshow("Difference Image", differenceImage);
                cv::imshow("Threshold Image", thresholdImage);
            }else{
                //if not in debug mode, destroy the windows so we don't see them anymore
                destroyWindow("Difference Image");
                destroyWindow("Threshold Image");
            }
            //use blur() to smooth the image, remove possible noise and
            //increase the size of the object we are trying to track. (Much like dilate and erode)
            cv::blur(thresholdImage, thresholdImage, cv::Size(BLUR_SIZE, BLUR_SIZE));
            //threshold again to obtain binary image from blur output
            cv::threshold(thresholdImage, thresholdImage, SENSITIVITY_VALUE, 255, THRESH_BINARY);
            if(debugMode==true){
                //show the threshold image after it's been "blurred"
                cv::imshow("Final Threshold Image", thresholdImage);
            }
            else {
                //if not in debug mode, destroy the windows so we don't see them anymore
                destroyWindow("Final Threshold Image");
            }

            //if tracking enabled, search for contours in our thresholded image
            if (trackingEnabled) {
                searchForMovement(thresholdImage, storedFrame);

            }

            //show our captured frame
            imshow("Frame1",storedFrame);

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

        } else {
            haveFirstFrame = true;
        }

        cameraFeedFrame.copyTo(storedFrame);
    }
}
