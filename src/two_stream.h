#include "Simulation.h"
#include "Species.h"
#include "Field.h"


// two stream functions
const uint ndump = 1;
const uint Nx = 64;
const uint Ny = 64; // number of grid points

const uint Npar = 4 * Nx * Ny;
const double L_x = 8.0 * M_PI * sqrt(3.0); // system length
const double L_y = 8.0 * M_PI * sqrt(3.0);
const double tmax = 10.0 * M_PI;
const double dt = 0.025;

const uint nspec = 2;

void two_stream_plus(Species& spec, uint Npar)
{
    const double dx = double(L_x) / double(Nx);
    const double dy = double(L_y) / double(Ny);
    const double Wpar = spec.Qpar * double(dx) * double(dy);
    double x_pos, x_pert, x_tot, x_mom, y_pos;
    double pert = 0.001;
    double k = 1.0 / (2.0 * sqrt(3.0));
    double _dx = L_x / double(2*Nx);
    double _dy = L_y / double(2*Ny);

    for (uint i = 0; i < 2 * Nx; ++i)
    {
        x_pos = double(i) * _dx;
        x_pert = (pert / (spec.Qpar * k)) * sin(k * x_pos);
        x_tot = x_pos + x_pert + 0.037;
        for (uint j = 0; j < 2 * Ny; ++j)
        {
            y_pos = double(j) * _dy;
            x_mom = 3.0;
            spec.add_particle(x_tot, y_pos, 0.0, x_mom, 0.0, 0.0, Wpar);
        }
    }

    spec.apply_bc(L_x, L_y, dx, dy);
}

void two_stream_minus(Species &spec, uint Npar)
{
    const double dx = double(L_x) / double(Nx);
    const double dy = double(L_y) / double(Ny);
    const double Wpar = spec.Qpar * double(dx) * double(dy);
    double x_pos, x_pert, x_tot, x_mom, y_pos;
    double pert = 0.001;
    double k = 1.0 / (2.0 * sqrt(3.0));
    double _dx = L_x / double(2 * Nx);
    double _dy = L_y / double(2 * Ny);

    for (uint i = 0; i < 2 * Nx; ++i)
    {
        x_pos = double(i) * _dx;
        x_pert = (pert / (spec.Qpar * k)) * sin(k * x_pos);
        x_tot = x_pos + x_pert + 0.037;
        for (uint j = 0; j < 2 * Ny; ++j)
        {
            y_pos = double(j) * _dy;
            x_mom = -3.0;
            spec.add_particle(x_tot, y_pos, 0.0, x_mom, 0.0, 0.0, Wpar);
        }
    }

    spec.apply_bc(L_x, L_y, dx, dy);
}


void init_e_field(Field& f, uint Nx, uint Ny)
{
    f.f1 = GridObject(Nx, Ny, 0.0);
    f.f2 = GridObject(Nx, Ny, 0.0);
    f.f3 = GridObject(Nx, Ny, 0.0);
}

void init_b_field(Field& f, uint Nx, uint Ny)
{
    f.f1 = GridObject(Nx, Ny, 0.0);
    f.f2 = GridObject(Nx, Ny, 0.0);
    f.f3 = GridObject(Nx, Ny, 0.0);
}

void Simulation::init_simulation()
{
    // Attributes for species 1
    const double Qpar1 = 1.0; // charge of particle
    this->add_species(Npar, Qpar1, two_stream_plus);

    // Attributes for species 2
    const double Qpar2 = 1.0; // charge of particle
    this->add_species(Npar, Qpar2, two_stream_minus);

    // Initialize fields
    this->add_e_field(init_e_field);
    this->add_b_field(init_b_field);
}
