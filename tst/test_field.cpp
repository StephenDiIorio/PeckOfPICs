#include "../src/Field.h"

//test that zero densith gives zero phi, E, energy


int zero_density_zero_field(int Nx, int Ny, double L_x, double L_y)
{
  GridObject density(Nx, Ny);

  double dx = L_x / Nx, dy = L_y / Ny;
  Field myfield(Nx, Ny, dx, dy);
  myfield.solve_field(density);
  
}

int main(int argc, char** argv)
{
  int Nx = 4, Ny = 4;
  double L_x = 1.0, L_y = 1.0;
  double dx = L_x / Nx, dy = L_y / Ny;
  GridObject density(Nx, Ny);
  density.print_grid_data();

  Field myfield(Nx, Ny, dx, dy);
  myfield.solve_field(density);
  myfield.print_field();

  return 0;
}