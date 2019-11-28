#include "../src/Species.h"
#include "../src/Field.h"
// testing the map fields to particles functionality

// constant field is same everywhere

// point field and point particles

int main(int argc, char **argv)
{
    int Nx = 8, Ny = 8;
    double L_x = 2.0, L_y = 3.0;
    double dx = L_x / Nx, dy = L_y / Ny;
    Species myspec(1, Nx, Ny, -1);
    Field testE(Nx, Ny, dx, dy);
    testE.f1.set_grid_data(0,0,2.);
    testE.print_field();

    myspec.add_particle(0 ,0 ,0,1.,2.,4.,1.);
    std::cout << "weights:\n";
    myspec.print_weights();
    std::cout << "part pos\n";
    myspec.print_part_coord(1);
    myspec.map_field_to_part(testE,dx,dy,L_x,L_y,Nx,Ny);
    myspec.print_E_x();

    
    return 0;
}