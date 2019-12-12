#include "Simulation.h"
#include "Species.h"
#include "Field.h"

// const uint ndump = 1;
// const uint Nx = 64;           // number of grid points
// const uint Ny = 64;           // number of grid points
// // const double L_x = 8.0 * M_PI * sqrt(3.0);//2.0 * M_PI; // system length
// // const double L_y = 8.0 * M_PI * sqrt(3.0); //2.0 * M_PI; // system length
// // const double tmax = 10.0 * M_PI;
// // const double dt = 0.025;

// // const uint nspec = 1;

// const uint Npar = 1;
// const double L_x = 1.0;
// const double L_y = 1.0;

// const double tmax = 20.0;
// const double dt = 0.01;

// const uint nspec = 1;

// Gaussian random distribution function
double gaussian()
{
    double gaussian_width = 2.0; // +/-2 sigma range
    double x, y;

    do
    {
        x = (2.0 * rand() / RAND_MAX - 1.0) * gaussian_width;
        y = exp(-x * x * 0.5);
    } while (1.0 * rand() / RAND_MAX > y);

    return x;
}

// void test_dens_efield(Species &spec, uint Npar)
// {
//     const double Wpar = spec.Qpar / double(spec.ppc); // weighting (should be / NPPC)
//     double x_pos, x_pert;
//     double pert = 0.01;
//     double k = 2.0 * M_PI / L_sys;
//     for (uint i = 0; i < Npar; ++i)
//     {
//         x_pos = (L_sys / double(Npar) * double(i)); // + 0.037; //TODO: do I need to account for Ksiser-Wilhelm effect here? pg 91 B-L. Dependent on ppc and Nx
//         x_pert = pert / (spec.density * k) * cos(k * x_pos);
//         spec.parts.push_back(Particle(x_pos + x_pert, 0.0, 0.0, 0.0, 0.0, 0.0, Wpar));
//     }
// }

// single particle should remain stationary and create dipole field
// const uint ndump = 1;
// const uint Nx = 8; // number of grid points
// const uint Ny = 8; // number of grid points

// const uint Npar = 1;
// const double L_x = 1.0;
// const double L_y = 1.0;

// const double tmax = 0.01;
// const double dt = 0.01;

// const uint nspec = 1;
// void test_pusher_1(Species& spec, uint Npar)
// {
//     const double dx = double(L_x) / double(Nx);
//     const double dy = double(L_y) / double(Ny);
//     const double Wpar = spec.Qpar * double(dx) * double(dy); // weighting (should be / NPPC)
//     double x_pos, y_pos;
//     for (uint i = 0; i < Npar; ++i)
//     {
//         x_pos = (L_x / 2.0);
//         y_pos = (L_y / 2.0);
//         spec.add_particle(Particle(x_pos, y_pos, 0.0, 0.0, 0.1, 0.0, Wpar));
//     }
// }

// two particles should dance with each other
// const uint ndump = 1;
// const uint Nx = 16; // number of grid points
// const uint Ny = 16; // number of grid points

// const uint Npar = 2;
// const double L_x = 1.0;
// const double L_y = 1.0;

// const double tmax = 0.1;
// const double dt = 0.01;

// const uint nspec = 1;
// void test_pusher_2(Species &spec, uint Npar)
// {
//     const double dx = double(L_x) / double(Nx);
//     const double dy = double(L_y) / double(Ny);
//     const double Wpar = spec.Qpar * double(dx) * double(dy);
//     double x_pos, y_pos;
//     // for (uint i = 0; i < Npar; ++i)
//     // {
//     //     x_pos = ((i+1) * L_x / 4.0);
//     //     y_pos = L_y / 2.0;
//     //     spec.add_particle(Particle(x_pos, y_pos, 0.0, 0.0, 0.0, 0.0, Wpar));
//     // }
//     spec.add_particle(Particle(7/16. + 0.05, L_y / 2.0, 0.0, 0.0, 0.0, 0.0, Wpar));
//     spec.add_particle(Particle(9/16. + 0.05, L_y / 2.0, 0.0, 0.0, 0.0, 0.0, Wpar));
// }

// two sheets of charge should just oscillate back and forth
const uint ndump = 1;
const uint Nx = 64; // number of grid points
const uint Ny = 64; // number of grid points

const uint Npar = Nx * 2;
const double L_x = 1.0;
const double L_y = 1.0;

const double tmax = 1.0;
const double dt = 0.01;

const uint nspec = 1;
void test_pusher_3(Species &spec, uint Npar)
{
    const double dx = double(L_x) / double(Nx);
    const double dy = double(L_y) / double(Ny);
    const double Wpar = spec.Qpar * double(dx) * double(dy); // weighting (should be / NPPC)
    double x_pos, y_pos;
    for (uint i = 0; i < Npar / 2; ++i)
    {
        x_pos = i * dx;
        y_pos = (26. * L_y / 64.0);
        spec.add_particle(Particle(x_pos, y_pos, 0.0, 0.0, 0.0, 0.0, Wpar));
    }
    for (uint i = 0; i < Npar / 2; ++i)
    {
        x_pos = i * dx;
        y_pos = (38. * L_y / 64.0);
        spec.add_particle(Particle(x_pos, y_pos, 0.0, 0.0, 0.0, 0.0, Wpar));
    }
}

// void two_stream_plus(Species& spec, uint Npar)
// {
//     const double dx = double(L_x) / double(Nx);
//     const double dy = double(L_y) / double(Ny);
//     const double Wpar = spec.Qpar * dx * dy;  // weighting (should be / NPPC)
//     double x_pos, x_pert, x_tot, x_mom;
//     double pert = 0.001;
//     double k = 1.0 / (2.0 * sqrt(3.0)); //2.0 * 2.0 * M_PI / L_sys;
//     double _dx = L_x / double(Npar);

//     for (uint i = 0; i < Npar; ++i)
//     {
//         // x_pos = ((L_x / double(Npar)) * double(i));
//         // // x_pert = (pert / (spec.Qpar * k)) * sin(k * x_pos);
//         // x_tot = x_pos;// + x_pert + 0.01; //TODO: do I need to account for Kaiser-Wilhelm effect here? pg 91 B-L. Dependent on ppc and Nx
//         // if (x_tot < -_dx / 2.0)
//         // {
//         //     x_tot += L_x;
//         // }
//         // else if (x_tot >= (L_x - (_dx / 2.0)))
//         // {
//         //     x_tot -= L_x;
//         // }

//         x_mom = 3.0;

//         spec.add_particle(L_x / 2., L_y / 2., 0.0, x_mom, 0.0, 0.0, Wpar);
//     }
//     //TODO: call species BC here?
// }

// void two_stream_minus(Species &spec, uint Npar)
// {
//     const double Wpar = spec.Qpar / (double(Npar) / double(Nx)); //double(spec.ppc); // weighting (should be / NPPC)
//     double x_pos, x_pert, x_tot, x_mom;
//     double pert = 0.001;
//     double k = 1.0 / (2.0 * sqrt(3.0)); //2.0 * 2.0 * M_PI / L_sys;
//     double dx = L_sys / double(Npar);

//     for (uint i = 0; i < Npar; ++i)
//     {
//         x_pos = ((L_sys / double(Npar)) * double(i));
//         x_pert = (pert / (spec.Qpar * k)) * sin(k * x_pos);
//         x_tot = x_pos + x_pert + 0.01; //TODO: do I need to account for Kaiser-Wilhelm effect here? pg 91 B-L. Dependent on ppc and Nx
//         if (x_tot < -dx / 2.0)
//         {
//             x_tot += L_sys;
//         }
//         else if (x_tot >= (L_sys - (dx / 2.0)))
//         {
//             x_tot -= L_sys;
//         }

//         x_mom = -3.0;

//         spec.add_particle(x_tot, 0.0, 0.0, x_mom, 0.0, 0.0, Wpar);
//     }
//     //TODO: call species BC here?
// }

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
    // Attributes for species
    // const double Qpar = 2.0;   // charge of particle
    // const uint ppc = 16;
    // const uint npar = ppc * Nx;
    // const double density = 2.0;
    // this->spec.push_back(Species(npar, this->Nx, Qpar, density, two_stream));

    // Attributes for species 1
    // const double Qpar1 = 1.0; // charge of particle
    // const uint ppc1 = 64;
    // const uint npar1 = ppc1 * Nx;
    // const double density1 = 1.0;
    // std::cout << "about to add species" << std::endl;
    // this->add_species(npar1, Qpar1, density1, two_stream_plus);
    // std::cout << "done adding species" << std::endl;

    // // Attributes for species 2
    // // const double Qpar2 = 1.0; // charge of particle
    // // const uint ppc2 = 64;
    // // const uint npar2 = ppc2 * Nx;
    // // const double density2 = 1.0;
    // // this->add_species(npar2, Qpar2, density2, two_stream_minus);

    // // Initialize fields
    // std::cout << "add fields" << std::endl;
    // this->add_e_field(init_e_field);
    // this->add_b_field(init_b_field);
    // std::cout << "done adding fields" << std::endl;

    const double Qpar = -10.0; // charge of particle
    const uint ppc = 1;
    const double density = Qpar;
    std::cout << "In sim setup" << std::endl;
    std::cout << Qpar << std::endl;
    std::cout << density << std::endl;
    std::cout << Npar << std::endl;
    this->add_species(Npar, Qpar, density, test_pusher_3);
    this->add_e_field(init_e_field);
    this->add_b_field(init_b_field);
}
