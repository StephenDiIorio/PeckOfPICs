#include "ThreeVec.h"

/**********************************************************
CONSTRUCTORS/DESTRUCTORS
***********************************************************/

/**
 * @brief Constructor for ThreeVec object
 *
 */
ThreeVec::ThreeVec() : ThreeVec(0.0, 0.0, 0.0)
{
}

/**
 * @brief Cartesian constructor for ThreeVec object
 *
 * @param x x-coordinate
 * @param y y-coordinate
 * @param z z-coordinate
 */
ThreeVec::ThreeVec(double x, double y, double z)
{
    coord_[X_IDX] = x;
    coord_[Y_IDX] = y;
    coord_[Z_IDX] = z;
}

/**
 * @brief Destructor for ThreeVec object
 *
 */
ThreeVec::~ThreeVec()
{
}
//-----------------------------------------


/**********************************************************
CLASS METHODS
***********************************************************/
/**
 * @brief Access function for x coordinate
 *
 * @return double x coordinate
 */
double ThreeVec::get_x()
{
    return coord_[X_IDX];
}

/**
 * @brief Access function for y coordinate
 *
 * @return double y coordinate
 */
double ThreeVec::get_y()
{
    return coord_[Y_IDX];
}

/**
 * @brief Access function for z coordinate
 *
 * @return double z coordinate
 */
double ThreeVec::get_z()
{
    return coord_[Z_IDX];
}

/**
 * @brief Access function for ith coordinate
 *
 * @param i Index into the vector
 * @return double Value at ith coordinate
 */
double ThreeVec::get(int i)
{
    return coord_[i];
}

/**
 * @brief Modifier method for x coordinate
 *
 * @param value Value to set x coorindate to
 */
void ThreeVec::set_x(double value)
{
    coord_[X_IDX] = value;
}

/**
 * @brief Modifier method for y coordinate
 *
 * @param value Value to set y coorindate to
 */
void ThreeVec::set_y(double value)
{
    coord_[Y_IDX] = value;
}

/**
 * @brief Modifier method for z coordinate
 *
 * @param value Value to set z coorindate to
 */
void ThreeVec::set_z(double value)
{
    coord_[Z_IDX] = value;
}

/**
 * @brief Modifier method for ith coordinate -> INSERT
 *
 * @param i Index into the vector
 * @param value Value to set ith coorindate to
 */
void ThreeVec::set(int i, double value)
{
    coord_[i] = value;
}

/**
 * @brief Modifier method for all coordinate
 *
 * @param x New x coordinate
 * @param y New y coordinate
 * @param z New z coordinate
 */
void ThreeVec::set_all(double x, double y, double z)
{
    coord_[X_IDX] = x;
    coord_[Y_IDX] = y;
    coord_[Z_IDX] = z;
}

/**
 * @brief Alternative modifier method for ith coordinate -> ADD
 *
 * @param i Index into the vector
 * @param value Value to add to the ith element
 */
void ThreeVec::inc(int i, double value)
{
    coord_[i] += value;
}

/**
 * @brief Square the ThreeVec
 *
 * @return double The sum of the squares of all components
 */
double ThreeVec::square()
{
    double answer = 0.0;
    for (int i = 0; i < MAX_DIM; ++i)
    {
        answer += coord_[i] * coord_[i];
    }
    return answer;
}

/**
 * @brief Calculate the magnitude of the ThreeVec
 *
 * @return double Magnitude of the ThreeVec
 */
double ThreeVec::mag()
{
    return sqrt(square());
}

/**
 * @brief Perform element-wise multiplication
 *
 * @param vec Vector to multiply element-wise with
 * @return ThreeVec Resultant ThreeVec
 */
ThreeVec ThreeVec::element_multiply(ThreeVec vec)
{
    ThreeVec ans(coord_[X_IDX] * vec.get_x(),
                 coord_[Y_IDX] * vec.get_y(),
                 coord_[Z_IDX] * vec.get_z());
    return ans;
}
//-----------------------------------------
