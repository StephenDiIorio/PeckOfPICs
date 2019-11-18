#include "Simulation.h"

/**********************************************************
CONSTRUCTORS/DESTRUCTORS
***********************************************************/

/**
 * @brief Constructor for Simulation object
 *
 * @param ndump Number of data dumps
 * @param Nx Number of grid spaces in x direction
 * @param Ny Number of grid spaces in y direction
 * @param L_x Physical length of system in x direction
 * @param L_y Physical length of system in y direction
 * @param dt Timestep
 * @param tmax Max simulation runtime
 */
Simulation::Simulation(uint ndump,
                       uint Nx, uint Ny,
                       double L_x, double L_y,
                       double dt, double tmax)
{
    this->err = 0;

    this->n_iter = 0;
    this->ndump = ndump;

    this->Nx = nx;
    this->Ny = ny;
    this->L_x = L_x;
    this->L_y = L_y;
    this->dx = L_x / double(nx);
    this->dy = L_y / double(ny);


    this->dt = dt;//0.99 * this->dx;
    this->tmax = tmax;

    this->spec.reserve(nspec);

    init_simulation();

    this->nspec = this->spec.size();

    // Initialize densities and fields after instantiation
    deposit_charge();
    solve_field();
}

/**
 * @brief Destructor for Simulation object
 *
 */
Simulation::~Simulation()
{
}
//-----------------------------------------


/**********************************************************
CLASS METHODS
***********************************************************/

/**
 * @brief Add a new species object to the simulation
 *
 * @param npar Total number of particles in the species
 * @param Qpar Charge of species
 * @param density Base density of the species
 * @param init_fcn User provided function which initializes the density of the
 *                 species to the user's specification
 */
void Simulation::add_species(uint npar, double Qpar, double density,
                             std::function<void(Species &, uint)> init_fcn)
{
    this->spec.push_back(Species(npar, this->Nx, this->Ny, Qpar, density,
                                 init_fcn));
}

/**
 * @brief Add a new electric field to the simulation
 *
 * @param init_fcn User provided function which initializes the field to the
 *                 user's specification
 */
void Simulation::add_e_field(std::function<void(Field2d &, uint)> init_fcn)
{
    this->e_field = Field(this->Nx, this->Ny, this->dx, this->dy, init_fcn);
}

/**
 * @brief Add a new magnetic field to the simulation
 *
 * @param init_fcn User provided function which initializes the field to the
 *                 user's specification
 */
void Simulation::add_b_field(std::function<void(Field2d &, uint)> init_fcn)
{
    this->b_field = Field(this->Nx, this->Ny, this->dx, this->dy, init_fcn);
}


/**
 * @brief Determine whether or not to dump simulation data
 *
 * @return true True if number of iterations is a multiple of ndump
 * @return false False if number of iterations is not a multiple of ndump
 */
bool Simulation::dump_data()
{
    if (this->ndump > 0)
    {
        return !(this->n_iter % this->ndump);
    }
    else
    {
        return false;
    }
}

/**
 * @brief Completes a single PIC loop iteration
 *
 */
void Simulation::iterate()
{
    // Already deposited charge and fields on creation, so can immediately push
    map_field_to_species();
    push_species();
    deposit_charge();
    solve_field();

    ++(this->n_iter);
}

/**
 * @brief Get the sum of all species' densities in simulation
 *
 * @return GridObject Matrix containing the sum of all species' densities in
 *                    simulation
 */
GridObject Simulation::get_total_density()
{
    GridObject total_dens = GridObject(this->Nx, this->Ny);

    for (const auto &s : this->spec)
    {
        total_dens += s.density_arr;
    }

    return total_dens;
}

/**
 * @brief Prints the density of the 'ith' species in simulation
 *
 * @param i Species number to print the density of
 */
void Simulation::print_spec_density(uint i)
{
    spec.at(i).print_density();
}
//-----------------------------------------


/**********************************************************
PRIVATE CLASS METHODS
***********************************************************/

/**
 * @brief Deposits all species' particle charge onto grid
 *
 */
void Simulation::deposit_charge()
{
    for (auto &s : this->spec)
    {
        s.deposit_charge(this->dx, this->dy, this->L_x, this->L_y, this->Nx,
                         this->Ny);
    }
}

/**
 * @brief Updates the fields in the simulation based on current distribution
 *
 */
void Simulation::solve_field()
{
    // std::vector<double> total_dens_re = get_total_density();
    // std::vector<double> total_dens_im = std::vector<double>(this->Nx, 0.0);

    // this->e_field.solve_field(total_dens_re, total_dens_im); //TODO: having this function return a value and change state seems bad, maybe pass err as a parameter to also be changed?
}

/**
 * @brief Weights the current fields to the particles for all species
 *
 */
void Simulation::map_field_to_species()
{
    for (auto &s : this->spec)
    {
        s.map_field_to_part(this->e_field, this->dx, this->dy, this->L_x,
                            this->L_y, this->Nx, this->Ny);
    }
}

/**
 * @brief Performs the particle push for all species
 *
 */
void Simulation::push_species()
{
    for (auto &s : this->spec)
    {
        s.push_particles(this->L_x, this->L_y, this->dt, this->dx,
                         this->dy);
    }
}
//-----------------------------------------
