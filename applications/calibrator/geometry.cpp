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
    //double radianX, radianY;

    //_tiltmin = tiltmin;
    //_tiltmax = tiltmax;
    //_panmin = panmin; // calibrate
    //_panmax = panmax;
    _tilthome = (tiltmax+tiltmin)/2;
    _panhome = (panmax+panmin)/2;

    _cameraX = cameraX;
    _cameraY = cameraY;
    _calDepth = calDepth;
    _calXDistance = calXDistance;
    _calYDistance = calYDistance;

    _calRadianX = atan2(_calXDistance/2, _calDepth);
    _calRadianY = atan2(_calYDistance/2, _calDepth);

    //_servoXstepSize = calcServoStepSize(6890, 5027, 730.00, 825.00);
    //_servoYstepSize = calcServoStepSize(6092, 4527, 500.00, 745.00);
    _servoXstepSize = calcServoStepSize(panmax, panmin, _calXDistance, _calDepth);
    _servoYstepSize = calcServoStepSize(tiltmax, tiltmin, _calYDistance, _calDepth);

    //radianX = atan2(_calXDistance, _calDepth);
    //_servoXstepSize =  (_panmax - _panmin) / radianX;// 1/4 microseconds / radian

    //radianY = atan2(_calXDistance, _calDepth);
    //_servoYstepSize =  (_tiltmax - _tiltmin) / radianY;

    setNewDepth(_calDepth);
}

int Geometry::calcServoStepSize(int servomax,
                                int servomin,
                                qreal calDistance,
                                qreal calDepth)
{
    double radian;
    int steps;
    radian = atan2(calDistance, calDepth);
    steps =  (servomax - servomin) / radian;// 1/4 microseconds / radian

    return steps;
}

void Geometry::setNewDepth(double depth)
{
    double maxDistanceAtDepth;

    _depth = depth;
    maxDistanceAtDepth = _depth * tan(_calRadianX);
    _distXPerPixel = maxDistanceAtDepth / (_cameraX/2);

    maxDistanceAtDepth = _depth * tan(_calRadianY);
    _distYPerPixel = maxDistanceAtDepth / (_cameraY/2);
}

int Geometry::getServoTiltAngle(int y)
{
    int qms;
    double radian, distanceY;

    if (y <= (_cameraY/2)) {
        // upper
        distanceY = pixelToDistanceY((double)((_cameraY/2) - y));
        radian = atan2(distanceY, _depth);
        qms = _tilthome - (radian * _servoYstepSize);

        //qms = ((_tiltmax+_tiltmin)/2) + (radian * _servoYstepSize);

    } else {
        // lower
        distanceY = pixelToDistanceY((double)(y - (_cameraY/2)));
        radian = atan2(distanceY, _depth);
        qms = _tilthome + (radian * _servoYstepSize);

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
        radian = atan2(distanceX, _depth);
       // qms = ((_panmax+_panmin)/2) - (radian * _servoXstepSize);

        /*distanceX = pixelToDistanceX((double)(x - (_cameraX/2)));
        radian = atan2(distanceX, _calDepth);
        qms = ((_panmax+_panmin)/2) + (radian * _servoXstepSize);*/

        qms = _panhome + (radian * _servoXstepSize);


    } else {
        // rightside
        distanceX = pixelToDistanceX((double)(x - (_cameraX/2)));
        radian = atan2(distanceX, _depth);
        //qms = ((_panmax+_panmin)/2) + (radian * _servoXstepSize);

        /*distanceX = pixelToDistanceX((double)((_cameraX/2) - x));
        radian = atan2(distanceX, _calDepth);
        qms = ((_panmax+_panmin)/2) - (radian * _servoXstepSize);*/

        qms = _panhome - (radian * _servoXstepSize);
    }

    return qms;
}

double Geometry::pixelToDistanceX(double pixelDistance)
{
    // get max distance at depth
    // -> mm / pixel
    // work out distance from pixel value
    return (pixelDistance * _distXPerPixel);
    //return (pixelDistance * (double)( (double)_calXDistance / (double)_cameraX ));  //   mm = pixels * ( mm / pixel )
}

double Geometry::pixelToDistanceY(double pixelDistance)
{
    return (pixelDistance * _distYPerPixel);
    //return (pixelDistance * (double)( (double)_calYDistance / (double)_cameraY ));  //   mm = pixels * ( mm / pixel )
}
