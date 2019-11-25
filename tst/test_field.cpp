#include "../src/Field.h"

//test that zero densith gives zero phi, E, energy

const double TOL = 1e-12;

int zero_density_zero_field(int Nx, int Ny, double L_x, double L_y)
{
  GridObject density(Nx, Ny); // is a zero object

  double dx = L_x / Nx, dy = L_y / Ny;
  Field myfield(Nx, Ny, dx, dy);
  myfield.solve_field(density);

  if (myfield.f1.compare_with(density, TOL) &&
    myfield.f2.compare_with(density,TOL) &&
    myfield.f3.compare_with(density,TOL) )
    {
      if (myfield.total_U != 0)
      {
        return false
      }
      else
      {
        return true;
      }
      
    }
    else
    {
      return false;
    }   
}

int constant_density_zero_field(int Nx, int Ny, double L_x, double L_y, double rho0)
{
  GridObject density(Nx, Ny, rho0); // is a constant object

  double dx = L_x / Nx, dy = L_y / Ny;
  Field myfield(Nx, Ny, dx, dy);
  myfield.solve_field(density);

  if (myfield.f1.compare_with(density, TOL) &&
    myfield.f2.compare_with(density,TOL) &&
    myfield.f3.compare_with(density,TOL) )
    {
      if (myfield.total_U == 0)
      {
        return false;
      }
      else
      {
        return false;
      }
      
    }
    else
    {
      return false;
    }   
}

int point_density_right_field(int Nx, int Ny, double L_x, double L_y, int xa, int yb, double rho1)
{
  GridObject density(Nx, Ny); // is a constant object
  density.set_grid_data(xa, yb, rho1);
  double dx = L_x / Nx, dy = L_y / Ny;
  Field myfield(Nx, Ny, dx, dy);
  myfield.solve_field(density);

  GridObject correct_f1(Nx,Ny), correct_f2(Nx,Ny);

  if (myfield.f1.compare_with(correct_f1, TOL) &&
    myfield.f2.compare_with(correct_f2,TOL) &&
    myfield.f3.compare_with(density,TOL) )
    {
      if (myfield.total_U == 0)
      {
        return false;
      }
      else
      {
        return false;
      }
      
    }
    else
    {
      return false;
    }   
}





int main(int argc, char** argv)
{
  int Nx = 4, Ny = 4;
  double L_x = 1.0, L_y = 1.0;
  double dx = L_x / Nx, dy = L_y / Ny;
  GridObject density(Nx, Ny);
  density.set_grid_data(1.,1.,1.);
  density.print_grid_data();

  Field myfield(Nx, Ny, dx, dy);
  myfield.solve_field(density);
  myfield.print_field();
  std::cout << "Energy is " << myfield.total_U << std::endl;

  if (zero_density_zero_field(Nx,Ny,L_x,L_y) )
  {
    std::cout << "Pass: Zero density has zero field.\n";
  }
  else
  {
    std::cout << "Fail: zero density has nonzero field!\n";
  }

    if (constant_density_zero_field(Nx,Ny,L_x,L_y) )
  {
    std::cout << "Pass: Zero density has zero field.\n";
  }
  else
  {
    std::cout << "Fail: zero density has nonzero field!\n";
  }

    if (point_density_right_field(Nx,Ny,L_x,L_y) )
  {
    std::cout << "Pass: point density has right field.\n";
  }
  else
  {
    std::cout << "Fail: point density has wrong field!\n";
  }
  

  return 0;
}