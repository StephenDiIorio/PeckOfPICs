#include "Particle.h"

/**********************************************************
CONSTRUCTORS/DESTRUCTORS
***********************************************************/

/**
 * @brief Constructor for Particle object - sets all pos and mom values to 0
 *        and weight to 1
 *
 */
Particle::Particle() : Particle(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0)
{
}

/**
 * @brief Constructor for Particle object - sets all pos and mom values to 0
 *        and weight to the provided value
 *
 * @param weight The weight to assign the particle
 */
Particle::Particle(double weight) : Particle(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, weight)
{
}

/**
 * @brief Constructor for Particle object - sets pos and mom values to provided
 *        quantities and weight to 1
 *
 * @param pos_x Position x coordinate
 * @param pos_y Position y coordinate
 * @param pos_z Position z coordinate
 * @param mom_x Momentum x coordinate
 * @param mom_y Momentum y coordinate
 * @param mom_z Momentum z coordinate
 */
Particle::Particle(double pos_x, double pos_y, double pos_z, double mom_x, double mom_y, double mom_z) : Particle(pos_x, pos_y, pos_z, mom_x, mom_y, mom_z, 1.0)
{
}

/**
 * @brief Constructor for Particle object - sets all values to provided
 *        quantities
 *
 * @param pos_x Position x coordinate
 * @param pos_y Position y coordinate
 * @param pos_z Position z coordinate
 * @param mom_x Momentum x coordinate
 * @param mom_y Momentum y coordinate
 * @param mom_z Momentum z coordinate
 * @param weight The weight to assign the particle
 */
Particle::Particle(double pos_x, double pos_y, double pos_z, double mom_x, double mom_y, double mom_z, double weight)
{
    this->pos = ThreeVec(pos_x, pos_y, pos_z);
    this->mom = ThreeVec(mom_x, mom_y, mom_z);
    this->weight = weight;
    this->local_e_field = ThreeVec();
    this->local_b_field = ThreeVec();
}

/**
 * @brief Constructor for Particle object - copies pos and mom values from
 *        provided vectors and sets weight to 1
 *
 * @param pos ThreeVec with position values to copy
 * @param mom ThreeVec with momentum values to copy
 */
Particle::Particle(ThreeVec pos, ThreeVec mom) : Particle(pos.get_x(), pos.get_y(), pos.get_z(), mom.get_x(), mom.get_y(), mom.get_z(), 1.0)
{
}

/**
 * @brief Constructor for Particle object - copies pos and mom values from
 *        provided vectors and sets weight to provided value
 *
 * @param pos ThreeVec with position values to copy
 * @param mom ThreeVec with momentum values to copy
 * @param weight The weight to assign the particle
 */
Particle::Particle(ThreeVec pos, ThreeVec mom, double weight) : Particle(pos.get_x(), pos.get_y(), pos.get_z(), mom.get_x(), mom.get_y(), mom.get_z(), weight)
{
}

/**
 * @brief Destructor for Particle object
 *
 */
Particle::~Particle()
{
}
//-----------------------------------------


/**********************************************************
CLASS METHODS
***********************************************************/

// Print Functions
/**
 * @brief Print the position of the particle
 *
 */
void Particle::print_pos() const
{
    this->get_pos().print();
}

/**
 * @brief Print a coordinate of the potision of the particle
 *
 * @param i The index to the coordinate to print
 */
void Particle::print_pos_comp(uint i) const
{
    this->get_pos().print_comp(i);
}

/**
 * @brief Print the momentum of the particle
 *
 */
void Particle::print_mom() const
{
    this->get_mom().print();
}

/**
 * @brief Print a coordinate of the momentum of the particle
 *
 * @param i The index to the coordinate to print
 */
void Particle::print_mom_comp(uint i) const
{
    this->get_mom().print_comp(i);
}

/**
 * @brief Print the weight of the particle
 *
 */
void Particle::print_weight() const
{
    std::cout << this->get_weight() << std::endl;
}

/**
 * @brief Print the local electric field the particle sees
 *
 */
void Particle::print_local_e_field() const
{
    this->get_local_e_field().print();
}

/**
 * @brief Print a coordinate of the local electric field the particle sees
 *
 * @param i The index to the coordinate to print
 */
void Particle::print_local_e_field_comp(uint i) const
{
    this->get_local_e_field().print_comp(i);
}

/**
 * @brief Print the local magnetic field the partilce sees
 *
 */
void Particle::print_local_b_field() const
{
    this->get_local_b_field().print();
}

/**
 * @brief Print a coordinate of the local magnetic field the particle sees
 *
 * @param i The index to the coordinate to print
 */
void Particle::print_local_b_field_comp(uint i) const
{
    this->get_local_b_field().print_comp(i);
}
//-----------------------------------------
