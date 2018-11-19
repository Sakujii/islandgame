#include "coordinateconvert.hh"

#include <qmath.h>

namespace Student {

QPointF cubeToAxial(Common::CubeCoordinate cube, int side)
{
    double x = side * (sqrt(3) * cube.x  +  sqrt(3)/2 * cube.z);
    double y = side * (3./2 * cube.z);

    return QPointF(x, y);
}

Common::CubeCoordinate axialToCube(QPointF axial)
{
    int x = axial.x();
    int z = axial.y();
    int y = x - z;

    return Common::CubeCoordinate(x, y, z);
}
}
