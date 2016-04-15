#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

int main()
{    
    cout << "prueba 1 " << endl;
    Mat img;
    char key;

    while(1) {       

        img = imread("swirl_flat.png", CV_LOAD_IMAGE_COLOR);
        if(! img.data ) {
            cout <<  "Could not open or find the image" << endl ;
            return -1;
        }

        namedWindow("exit", WINDOW_AUTOSIZE );
        //imshow("exit", img);
        key = waitKey(1);
        if (key == 'q') {
            destroyAllWindows();
            break;
        }
    }

    return 0;
}
