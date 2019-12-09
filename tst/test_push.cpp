#include "../src/Species.h"
#include <stdlib.h>    /* for exit */
#include <getopt.h>

// testing the push particles functionality of Species
// Requires that the map_field_to_particles functionality is working

void test_const_E(int Nt, double E0)
{

	uint Nx = 10, Ny = 10;
	double L_x = 1.5, L_y = 1.5;
	double dx = L_x / Nx, dy = L_y / Ny;
	// double E0 = .1;
	double dt = .01;
	// int Nt = 1000;


	Species testSpec(1, Nx, Ny, -1.);
	testSpec.add_particle(0.,0.,0.,.01,.005,.01,1.*dx*dy);
	Field constE(Nx, Ny, dx, dy,0,E0);
	testSpec.map_field_to_part(constE, electric, dx, dy, L_x, L_y, Nx, Ny);
	
	// my hack of an output dump: print things,
	// then in analysis script, redirect output to a file
	// std::cout << "pos" << std::endl;
	testSpec.print_pos();
	// std::cout << "mom" << std::endl;
	testSpec.print_mom();
	// std::cout << "local e" << std::endl;
	testSpec.print_local_e_field();
	// std::cout << "e" << std::endl;
	// constE.print_field();


	for (int iter_num = 0; iter_num < Nt; ++iter_num)
	{
		testSpec.push_particles(L_x, L_y, dt, dx, dy);
		// std::cout << "pos" << std::endl;
		testSpec.print_pos();

		// std::cout << "mom" << std::endl;
		testSpec.print_mom();
		// std::cout << "local e" << std::endl;
		testSpec.print_local_e_field();
	}

}

void test_const_B(int Nt, double B0, double u, double v, double w)
{

	uint Nx = 10, Ny = 10;
	double L_x = 1.5, L_y = 1.5;
	double dx = L_x / Nx, dy = L_y / Ny;
	double dt = .01;
	// int Nt = 100;


	Species testSpec(1, Nx, Ny, -1.);
	testSpec.add_particle(0.,0.,0.,u,v,w,1.*dx*dy);
	Field constB(Nx, Ny, dx, dy,2, B0);
	testSpec.map_field_to_part(constB, magnetic, dx, dy, L_x, L_y, Nx, Ny);
	
	// my hack of an output dump: print things,
	// then in analysis script, redirect output to a file
	// std::cout << "pos" << std::endl;
	testSpec.print_pos();
	// std::cout << "mom" << std::endl;
	testSpec.print_mom();
	// std::cout << "local b" << std::endl;
	testSpec.print_local_b_field();
	// std::cout << "e" << std::endl;
	// constE.print_field();


	for (int iter_num = 0; iter_num < Nt; ++iter_num)
	{
		testSpec.push_particles(L_x, L_y, dt, dx, dy);
		// std::cout << "pos" << std::endl;
		testSpec.print_pos();

		// std::cout << "mom" << std::endl;
		testSpec.print_mom();
		// std::cout << "local b" << std::endl;
		testSpec.print_local_b_field();
	}

}

void test_const_EB(int Nt, double E0, double B0, double u, double v, double w)
{

	uint Nx = 10, Ny = 10;
	double L_x = 1.5, L_y = 1.5;
	double dx = L_x / Nx, dy = L_y / Ny;
	double dt = .01;
	// int Nt = 100;


	Species testSpec(1, Nx, Ny, -1.);
	testSpec.add_particle(0.,0.,0.,u,v,w,1.*dx*dy);
	Field constE(Nx, Ny, dx, dy,1, E0);
	Field constB(Nx, Ny, dx, dy,2, B0);
	testSpec.map_field_to_part(constE, electric, dx, dy, L_x, L_y, Nx, Ny);
	testSpec.map_field_to_part(constB, magnetic, dx, dy, L_x, L_y, Nx, Ny);
	
	// my hack of an output dump: print things,
	// then in analysis script, redirect output to a file
	// std::cout << "pos" << std::endl;
	testSpec.print_pos();
	// std::cout << "mom" << std::endl;
	testSpec.print_mom();
	// std::cout << "local b" << std::endl;
	// testSpec.print_local_b_field();
	// std::cout << "e" << std::endl;
	// constE.print_field();


	for (int iter_num = 0; iter_num < Nt; ++iter_num)
	{
		testSpec.push_particles(L_x, L_y, dt, dx, dy);
		// std::cout << "pos" << std::endl;
		testSpec.print_pos();

		// std::cout << "mom" << std::endl;
		testSpec.print_mom();
		// std::cout << "local b" << std::endl;
		// testSpec.print_local_b_field();
	}

}




int main(int argc, char** argv)
{

	int opt;
	int Nt = 0;
	double amp = 0, u=0, v=0, w=0;
  std::string test_options;
  while ((opt = getopt(argc, argv, "n:a:u:v:w:")) != -1) {
    switch (opt) {
      case 'n':
      {
        Nt = atoi(optarg);
      }
        break;
      case 'a':
      {
				amp = atof(optarg);
      }// end case 'f' for test field
        break;
      case 'u':
      {
				u = atof(optarg);
      }// end case 'f' for test field
        break;
      case 'v':
      {
				v = atof(optarg);
      }// end case 'f' for test field
        break;
      case 'w':
      {
				w = atof(optarg);
      }// end case 'f' for test field
        break;
      default: /* '?' */
        fprintf(stderr, "Usage:  [-n number steps][-a amplitude]\n");
        exit(EXIT_FAILURE);
    }
  }
  
	// test: particle seeing constant E has constant acceleration
	// test_const_E(.1);
	// test: particle seeing constant B does circles
	// std::cout << Nt << "amp " << amp << std::endl;
	test_const_EB(Nt, .1, amp,u,v,w);
	// test: particle seeing constant E, B does cycloid with ExB drift
	// test: magnetic bottle?
    
}