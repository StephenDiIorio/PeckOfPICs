#include "Simulation.h"
#include "Species.h"
#include "Field.h"

const uint ndump = 1;
const uint Nx = 8;   
const uint Ny = 8;           // number of grid points
const double L_x = 1.0;
const double L_y = 1.0;
const double tmax = 2*M_PI;
const double dt = 0.1;

const uint nspec = 1;


void two_part_test(Species &spec, uint Npar)
{
    double dx = L_x / Nx, dy = L_y / Ny;
    const double W1 = 1.;
    const double W2 = 1.;
    
    double x11 = .35*L_x, x21 = .65 * L_x;
    double x12 = .5 * L_x, x22 = .5 * L_y;

    spec.add_particle(x11,x12,0,0,0,0,W1);
    spec.add_particle(x21,x22,0,0,0,0,W2);
}

void init_e_field(Field &f, uint N_x, uint N_y)
{
    f.f1 = GridObject(N_x, N_y);
    f.f2 = GridObject(N_x, N_y);
    f.f3 = GridObject(N_x, N_y);
}

void init_b_field(Field &f, uint N_x, uint N_y)
{
    f.f1 = GridObject(N_x, N_y);
    f.f2 = GridObject(N_x, N_y);
    f.f3 = GridObject(N_x, N_y);
}

void Simulation::init_simulation()
{

    double Qpar = 1.0; // charge of particle
    const uint npar = 2;
    const double density = 1.0;
    this->add_species(npar, Qpar, density, two_part_test);


    // Initialize fields
    this->add_e_field(init_e_field);
    this->add_b_field(init_b_field);
}
