#include "../src/Field.h"

#include <stdlib.h>    /* for exit */
#include <getopt.h>

enum type_field_test {linear_x,linear_y,sinusoid};

int main(int argc, char **argv)
{


	int opt;
  int Nx = 8, Ny = 8;
	double rho0 = 1.;
  double L_x = 1.4, L_y = 1.6;

  std::string test_options;
  while ((opt = getopt(argc, argv, "n:m:x:y:r:")) != -1) {
    switch (opt) {
      case 'n':
      {
        Nx = atoi(optarg);
      }
        break;
			case 'm':
			{
				Ny = atoi(optarg);
			}
			break;
			case 'x':
			{
				L_x = atof(optarg);
			}
			break;
			case 'y':
			{
				L_y = atof(optarg);
			}
			break;
			case 'r':
			{
				rho0 = atof(optarg);
			}
			break;
      default: /* '?' */
        fprintf(stderr, "Usage:  [-n number steps][-dt step][-q charge]");
        exit(EXIT_FAILURE);
    }
  }


  // int Nx = 32, Ny = 32;
  double dx = L_x / Nx, dy = L_y / Ny;
  // set a line of density to 0
  GridObject density(Nx, Ny);
  int xi = 3; int yj =3;
  double kx = 2*M_PI / L_x;
  double ky = 2*M_PI / L_y;
  type_field_test this_test = linear_x;

  switch (this_test)
  {
  case linear_x:
    {
      for (yj = 0; yj < Ny; ++yj)
      {
          density.set_comp(xi,yj,rho0/dy);

      }
    }
    break;
  case linear_y:
    {
      for (yj = 0; yj < Ny; ++yj)
      {
          density.set_comp(xi,yj,rho0/dx);

      }
    }
    break;
  default:
    { // sinusoid
      for(xi = 0; xi < Nx; ++xi)
      {
        for (yj = 0; yj < Ny; ++yj)
        {
          density.set_comp(xi, yj, rho0 + .1 * rho0*cos(kx * xi * dx)*\
                                                    cos(ky*yj*dy));
        }
      }
    }
    break;
  } // end switch statement

  Field myfield(Nx, Ny, dx, dy);
  myfield.solve_field(density);

  density.print();
  myfield.f1.print();
  myfield.f2.print();

  return 0;
}