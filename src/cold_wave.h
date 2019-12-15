#include "Simulation.h"
#include "Species.h"
#include "Field.h"

const uint ndump = 1;
const uint Nx = 64;
const uint Ny = 64;           // number of grid points
const double L_x = 1.0;
const double L_y = 1.0;
const double tmax = 2 * M_PI;
const double dt = 0.1;

double Qpar = 1.0; // charge of particle
const uint ppcx = 4;
const uint ppcy = 4;
const uint Npx = ppcx * Nx;
const uint Npy = ppcy * Ny;
const uint npar = Npx * Npy;

const double kx = 2 * M_PI / L_x;
const double ky = 1 * (2 * M_PI / L_y);

const double ampx = .01;
const double ampy = .01;

const uint nspec = 1;


void cold_wave_test(Species &spec, uint Npar)
{
    // cold wave: x0: regularly spaced, y0: regularly spaced
    // x = x0 + amp * cos(x0), y = y0 + amp * cos(y0)
    // W = total charge / npar = n0 * Lx * Ly * Qpar / npar
    double dx = L_x / Nx, dy = L_y / Ny;
    const double Wpar = L_x * L_y * Qpar / npar;
    for (int ii = 0; ii < Npx; ++ii)
    {
      double xi0 = ii*dx + dx*.37;
      double xi = xi0 + ampx * cos(kx * xi0);
      for (int jj = 0; jj < Npy; ++jj)
      {
        double yj0 = jj * dy+dy*.17;
        double yj = yj0 + ampy * cos(ky * yj0);
        spec.add_particle(xi0, yj, 0, 0, 0, 0, Wpar);
      }
    }
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
    this->add_species(npar, Qpar, cold_wave_test);

    // Initialize fields
    this->add_e_field(init_e_field);
    this->add_b_field(init_b_field);
}
