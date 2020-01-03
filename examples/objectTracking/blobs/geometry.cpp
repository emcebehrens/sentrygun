#include "geometry.h"
#include <math.h>

Geometry::Geometry(int tiltmin,
                   int tiltmax,
                   int panmin,
                   int panmax,
                   int cameraX,
                   int cameraY,
                   double calDepth,
                   double calXDistance,
                   double calYDistance)
{
    double radianX, radianY;

    _tiltmin = tiltmin;
    _tiltmax = tiltmax;
    _panmin = panmin; // calibrate
    _panmax = panmax;
    _cameraX = cameraX;
    _cameraY = cameraY;
    _calDepth = calDepth;
    _calXDistance = calXDistance;
    _calYDistance = calYDistance;

    radianX = atan2(_calXDistance, _calDepth);
    _servoXstepSize =  (_panmax - _panmin) / radianX;// 1/4 microseconds / radian

    radianY = atan2(_calXDistance, _calDepth);
    _servoYstepSize =  (_tiltmax - _tiltmin) / radianY;
}

int Geometry::getServoTiltAngle(int y)
{
    int qms;
    double radian, distanceY;

    if (y <= (_cameraY/2)) {
        // upper
        distanceY = pixelToDistanceY((double)((_cameraY/2) - y));
        radian = atan2(distanceY, _calDepth);
        qms = ((_tiltmax+_tiltmin)/2) - (radian * _servoYstepSize);

        //qms = ((_tiltmax+_tiltmin)/2) + (radian * _servoYstepSize);

    } else {
        // lower
        distanceY = pixelToDistanceY((double)(y - (_cameraY/2)));
        radian = atan2(distanceY, _calDepth);
        qms = ((_tiltmax+_tiltmin)/2) + (radian * _servoYstepSize);

        //qms = ((_tiltmax+_tiltmin)/2) - (radian * _servoYstepSize);

    }

    return qms;
}

int Geometry::getServoPanAngle(int x)
{
    int qms;
    double radian, distanceX;

    if (x <= (_cameraX/2)) {
        // leftside
        distanceX = pixelToDistanceX((double)((_cameraX/2) - x));
        radian = atan2(distanceX, _calDepth);
       // qms = ((_panmax+_panmin)/2) - (radian * _servoXstepSize);

        /*distanceX = pixelToDistanceX((double)(x - (_cameraX/2)));
        radian = atan2(distanceX, _calDepth);
        qms = ((_panmax+_panmin)/2) + (radian * _servoXstepSize);*/

        qms = ((_panmax+_panmin)/2) + (radian * _servoXstepSize);


    } else {
        // rightside
        distanceX = pixelToDistanceX((double)(x - (_cameraX/2)));
        radian = atan2(distanceX, _calDepth);
        //qms = ((_panmax+_panmin)/2) + (radian * _servoXstepSize);

        /*distanceX = pixelToDistanceX((double)((_cameraX/2) - x));
        radian = atan2(distanceX, _calDepth);
        qms = ((_panmax+_panmin)/2) - (radian * _servoXstepSize);*/

        qms = ((_panmax+_panmin)/2) - (radian * _servoXstepSize);
    }

    return qms;
}

double Geometry::pixelToDistanceX(double pixelDistance)
{
    return (pixelDistance * (double)( (double)_calXDistance / (double)_cameraX ));  //   mm = pixels * ( mm / pixel )
}

double Geometry::pixelToDistanceY(double pixelDistance)
{
    return (pixelDistance * (double)( (double)_calYDistance / (double)_cameraY ));  //   mm = pixels * ( mm / pixel )
}
