#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QtCore>

class Geometry
{
public:
    Geometry(int tiltmin,
             int tiltmax,
             int panmin,
             int panmax,
             int cameraX,
             int cameraY,
             double calDepth,
             double calXDistance,
             double calYDistance);

    int getServoTiltAngle(int y);
    int getServoPanAngle(int x);    
    int calcServoStepSize(int servomax,
                          int servomin,
                          qreal calDistance,
                          qreal calDepth);
    void setNewDepth(double depth);

private:
    double pixelToDistanceX(double pixelDistance);
    double pixelToDistanceY(double pixelDistance);

    //int _tiltmin;
    //int _tiltmax;
    int _tilthome;
    //int _panmin;
   // int _panmax;
    int _panhome;
    int _cameraX;
    int _cameraY;
    double _calDepth;
    double _calXDistance;
    double _calYDistance;
    int _servoXstepSize;
    int _servoYstepSize;
    double _calRadianX;
    double _calRadianY;
    double _depth;
    double _distXPerPixel;
    double _distYPerPixel;

};

#endif // GEOMETRY_H
