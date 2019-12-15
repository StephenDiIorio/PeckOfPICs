#include "Species.h"

/**********************************************************
CONSTRUCTORS/DESTRUCTORS
***********************************************************/

/**
 * @brief Constructor for Species object
 *
 */
Species::Species()
{
}

/**
 * @brief Constructor for Species object - inits density to 0.0 everywhere
 *
 * @param Npar Total number of particles in the species
 * @param Nx Number of grid spaces in x direction
 * @param Ny Number of grid spaces in y direction
 * @param Qpar Charge of particle in units of fundamental charge
 */
Species::Species(std::size_t Npar, std::size_t Nx, std::size_t Ny, double Qpar)
{
    this->Npar = Npar;
    this->parts.reserve(Npar);

    this->density_arr = GridObject(Nx, Ny);

    this->Qpar = Qpar;

    // this->total_KE = 0.0;
}

/**
 * @brief Constructor for Species object
 *
 * @param Npar Total number of particles in the species
 * @param Nx Number of grid spaces in x direction
 * @param Ny Number of grid spaces in y direction
 * @param Qpar Charge of particle in units of fundamental charge
 * @param init_fcn User provided function which initializes the density of the
 *                 species to the user's specification
 */
Species::Species(std::size_t Npar, std::size_t Nx, std::size_t Ny, double Qpar,
                 std::function<void(Species &, std::size_t)> init_fcn)
{
    this->Npar = Npar;
    this->parts.reserve(Npar);

    this->density_arr = GridObject(Nx, Ny);

    this->Qpar = Qpar;

    // this->total_KE = 0.0;

    init_species(init_fcn);
}

/**
 * @brief Destructor for Species object
 *
 */
Species::~Species()
{
}
//-----------------------------------------


/**********************************************************
CLASS METHODS
***********************************************************/

/**
 * @brief Adds a new particle to the species
 *
 * @param x_pos The physical x position of the particle
 * @param y_pos The physical y position of the particle
 * @param z_pos The physical z position of the particle
 * @param x_mom The physical x momentum of the particle
 * @param y_mom The physical y momentum of the particle
 * @param z_mom The physical z momentum of the particle
 * @param Wpar The weight of the particle
 */
void Species::add_particle(double x_pos, double y_pos, double z_pos,
                           double x_mom, double y_mom, double z_mom,
                           double Wpar)
{
    this->add_particle(Particle(x_pos, y_pos, z_pos,
                                x_mom, y_mom, z_mom,
                                Wpar));
}

/**
 * @brief Adds a new particle to the species
 *
 * @param pos A vector containing all position components of the particle
 * @param mom A vector containing all momentum components of the particle
 * @param Wpar The weight of the particle
 */
void Species::add_particle(const ThreeVec& pos, const ThreeVec& mom, double Wpar)
{
    this->add_particle(Particle(pos, mom, Wpar));
}

/**
 * @brief Adds a new particle to the species
 *
 * @param p A particle object to add to the species
 */
void Species::add_particle(const Particle& p)
{
    this->parts.push_back(p);
}
//-----------------------------------------


/**
 * @brief Deposits and interpolates the species charge onto the grid
 *
 * @param dx Spatial grid step in x direction
 * @param dy Spatial grid step in y direction
 * @param L_x Physical length of system in x direction
 * @param L_y Physical length of system in y direction
 * @param Nx Number of grid spaces in x direction
 * @param Ny Number of grid spaces in y direction
 * @return int Returns an error code or 0 if successful
 */
int Species::deposit_charge(const double dx, const double dy,
                            const double L_x, const double L_y,
                            const std::size_t Nx, const std::size_t Ny)
{
    // Initialize
    this->density_arr.zero();

    const double x_min = 0.0, y_min = 0.0;

    for (const auto &p : this->parts)
    {
        double par_weight = p.get_weight() / dx / dy; // normalization factor
        double x_pos = p.get_pos().get_x();
        double y_pos = p.get_pos().get_y();

        // This is because I have chosen to start my boundary at -dx/2
        if (x_pos < 0.0)
        {
            x_pos += L_x;
        }
        if (y_pos < 0.0)
        {
            y_pos += L_y;
        }

        double fi = (x_pos - x_min) / dx; // shape function normalization here
        std::size_t i = fi;
        double hx = fi - double(i);

        double fj = (y_pos - y_min) / dy; // shape function normalization here
        std::size_t j  = fj;
        double hy = fj - double(j);

        density_arr.comp_add_to(i,   j,   (1.-hx) * (1.-hy) * par_weight);
        density_arr.comp_add_to(i+1, j,   hx      * (1.-hy) * par_weight);
        density_arr.comp_add_to(i,   j+1, (1.-hx) * hy      * par_weight);
        density_arr.comp_add_to(i+1, j+1, hx      * hy      * par_weight);
    }
    return 0;
}


/**
 * @brief Interpolates the field values from the grid to the particle position
 *
 * @param f Field to interpolate to particle position
 * @param field_to_map Type of field to map to particles
 * @param dx Spatial grid step in x direction
 * @param dy Spatial grid step in y direction
 * @param L_x Physical length of system in x direction
 * @param L_y Physical length of system in y direction
 * @param Nx Number of grid spaces in x direction
 * @param Ny Number of grid spaces in y direction
 * @return int Returns an error code or 0 if successful
 */
int Species::map_field_to_part(const Field& f,
                               const Field_T::Field_Type field_to_map,
                               const double dx, const double dy,
                               const double L_x, const double L_y,
                               const std::size_t Nx, const std::size_t Ny)
{
    const double x_min = 0.0, y_min = 0.0;

    for (auto &p : this->parts)
    {
        double loc_f_x1 = 0.0;
        double loc_f_x2 = 0.0;
        double loc_f_x3 = 0.0;

        double x_pos = p.get_pos().get_x();
        double y_pos = p.get_pos().get_y();

        // This is because I have chosen to start my boundary at -dx/2
        if (x_pos < 0.0)
        {
            x_pos += L_x;
        }
        if (y_pos < 0.0)
        {
            y_pos += L_y;
        }

        double fi = (x_pos - x_min) / dx; // shape function normalization here
        std::size_t i = fi;
        double hx = fi - double(i);

        double fj = (y_pos - y_min) / dy; // shape function normalization here
        std::size_t j = fj;
        double hy = fj - double(j);

        loc_f_x1 += (1.-hx) * (1.-hy) * f.f1.get_comp(i, j);
        loc_f_x1 += hx      * (1.-hy) * f.f1.get_comp(i+1, j);
        loc_f_x1 += (1.-hx) * hy      * f.f1.get_comp(i, j+1);
        loc_f_x1 += hx      * hy      * f.f1.get_comp(i+1, j+1);

        loc_f_x2 += (1.-hx) * (1.-hy) * f.f2.get_comp(i, j);
        loc_f_x2 += hx      * (1.-hy) * f.f2.get_comp(i+1, j);
        loc_f_x2 += (1.-hx) * hy      * f.f2.get_comp(i, j+1);
        loc_f_x2 += hx      * hy      * f.f2.get_comp(i+1, j+1);

        loc_f_x3 += (1.-hx) * (1.-hy) * f.f3.get_comp(i, j);
        loc_f_x3 += hx      * (1.-hy) * f.f3.get_comp(i+1, j);
        loc_f_x3 += (1.-hx) * hy      * f.f3.get_comp(i, j+1);
        loc_f_x3 += hx      * hy      * f.f3.get_comp(i+1, j+1);

        switch (field_to_map)
        {
            case Field_T::Electric:
                p.set_local_e_field(loc_f_x1, loc_f_x2, loc_f_x3);
                break;
            case Field_T::Magnetic:
                p.set_local_b_field(loc_f_x1, loc_f_x2, loc_f_x3);
                break;
            default:
                throw std::runtime_error(Field_T::Field_T_err);
                break;
        }
    }

    return 0;
}


/**
 * @brief Performs a Boris push on all of the particles in the species
 *
 * @param L_x Physical length of system in x direction
 * @param L_y Physical length of system in y direction
 * @param dt Timestep
 * @param dx Spatial grid step in x direction
 * @param dy Spatial grid step in y direction
 * @return int Returns an error code or 0 if successful
 */
int Species::push_particles(const double L_x, const double L_y,
                            const double dt,
                            const double dx, const double dy)
{
    // For total kinetic energy diagnostic
    // double KE = 0.0;
    // this->total_KE = 0.0;

    for (auto &p : this->parts)
    {
        ThreeVec pos = p.get_pos();
        ThreeVec mom = p.get_mom();

        mom += p.get_local_e_field() * (this->Qpar * dt * 0.5);

        double mom2 = mom.square();
        double gamma = 1. / sqrt(1. + mom2);

        double b2 = p.get_local_b_field().square();

        if (b2) // test if non-zero
        {
            ThreeVec t = p.get_local_b_field() * this->Qpar * dt * 0.5;
            ThreeVec s = t * (2. / (1. + t.square()));

            ThreeVec vperp = mom - ((mom.element_multiply(p.get_local_b_field())) / sqrt(b2));
            ThreeVec vstar = vperp + (vperp^t);

            mom += vstar^s;
        }

        mom += p.get_local_e_field() * (this->Qpar * dt * 0.5);

        pos += mom * (dt / gamma);

        this->_apply_bc(pos, L_x, L_y, dx, dy);

        p.set_mom(mom);
        p.set_pos(pos);

        // For total kinetic energy diagnostic
        // KE *= mom.mag();
        // this->total_KE += KE;
    }

    // For total kinetic energy diagnostic
    // this->total_KE *= (L_sys / (2.0 * double(this->npar)));

    return 0;
}

/**
 * @brief Applies the boundary condition for every particle in the species
 *
 * @param L_x Physical length of system in x direction
 * @param L_y Physical length of system in y direction
 * @param dx Spatial grid step in x direction
 * @param dy Spatial grid step in y direction
 */
void Species::apply_bc(const double L_x, const double L_y,
                       const double dx, const double dy)
{
    for (auto &p : this->parts)
    {
        ThreeVec pos = p.get_pos();
        this->_apply_bc(pos, L_x, L_y, dx, dy);
        p.set_pos(pos);
    }
}

/**
 * @brief Returns all of the particles' x positions
 *
 * @return std::vector<double> Vector containing the x positions of all
 *                             particles in species
 */
DataStorage_1D Species::get_x_phasespace()
{
    DataStorage_1D to_ret(this->Npar);

    for (std::size_t i = 0; i < this->Npar; ++i)
    {
        to_ret[i] = this->parts[i].get_pos().get_x();
    }

    return to_ret;
}

/**
 * @brief Returns all of the particles' y positions
 *
 * @return std::vector<double> Vector containing the y positions of all
 *                             particles in species
 */
DataStorage_1D Species::get_y_phasespace()
{
    DataStorage_1D to_ret(this->Npar);

    for (std::size_t i = 0; i < this->Npar; ++i)
    {
        to_ret[i] = this->parts[i].get_pos().get_y();
    }

    return to_ret;
}

/**
 * @brief Returns all of the particles' x momenta
 *
 * @return std::vector<double> Vector containing the x momenta of all particles
 *                             in species
 */
DataStorage_1D Species::get_px_phasespace()
{
    DataStorage_1D to_ret(this->Npar);

    for (std::size_t i = 0; i < this->Npar; ++i)
    {
        to_ret[i] = this->parts[i].get_mom().get_x();
    }

    return to_ret;
}

/**
 * @brief Returns all of the particles' y momenta
 *
 * @return std::vector<double> Vector containing the y momenta of all particles
 *                             in species
 */
DataStorage_1D Species::get_py_phasespace()
{
    DataStorage_1D to_ret(this->Npar);

    for (std::size_t i = 0; i < this->Npar; ++i)
    {
        to_ret[i] = this->parts[i].get_mom().get_y();
    }

    return to_ret;
}


std::vector<double> Species::get_local_E(std::size_t i)
{
    std::vector<double> to_ret = std::vector<double>(this->Npar);

    for (std::size_t i = 0; i < this->Npar; ++i)
    {
        to_ret[i] = this->parts[i].get_local_e_field().get(i);
    }

    return to_ret;
}

std::vector<double> Species::get_local_B(std::size_t i)
{
    std::vector<double> to_ret = std::vector<double>(this->Npar);

    for (int i = 0; i < this->Npar; ++i)
    {
        to_ret[i] = (this->parts[i].get_local_b_field()).get(i);
    }

    return to_ret;
}

/**
 * @brief Prints the positions of all the particles in the species
 *
 */
void Species::print_pos() const
{
    for (auto &p : this->parts)
    {
        p.print_pos();
    }
}

/**
 * @brief Prints the component of the position of all the particles in the
 *        species
 *
 * @param i The index of the component to print
 */
void Species::print_pos_comp(std::size_t i) const
{
    for (auto &p : this->parts)
    {
        p.print_pos_comp(i);
    }
}

/**
 * @brief Prints the momentum of all the particles in the species
 *
 */
void Species::print_mom() const
{
    for (auto &p : this->parts)
    {
        p.print_mom();
    }
}

/**
 * @brief Prints the component of the momentum of all the particles in the
 *        species
 *
 * @param i The index of the component to print
 */
void Species::print_mom_comp(std::size_t i) const
{
    for (auto &p : this->parts)
    {
        p.print_mom_comp(i);
    }
}

/**
 * @brief Prints the weights of all the particles in the species
 *
 */
void Species::print_weight() const
{
    for (auto &p : this->parts)
    {
        p.print_weight();
    }
}

/**
 * @brief Prints the local electric field for all the particles in the species
 *
 */
void Species::print_local_e_field() const
{
    for (auto &p : this->parts)
    {
        p.print_local_e_field();
    }
}

/**
 * @brief Prints the component of the local electric field for all the
 *        particles in the species
 *
 * @param i The index of the component to print
 */
void Species::print_local_e_field_comp(std::size_t i) const
{
    for (auto &p : this->parts)
    {
        p.print_local_e_field_comp(i);
    }
}

/**
 * @brief Prints the local magnetic field for all the particles in the species
 *
 */
void Species::print_local_b_field() const
{
    for (auto &p : this->parts)
    {
        p.print_local_b_field();
    }
}

/**
 * @brief Prints the component of the local magnetic field for all the
 *        particles in the species
 *
 * @param i The index of the component to print
 */
void Species::print_local_b_field_comp(std::size_t i) const
{
    for (auto &p : this->parts)
    {
        p.print_local_b_field_comp(i);
    }
}

/**
 * @brief Prints the density distribution for this species
 *
 */
void Species::print_density() const
{
    this->density_arr.print();
}
//-----------------------------------------


/**********************************************************
PRIVATE CLASS METHODS
***********************************************************/

void Species::init_species(std::function<void(Species &, std::size_t)> init_fcn)
{
    init_fcn(*this, this->Npar);
}

/**
 * @brief Currently applies periodic boundary conditions in x and y directions
 *        for the species
 *
 * @param pos A vector containing all position components of the particle
 * @param L_x Physical length of system in x direction
 * @param L_y Physical length of system in y direction
 * @param dx Spatial grid step in x direction
 * @param dy Spatial grid step in y direction
 */
void Species::_apply_bc(ThreeVec& pos,
                       const double L_x, const double L_y,
                       const double dx, const double dy)
{
    double x1 = pos.get_x();
    double y1 = pos.get_y();

    // Periodic x boundaries
    while (x1 < -dx / 2.0)
    {
        x1 += L_x;
        pos.set_x(x1);
    }
    while (x1 >= (L_x - (dx / 2.0)))
    {
        x1 -= L_x;
        pos.set_x(x1);
    }

    // Periodic y boundaries
    while (y1 < -dy / 2.0)
    {
        y1 += L_y;
        pos.set_y(y1);
    }
    while (y1 >= (L_y - (dy / 2.0)))
    {
        y1 -= L_y;
        pos.set_y(y1);
    }
}
//-----------------------------------------
