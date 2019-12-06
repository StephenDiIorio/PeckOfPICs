#include "../src/Species.h"

// testing the push particles functionality of Species
// Requires that the map_field_to_particles functionality is working

int main(int argc, char** argv)
{

	Species testSpec;
	uint Nx = 10, Ny = 10;
	double L_x = 1., L_y = 1.5;
	double dx = L_x / Nx, dy = L_y / Ny;
	double E0 = 1.2;
	double dt = .1;
	int Nt = 100;

	testSpec.add_particle(0.,0.,0.,-1.,-1.,-1.,1.);
	Field constE(Nx, Ny, dx, dy ,0,1);
	testSpec.map_field_to_part(constE, dx, dy, L_x, L_y, Nx, Ny);

	for (int iter_num = 0; iter_num < Nt; ++iter_num)
	{
		testSpec.push_particles(L_x, L_y, dt, dx, dy);
	}

	// test: particle seeing constant E has constant acceleration
	// test: particle seeing constant B does circles
	// test: particle seeing constant E, B does cycloid with ExB drift
	// test: magnetic bottle?
    return 0;
}