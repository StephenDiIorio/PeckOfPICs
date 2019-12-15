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
Simulation::Simulation(std::size_t ndump,
                       std::size_t Nx, std::size_t Ny,
                       double L_x, double L_y,
                       double dt, double tmax)
{
    std::cout << "in sim" << std::endl;
    this->err = 0;

    this->n_iter = 0;
    this->ndump = ndump;

    this->Nx = Nx;
    this->Ny = Ny;
    this->L_x = L_x;
    this->L_y = L_y;
    this->dx = L_x / double(Nx);
    this->dy = L_y / double(Ny);


    this->dt = dt;
    this->tmax = tmax;

    std::cout << "before res" << std::endl;
    std::cout << nspec << std::endl;
    // nspec = 1;
    std::cout << nspec << std::endl;
    this->spec.reserve(nspec);
    std::cout << "after res" << std::endl;

    std::cout << "before init" << std::endl;
    init_simulation();
    std::cout << "after init" << std::endl;

    this->nspec = this->spec.size();

    // Initialize densities and fields after instantiation
    _deposit_charge();
    _solve_field();
    std::cout << "finished init" << std::endl;
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
 * @param init_fcn User provided function which initializes the density of the
 *                 species to the user's specification
 */
void Simulation::add_species(std::size_t npar, double Qpar,
                             std::function<void(Species &, std::size_t)> init_fcn)
{
    this->spec.push_back(Species(npar, this->Nx, this->Ny, Qpar,
                                 init_fcn));
}

/**
 * @brief Add a new electric field to the simulation
 *
 * @param init_fcn User provided function which initializes the field to the
 *                 user's specification
 */
void Simulation::add_e_field(std::function<void(Field &, std::size_t, std::size_t)> init_fcn)
{
    this->e_field = Field(this->Nx, this->Ny, this->dx, this->dy, init_fcn);
}

/**
 * @brief Add a new magnetic field to the simulation
 *
 * @param init_fcn User provided function which initializes the field to the
 *                 user's specification
 */
void Simulation::add_b_field(std::function<void(Field &, std::size_t, std::size_t)> init_fcn)
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
    _map_field_to_species();
    std::cout << "finished map" << std::endl;
    _push_species();
    std::cout << "finished push" << std::endl;
    _deposit_charge();
    std::cout << "finished depo" << std::endl;
    _solve_field();
    std::cout << "finished solve" << std::endl;

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
void Simulation::print_spec_density(std::size_t i) const
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
void Simulation::_deposit_charge()
{
    for (auto &s : this->spec)
    {
        s.deposit_charge(this->dx, this->dy,
                         this->L_x, this->L_y,
                         this->Nx, this->Ny);
    }
}

/**
 * @brief Updates the fields in the simulation based on current distribution
 *
 */
void Simulation::_solve_field()
{
    const GridObject total_dens = get_total_density();

    this->err = this->e_field.solve_field(total_dens, this->dx, this->dy);
}

/**
 * @brief Weights the current fields to the particles for all species
 *
 */
void Simulation::_map_field_to_species()
{
    for (auto &s : this->spec)
    {
        s.map_field_to_part(this->e_field, Electric,
                            this->dx,  this->dy,
                            this->L_x, this->L_y,
                            this->Nx,  this->Ny);
    }
}

/**
 * @brief Performs the particle push for all species
 *
 */
void Simulation::_push_species()
{
    for (auto &s : this->spec)
    {
        s.push_particles(this->L_x, this->L_y,
                         this->dt,
                         this->dx, this->dy);
    }
}
//-----------------------------------------
