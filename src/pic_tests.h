#include "Simulation.h"
#include "Species.h"
#include "Field.h"

// // TEST 1: Single particle should remain stationary and create dipole field
// const std::size_t ndump = 1;
// const std::size_t Nx = 8; // number of grid points
// const std::size_t Ny = 8; // number of grid points

// const std::size_t Npar = 1;
// const double L_x = 1.0;
// const double L_y = 1.0;

// const double tmax = 0.01;
// const double dt = 0.01;

// const std::size_t nspec = 1;
// void sing_part_test(Species& spec, std::size_t Npar)
// {
//     const double dx = double(L_x) / double(Nx);
//     const double dy = double(L_y) / double(Ny);
//     const double Wpar = spec.Qpar * double(dx) * double(dy); // weighting (should be / NPPC)
//     double x_pos, y_pos;
//     for (std::size_t i = 0; i < Npar; ++i)
//     {
//         x_pos = (L_x / 2.0);
//         y_pos = (L_y / 2.0);
//         spec.add_particle(Particle(x_pos, y_pos, 0.0, 0.0, 0.1, 0.0, Wpar));
//     }
// }


// // TEST 2: Two particles should oscillate back and forth
// const std::size_t ndump = 1;
// const std::size_t Nx = 8;
// const std::size_t Ny = 8; // number of grid points
// const double L_x = 1.0;
// const double L_y = 1.0;
// const double tmax = 2*M_PI;
// const double dt = 0.1;

// const std::size_t npar = 2;
// const std::size_t nspec = 1;

// void two_part_test(Species &spec, std::size_t Npar)
// {
//     double dx = L_x / Nx, dy = L_y / Ny;
//     const double W1 = 1. * dx * dy;
//     const double W2 = 1. * dx * dy;

//     double x11 = .35 * L_x, x21 = .65 * L_x;
//     double x12 = .5 * L_x, x22 = .5 * L_y;

//     spec.add_particle(x11,x12,0,0,0,0,W1);
//     spec.add_particle(x21,x22,0,0,0,0,W2);
// }


// TEST 3: Two line charges should oscillate back and forth
const std::size_t ndump = 1;
const std::size_t Nx = 64; // number of grid points
const std::size_t Ny = 64; // number of grid points

const std::size_t Npar = Nx*2;
const double L_x = 1.0;
const double L_y = 1.0;

const double tmax = 3.0;
const double dt = 0.01;

const std::size_t nspec = 1;

void two_line_test(Species &spec, std::size_t Npar)
{
    const double dx = double(L_x) / double(Nx);
    const double dy = double(L_y) / double(Ny);
    const double Wpar = spec.Qpar * double(dx) * double(dy);
    double x_pos, y_pos;
    for (std::size_t i = 0; i < Npar / 2; ++i)
    {
        x_pos = i * dx;
        y_pos = (26. * L_y / 64.0);
        spec.add_particle(Particle(x_pos, y_pos, 0.0, 0.0, 0.0, 0.0, Wpar));
    }
    for (std::size_t i = 0; i < Npar / 2; ++i)
    {
        x_pos = i * dx;
        y_pos = (38. * L_y / 64.0);
        spec.add_particle(Particle(x_pos, y_pos, 0.0, 0.0, 0.0, 0.0, Wpar));
    }
}


void init_e_field(Field &f, std::size_t N_x, std::size_t N_y)
{
    f.f1 = GridObject(N_x, N_y);
    f.f2 = GridObject(N_x, N_y);
    f.f3 = GridObject(N_x, N_y);
}

void init_b_field(Field &f, std::size_t N_x, std::size_t N_y)
{
    f.f1 = GridObject(N_x, N_y);
    f.f2 = GridObject(N_x, N_y);
    f.f3 = GridObject(N_x, N_y);
}

void Simulation::init_simulation()
{
    double Qpar = 1.0; // charge of particle
    this->add_species(Npar, Qpar, two_line_test);

    // Initialize fields
    this->add_e_field(init_e_field);
    this->add_b_field(init_b_field);
}
