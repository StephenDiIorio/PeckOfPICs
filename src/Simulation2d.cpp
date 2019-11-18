#include "Simulation2d.h"

/**********************************************************
CONSTRUCTORS/DESTRUCTORS
***********************************************************/
Simulation2d::Simulation2d(uint ndump, uint nx, uint ny, double L_x, double L_y, 
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

    // this->grid.reserve(nx*ny);
    // for (uint i = 0; i < nx; ++i)
    // {
    //     this->grid.push_back(this->dx * double(i));
    // }

    this->dt = dt;//0.99 * this->dx;
    this->tmax = tmax;

    this->spec.reserve(nspec);

    init_simulation2d();

    this->nspec = this->spec.size();

    // Initialize densities and fields after instantiation
    deposit_charge();
    // solve_field();
    this->e_field.solve_field(get_total_density());
}

Simulation2d::~Simulation2d()
{
}
//-----------------------------------------

void Simulation2d::add_species(uint npar, double Qpar, double density, std::function<void(Species &, uint, uint)> init_fcn)
{
    this->spec.push_back(Species(npar, this->Nx, this->Ny, Qpar, density, init_fcn));
}

void Simulation2d::add_e_field(std::function<void(Field2d &, uint, uint)> init_fcn)
{
    this->e_field = Field2d(this->Nx, this->Ny, this->dx, this->dy, init_fcn);
}

void Simulation2d::add_b_field(std::function<void(Field2d &, uint, uint)> init_fcn)
{
    this->b_field = Field2d(this->Nx, this->Ny, this->dx, this->dy, init_fcn);
}

//-----------------------------------------

bool Simulation2d::dump_data()
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

void Simulation2d::iterate()
{
    // Already deposited charge and fields on creation, so can immediately push
    map_field_to_species();
    push_species();
    deposit_charge();
    this->e_field.solve_field(get_total_density());

    ++this->n_iter;
}

GridObject Simulation2d::get_total_density()
{
    // std::vector<double> total_dens = std::vector<double>(this->Nx, this->Ny, 0.0);
    GridObject total_dens = GridObject(this->Nx, this->Ny);

    for (const auto &s : this->spec)
    {
        for (uint i = 0; i < this->Nx; ++i)
        {
            for (uint j = 0; j < this->Ny; ++j)
            {
                total_dens.add_to_grid_data(i, j, s.density_arr.get_grid_data(i,j));
            }
        }
    }

    return total_dens;
}

void Simulation2d::print_spec_density(uint i)
{
    spec.at(i).print_density();
}

/**********************************************************
PRIVATE FUNCTIONS
***********************************************************/
/**********************************************************
void Simulation2d::init_Simulation2d() is defined for each run.
***********************************************************/

void Simulation2d::deposit_charge()
{
    for (auto &s : this->spec)
    {
        s.deposit_charge(this->dx, this->dy, this->L_x, this->L_y, this->Nx, this->Ny);
    }
}

// void Simulation2d::solve_field()
// {
//     // std::vector<double> total_dens_re = get_total_density();
//     // std::vector<double> total_dens_im = std::vector<double>(this->Nx, 0.0);

//     // this->e_field.solve_field(total_dens_re, total_dens_im); //TODO: having this function return a value and change state seems bad, maybe pass err as a parameter to also be changed?
// }

void Simulation2d::map_field_to_species()
{
    for (auto &s : this->spec)
    {
        s.map_field_to_part(this->e_field, this->dx, this->dy, this->L_x, this->L_y,
            this->Nx, this->Ny);
    }
}

void Simulation2d::push_species()
{
    for (auto &s : this->spec)
    {
        // s.push_particles(n_iter, this->L_sys, this->dt, this->dx);
        s.boris_push(n_iter, this->L_x, this->L_y, this->dt, this->dx, this->dy);
    }
}
