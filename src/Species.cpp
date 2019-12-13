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
 * @param npar Total number of particles in the species
 * @param nx Number of grid spaces in x direction
 * @param ny Number of grid spaces in y direction
 * @param Qpar Charge of particle in units of fundamental charge
 */
Species::Species(uint npar, uint nx, uint ny, double Qpar)
{
    this->npar = npar;
    this->parts.reserve(npar);

    this->density = 0.0;
    this->density_arr = GridObject(nx, ny);

    this->Qpar = Qpar;

    this->total_KE = 0.0;
}

/**
 * @brief Constructor for Species object
 *
 * @param npar Total number of particles in the species
 * @param Nx Number of grid spaces in x direction
 * @param Ny Number of grid spaces in y direction
 * @param Qpar Charge of particle in units of fundamental charge
 * @param density
 * @param init_fcn User provided function which initializes the density of the
 *                 species to the user's specification
 */
Species::Species(uint npar, uint Nx, uint Ny, double Qpar, double density,
                 std::function<void(Species &, uint)> init_fcn)
{
    this->npar = npar;
    this->parts.reserve(npar);

    this->density = density;
    this->density_arr = GridObject(Nx, Ny);

    this->Qpar = Qpar;

    this->total_KE = 0.0;

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
void Species::add_particle(ThreeVec pos, ThreeVec mom, double Wpar)
{
    this->add_particle(Particle(pos, mom, Wpar));
}

/**
 * @brief Adds a new particle to the species
 *
 * @param p A particle object to add to the species
 */
void Species::add_particle(Particle p)
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
                            const uint Nx, const uint Ny)
{
    // Initialize
    for (auto &d : this->density_arr)
    {
        d = 0.0;
    }

    double x_pos, y_pos, par_weight;
    double fi, fj, hx, hy;
    double x_min = 0.0, y_min = 0.0;
    std::size_t i, j;

    // uint print_num = 0;
    for (const auto &p : this->parts)
    {
        // std::cout << "Printing info for part " << print_num << std::endl;
        par_weight = p.get_weight() / dx / dy; // add normalization factor here
        x_pos = p.get_pos().get_x();
        y_pos = p.get_pos().get_y();

        // std::cout << "weight " << par_weight << std::endl;
        // std::cout << "x pos " << x_pos << std::endl;
        // std::cout << "y pos " << y_pos << std::endl;

        // This is because I have chosen to start my boundary at -dx/2
        if (x_pos < 0.0)
        {
            x_pos += L_x;
        }
        if (y_pos < 0.0)
        {
            y_pos += L_y;
        }

        fi = (x_pos - x_min) / dx; // shape function normalization here
        i  = fi;
        hx = fi - i;

        fj = (y_pos - y_min) / dy; // shape function normalization here
        j  = fj;
        hy = fj - j;

        // std::cout << "fi=" << fi << std::endl;
        // std::cout << "i=" << i << std::endl;
        // std::cout << "hx=" << hx << std::endl;
        // std::cout << "fj=" << fj << std::endl;
        // std::cout << "j=" << j << std::endl;
        // std::cout << "hy=" << hy << std::endl;
        // std::cout << "i, j gets " << (1.-hx) * (1.-hy) * par_weight << std::endl;
        // std::cout << "i+1, j gets " << (1. - hx) * hy * par_weight << std::endl;
        // std::cout << "i, j+1 gets " << hx * (1. - hy) * par_weight << std::endl;
        // std::cout << "i+1, j+1 gets " << hx * hy * par_weight << std::endl;

        density_arr.comp_add_to(i,   j,   (1.-hx) * (1.-hy) * par_weight);
        density_arr.comp_add_to(i+1, j,   hx      * (1.-hy) * par_weight);
        density_arr.comp_add_to(i,   j+1, (1.-hx) * hy      * par_weight);
        density_arr.comp_add_to(i+1, j+1, hx      * hy      * par_weight);
        // print_num++;
    }
    return 0;
}


/**
 * @brief Interpolates the field values from the grid to the particle position
 *
 * @param f Field to interpolate to particle position
 * @param dx Spatial grid step in x direction
 * @param dy Spatial grid step in y direction
 * @param L_x Physical length of system in x direction
 * @param L_y Physical length of system in y direction
 * @param Nx Number of grid spaces in x direction
 * @param Ny Number of grid spaces in y direction
 * @return int Returns an error code or 0 if successful
 */
int Species::map_field_to_part(const Field& f, field_type field_to_map,
                               const double dx, const double dy,
                               const double L_x, const double L_y,
                               const uint Nx, const uint Ny)
{
    double x_pos, y_pos, par_weight;
    double fi, fj, hx, hy;
    double x_min = 0.0, y_min = 0.0;
    uint i, j;


    for (auto &p : this->parts)
    {
        double loc_f_x1 = 0.0;
        double loc_f_x2 = 0.0;
        double loc_f_x3 = 0.0;

        // par_weight = p.get_weight() / dx / dy; // add normalization factor here
        x_pos = p.get_pos().get_x();
        y_pos = p.get_pos().get_y();

        // This is because I have chosen to start my boundary at -dx/2
        if (x_pos < 0.0)
        {
            x_pos += L_x;
        }
        if (y_pos < 0.0)
        {
            y_pos += L_y;
        }

        fi = (x_pos - x_min) / dx;
        i = fi;
        hx = fi - i;

        fj = (y_pos - y_min) / dy;
        j = fj;
        hy = fj - j;

// added a print statement here for debuggin purposes
        // std::cout << "i " << i << ", j " << j << std::endl;
        // std::cout << "hx " << hx << ", hy " << hy << std::endl;
        // std::cout << "weight " << par_weight << std::endl;
        // std::cout << "Qpar " << this->Qpar << std::endl;
        // std::cout << "Eij " << f.f1.get_comp(i,j) << std::endl;
        // std::cout << "Ei+1,j " << f.f1.get_comp(i+1,j) << std::endl;
        // std::cout << "Ei,j+1 " << f.f1.get_comp(i,j+1) << std::endl;
        // std::cout << "Ei+1,j+1 " << f.f1.get_comp(i+1,j+1) << std::endl;

        loc_f_x1 += (1.-hx) * (1.-hy)  \
                    * f.f1.get_comp(i, j);
        loc_f_x1 += hx * (1.-hy) \
                    * f.f1.get_comp(i+1, j);
        loc_f_x1 += (1.-hx) * hy \
                    * f.f1.get_comp(i, j+1);
        loc_f_x1 += hx      * hy \
                    * f.f1.get_comp(i+1, j+1);

        loc_f_x2 += (1.-hx) * (1.-hy) \
                    * f.f2.get_comp(i, j);
        loc_f_x2 += hx * (1.-hy)  \
                    * f.f2.get_comp(i+1, j);
        loc_f_x2 += (1.-hx) * hy  \
                    * f.f2.get_comp(i, j+1);
        loc_f_x2 += hx      * hy  \
                    * f.f2.get_comp(i+1, j+1);

        loc_f_x3 += (1.-hx) * (1.-hy)  \
                    * f.f3.get_comp(i, j);
        loc_f_x3 += hx * (1.-hy)  \
                    * f.f3.get_comp(i+1, j);
        loc_f_x3 += (1.-hx)   * hy  \
                    * f.f3.get_comp(i, j+1);
        loc_f_x3 += hx      * hy    \
                    * f.f3.get_comp(i+1, j+1);

        // std::cout << "loc_f_x1 " << loc_f_x1 << ", loc_f_x2 " << loc_f_x2 \
        //     << ", loc_f_x3 " << loc_f_x3 << std::endl;

        // p.set_local_e_field(loc_f_x1, loc_f_x2, loc_f_x3);
        if (field_to_map == electric)
        {
            p.set_local_e_field(loc_f_x1, loc_f_x2, loc_f_x3);
        }
        else
        {
            p.set_local_b_field(loc_f_x1, loc_f_x2, loc_f_x3);
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
    double KE = 0.0;
    this->total_KE = 0.0;

    // double B0 = sqrt(3.0);//1.0;
    double b2, mom2, gamma;
    ThreeVec t, s, pos, mom;
    // ThreeVec t = ThreeVec(0.0, 0.0, B0 * dt / 2);
    // ThreeVec s = t * (2 / (1 + t.square()));
    ThreeVec vstar, vperp;

    for (auto &p : this->parts)
    {
        pos = p.get_pos();
        mom = p.get_mom();

        // minus instead of plus b/c of convention used in this program
        mom -= p.get_local_e_field() * (this->Qpar * dt * 0.5);

        mom2 = mom.square();
        gamma = 1. / sqrt(1. + mom2);

        b2 = p.get_local_b_field().square();
        // debug print statement
        // std::cout << "local b" << std::endl;
        // p.print_local_b_field();
        // std::cout << "local e" << std::endl;
        // p.print_local_e_field();
        // end print

        if (b2) // test if non-zero
        {
            t = p.get_local_b_field() * this->Qpar * dt * 0.5;
            s = t * (2. / (1. + t.square()));

            vperp = mom - ((mom.element_multiply(p.get_local_b_field()))
                           / sqrt(b2));
            vstar = vperp + (vperp^t);

            mom += vstar^s;
        }

        // something's wrong with position part of pusher
        // std::cout << "in pusher:" << std::endl;
        // std::cout << "pos " << std::endl;
        // pos.print();
        // std::cout << "halfstep E + B mom " << std::endl;
        // mom.print();
        // std::cout << "local e " << std::endl;
        // p.print_local_e_field();
        // std::cout << "gamma " << gamma << std::endl;
        // // end prints

        mom -= p.get_local_e_field() * (this->Qpar * dt * 0.5);
        // //debugging prints
        // std::cout << "mom after step"<< std::endl;
        // mom.print();
        // //end prints

        pos += mom * (dt / gamma);
        // //debugging prints
        // std::cout << "pos after step" <<  std::endl;
        // pos.print();
        // //end prints

        this->apply_bc(pos, L_x, L_y, dx, dy);
        // // debugging prints
        // std::cout << "pos after mod " << std::endl;
        // pos.print();
        // // end prints

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
 * @brief Returns all of the particles' x positions
 *
 * @return std::vector<double> Vector containing the x positions of all
 *                             particles in species
 */
DataStorage_1D Species::get_x_phasespace()
{
    DataStorage_1D to_ret(this->npar);

    for (int i = 0; i < this->npar; ++i)
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
    DataStorage_1D to_ret(this->npar);

    for (int i = 0; i < this->npar; ++i)
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
    DataStorage_1D to_ret(this->npar);

    for (int i = 0; i < this->npar; ++i)
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
    DataStorage_1D to_ret(this->npar);

    for (int i = 0; i < this->npar; ++i)
    {
        to_ret[i] = this->parts[i].get_mom().get_y();
    }

    return to_ret;
}


std::vector<double> Species::get_local_E(int i)
{
    std::vector<double> to_ret = std::vector<double>(this->npar);

    for (int i = 0; i < this->npar; ++i)
    {
        to_ret[i] = this->parts[i].get_local_e_field().get(i);
    }

    return to_ret;
}

std::vector<double> Species::get_local_E_x()
{
    std::vector<double> to_ret = std::vector<double>(this->npar);

    for (int i = 0; i < this->npar; ++i)
    {
        to_ret[i] = (this->parts[i].get_local_e_field()).get_x();
    }

    return to_ret;
}

std::vector<double> Species::get_local_E_y()
{
    std::vector<double> to_ret = std::vector<double>(this->npar);

    for (int i = 0; i < this->npar; ++i)
    {
        to_ret[i] = (this->parts[i].get_local_e_field()).get_y();
    }

    return to_ret;
}

std::vector<double> Species::get_local_E_z()
{

    std::vector<double> to_ret = std::vector<double>(this->npar);

    for (int i = 0; i < this->npar; ++i)
    {
        to_ret[i] = (this->parts[i].get_local_e_field()).get_z();
    }

    return to_ret;
}

std::vector<double> Species::get_local_B(int i)
{
    std::vector<double> to_ret = std::vector<double>(this->npar);

    for (int i = 0; i < this->npar; ++i)
    {
        to_ret[i] = (this->parts[i].get_local_b_field()).get(i);
    }

    return to_ret;
}
std::vector<double> Species::get_local_B_x()
{

    std::vector<double> to_ret = std::vector<double>(this->npar);

    for (int i = 0; i < this->npar; ++i)
    {
        to_ret[i] = (this->parts[i].get_local_e_field()).get_x();
    }

    return to_ret;
}
std::vector<double> Species::get_local_B_y()
{
    std::vector<double> to_ret = std::vector<double>(this->npar);

    for (int i = 0; i < this->npar; ++i)
    {
        to_ret[i] = (this->parts[i].get_local_e_field()).get_y();
    }

    return to_ret;
}
std::vector<double> Species::get_local_B_z()
{
    std::vector<double> to_ret = std::vector<double>(this->npar);

    for (int i = 0; i < this->npar; ++i)
    {
        to_ret[i] = (this->parts[i].get_local_e_field()).get_z();
    }

    return to_ret;
}
/*
void Species::print_part_v_coord() const
{
    for (auto &p : this->parts)
    {
        p.print_mom();
    }
}
*/
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
void Species::print_pos_comp(uint i) const
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
void Species::print_mom_comp(uint i) const
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
void Species::print_local_e_field_comp(uint i) const
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
void Species::print_local_b_field_comp(uint i) const
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

void Species::init_species(std::function<void(Species &, uint)> init_fcn)
{
    init_fcn(*this, this->npar);
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
void Species::apply_bc(ThreeVec &pos,
                       const double L_x, const double L_y,
                       const double dx, const double dy)
{
    double x1 = pos.get_x();
    double y1 = pos.get_y();

    // Periodic x boundaries
    while (x1 < -dx / 2.0)
    {
        // std::cout << "x pos too small: before=" << x1 << std::endl;
        x1 += L_x;
        // std::cout << "x pos too small: after=" << x1 << std::endl;
        pos.set_x(x1);
    }
    while (x1 >= (L_x - (dx / 2.0)))
    {
        // std::cout << "x pos too large: before=" << x1 << std::endl;
        x1 -= L_x;
        // std::cout << "x pos too large: after=" << x1 << std::endl;
        pos.set_x(x1);
    }

    // Periodic y boundaries
    while (y1 < -dy / 2.0)
    {
        // std::cout << "y pos too small: before=" << y1 << std::endl;
        y1 += L_y;
        // std::cout << "y pos too small: after=" << y1 << std::endl;
        pos.set_y(y1);
    }
    while (y1 >= (L_y - (dy / 2.0)))
    {
        // std::cout << "y pos too large: before=" << y1 << std::endl;
        y1 -= L_y;
        // std::cout << "y pos too large: before=" << y1 << std::endl;
        pos.set_y(y1);
    }
}
//-----------------------------------------
