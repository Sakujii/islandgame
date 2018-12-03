#ifndef COORDINATECONVERT_HH
#define COORDINATECONVERT_HH

#include "cubecoordinate.hh"

#include <QPointF>

/**
 * @file
 * @brief Offers methods for coordinate changes
 */

namespace Student{

/**
 * @brief Changes cube coordinates to axial coordinates
 * @param cube Coordinates as common CubeCoordinate object
 * @param side Size of the object we are working with
 * @return returns the axial coordinate point as QPointF
 * @post Exception guarantee: strong
 */
QPointF cubeToAxial(Common::CubeCoordinate cube, int side);

/**
 * @brief Changes axial coordinates to cube coordinates
 * @param axial Coordinates as QPointF object
 * @return returns the coordinates as common CubeCoordinate
 * @post Exception guarantee: strong
 */
Common::CubeCoordinate axialToCube(QPointF axial);

}

#endif // COORDINATECONVERT_HH
