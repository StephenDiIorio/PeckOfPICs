#include <cmath>
#include <iostream>
#include <fstream>
using namespace std;

#include "Simulation.h"
#include "Field.h"
#include "FFT.h"

int main()
{
    ofstream x_dom, t_dom, dens_out, dens_out_b, dens_out_p, field_out, KE_out, U_out, totE_out, part_x, part_px, part_py;
    GridObject mydens = GridObject();
    Field myfield = Field();
    myfield.solve_field(mydens);

    return 0;
}
