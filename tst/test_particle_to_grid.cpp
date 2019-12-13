#include "../src/Species.h"
#include <stdlib.h>    /* for exit */
#include <getopt.h>

int main(int argc, char ** argv)
{
	int opt;
  int Nx = 8, Ny = 8;
  double L_x = 1.4, L_y = 1.6;
  double xpar=0, ypar=0;

  std::string test_options;
  while ((opt = getopt(argc, argv, "n:m:x:y:u:v:")) != -1) {
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
			case 'u':
			{
				xpar = L_x * atof(optarg);
			}
			break;
			case 'v':
			{
				ypar = L_y * atof(optarg);
			}
			break;
      default: /* '?' */
        fprintf(stderr, "Usage:  [-n number steps][-dt step][-q charge]");
        exit(EXIT_FAILURE);
    }
  }

  double dx = L_x/Nx, dy = L_y/Ny;
  double Q = 1;
  uint Npar = 1;
  Species myspec(Npar, Nx, Ny, Q);

  myspec.add_particle(xpar,ypar,0,0,0,0,1.);
  myspec.deposit_charge(dx,dy,L_x,L_y,Nx,Ny);

  std::cout << "Particle positions are" << std::endl;
  myspec.print_pos();
  std::cout << "Particle weights are" << std::endl;
  myspec.print_weight();
  std::cout << "Particle charge is " << myspec.Qpar;

  std::cout << "Charge density on grid is" << std::endl;
  myspec.print_density();

}