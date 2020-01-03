#ifndef GEOMETRY_H
#define GEOMETRY_H

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


private:
    double pixelToDistanceX(double pixelDistance);
    double pixelToDistanceY(double pixelDistance);

    int _tiltmin;
    int _tiltmax;
    int _panmin;
    int _panmax;
    int _cameraX;
    int _cameraY;
    double _calDepth;
    double _calXDistance;
    double _calYDistance;
    int _servoXstepSize;
    int _servoYstepSize;

};

#endif // GEOMETRY_H
