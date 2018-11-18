#ifndef COORDINATECONVERT_HH
#define COORDINATECONVERT_HH

#include "cubecoordinate.hh"

#include <QPointF>

namespace Student{

QPointF cubeToAxial(Common::CubeCoordinate, int side);
Common::CubeCoordinate axialToCube(QPointF);

}

#endif // COORDINATECONVERT_HH
