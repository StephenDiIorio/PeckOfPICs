#include "../src/Species.h"
#include "../src/Field.h"
#include <stdlib.h>  // for rand, srand
#include <time.h>  // for time, to initialize random seed
// testing the map fields to particles functionality

// constant field is same everywhere

// point field and point particles

double get_rand()
{
    srand((unsigned)time(NULL));
    return rand() / RAND_MAX;
}

bool test_single_point_field_to_particle(uint field_component, double E0, int b1_x,\
                int b1_y, int Nx, int Ny, double L_x, double L_y,\
                double dx, double dy)
{
    Species box_one(18,Nx,Ny,-1);
    Field testE(Nx, Ny, dx, dy);
    double h_rand, k_rand;
    bool local_field_is_right = true;

    std::vector<double> expected_E_b1;
    switch (field_component)
    {
    case 1:
        testE.f2.set_grid_data(b1_x,b1_y,E0);
        break;
    case 2:
        testE.f3.set_grid_data(b1_x,b1_y,E0);
        break;
    
    default:
        testE.f1.set_grid_data(b1_x,b1_y,E0);
        break;
    }

    //box center
    box_one.add_particle(b1_x*dx , b1_y*dy ,0,get_rand(), get_rand(), get_rand(),1.*dx*dy);
    expected_E_b1.push_back(E0);
    
    // box edges
    box_one.add_particle((b1_x-.5)*dx , b1_y*dy ,0,get_rand(), get_rand(), get_rand(),1.*dx*dy);
    expected_E_b1.push_back(E0/2.);
    box_one.add_particle((b1_x+.5)*dx , b1_y*dy ,0,get_rand(), get_rand(), get_rand(),1.*dx*dy);
    expected_E_b1.push_back(E0/2.);
    box_one.add_particle(b1_x*dx , (b1_y-.5)*dy ,0,get_rand(), get_rand(), get_rand(),1.*dx*dy);
    expected_E_b1.push_back(E0/2.);
    box_one.add_particle(b1_x*dx , (b1_y+.5)*dy ,0,get_rand(), get_rand(), get_rand(),1.*dx*dy);
    expected_E_b1.push_back(E0/2.);
    //box corners
    box_one.add_particle((b1_x-.5)*dx , (b1_y-.5)*dy ,0,get_rand(), get_rand(), get_rand(),1.*dx*dy);
    expected_E_b1.push_back(E0/4.);
    box_one.add_particle((b1_x+.5)*dx , (b1_y-.5)*dy ,0,get_rand(), get_rand(), get_rand(),1.*dx*dy);
    expected_E_b1.push_back(E0/4.);
    box_one.add_particle((b1_x+.5)*dx , (b1_y+.5)*dy ,0,get_rand(), get_rand(), get_rand(),1.*dx*dy);
    expected_E_b1.push_back(E0/4.);
    box_one.add_particle((b1_x-.5)*dx , (b1_y+.5)*dy ,0,get_rand(), get_rand(), get_rand(),1.*dx*dy);
    expected_E_b1.push_back(E0/4.);
    // out of the box
    box_one.add_particle((b1_x+1.5)*dx , b1_y*dy ,0,get_rand(), get_rand(), get_rand(),1.*dx*dy);
    expected_E_b1.push_back(0.);
    
    
    // proper test
    // ----------------------------
    //  |       |   :   |       |  
    //  |   o . | . o . | . o   |
    //  |   :   |   :   |   :   |
    // --------------------------
    //  |   :   |   :   |   :   |  
    //  | . o . |   o   | . o   |
    //  |   :   |   :   |   :   |
    // --------------------------
    //  |   :   |   :   |   :   |  
    //  |   o . | . o . | . o   |
    //  |       |   :   |       |
    // --------------------------
    // quadrant 1
    // h_rand = get_rand();
    // k_rand = get_rand();
    h_rand = .9; k_rand = .8;
    box_one.add_particle( (b1_x -1 + h_rand)*dx ,(b1_y - 1 + k_rand)*dy  ,0,\
        get_rand(), get_rand(), get_rand(),1.*dx*dy);
    expected_E_b1.push_back(E0 * h_rand * k_rand);
    // quadrant 2
    // h_rand = get_rand();
    // k_rand = get_rand();
    h_rand = .9; k_rand = .8;

    box_one.add_particle( (b1_x + h_rand)*dx ,(b1_y - 1 + k_rand)*dy  ,0,\
        get_rand(), get_rand(), get_rand(),1.*dx*dy);
    expected_E_b1.push_back(E0 * (1-h_rand) * (k_rand));
    // quadrant 3
    h_rand = get_rand();
    k_rand = get_rand();
    h_rand = .8; k_rand = .5;
    box_one.add_particle( (b1_x + h_rand)*dx ,(b1_y + k_rand)*dy  ,0,\
        get_rand(), get_rand(), get_rand(),1.*dx*dy);
    expected_E_b1.push_back(E0 * (1-h_rand) * (1-k_rand));
    // quadrant 4
    h_rand = get_rand();
    k_rand = get_rand();
    h_rand = .7; k_rand = .6;
    box_one.add_particle( (b1_x - 1 + h_rand)*dx ,(b1_y + k_rand)*dy  ,0,\
        get_rand(), get_rand(), get_rand(),1.*dx*dy);
    expected_E_b1.push_back(E0 * (h_rand) * (1-k_rand));

    // below
    h_rand = .4; k_rand = .6;
    box_one.add_particle( (b1_x - 1 - h_rand)*dx ,(b1_y -1 - k_rand)*dy  ,0,\
        get_rand(), get_rand(), get_rand(),1.*dx*dy);
    expected_E_b1.push_back(0);
    // right
    h_rand = .4; k_rand = .6;
    box_one.add_particle( (b1_x + 1 + h_rand)*dx ,(b1_y -1 - k_rand)*dy  ,0,\
        get_rand(), get_rand(), get_rand(),1.*dx*dy);
    expected_E_b1.push_back(0);
    // above
    h_rand = .4; k_rand = .6;
    box_one.add_particle( (b1_x - 1 - h_rand)*dx ,(b1_y +1 + k_rand)*dy  ,0,\
        get_rand(), get_rand(), get_rand(),1.*dx*dy);
    expected_E_b1.push_back(0);
    // left
    h_rand = .4; k_rand = .6;
    box_one.add_particle( (b1_x - 1 - h_rand)*dx ,(b1_y -1 - k_rand)*dy  ,0,\
        get_rand(), get_rand(), get_rand(),1.*dx*dy);
    expected_E_b1.push_back(0);
 // end box 1 test

    box_one.map_field_to_part(testE,dx,dy,L_x,L_y,Nx,Ny);

    
    
    std::vector<double> local_e;
    switch (field_component)
    {
    case 1:
        local_e = box_one.get_local_E_y();
        break;
    case 2:
        local_e = box_one.get_local_E_z();
        break;
    
    default:
        local_e = box_one.get_local_E_x();
        break;
    }

    
    for (int ii = 0; ii < expected_E_b1.size(); ++ii)
    {
        if (fabs(expected_E_b1.at(ii) - local_e.at(ii)) >= 1e-12 )
        {
            std::cout << "error is " << fabs(expected_E_b1.at(ii) - local_e.at(ii)) << std::endl;
            std::cout << "expected E " << expected_E_b1.at(ii) << std::endl;
            std::cout << "local E: " << local_e.at(ii);
            std::cout << ", " << (box_one.get_local_E(1)).at(ii);
            std::cout << ", " << (box_one.get_local_E(2)).at(ii) << std::endl;
            local_field_is_right = false;
        }
    }
    if (!local_field_is_right)
    {
        std::cout << "test failed here\n";
        std::cout << "Expected E values:\n";
        for (auto val : expected_E_b1)
        {
            std::cout << val << " ";
        }
        std::cout << std::endl;


        std::cout << "local E values:\n";
        for (auto val : local_e)
        {
            std::cout << val << " ";
        }
        std::cout << std::endl;

    }
    return local_field_is_right;
}

int main(int argc, char **argv)
{

    int Nx = 8, Ny = 8;
    double L_x = 2.0, L_y = 3.0;
    double dx = L_x / Nx, dy = L_y / Ny;
    double E0 = 2.0;

    bool test_passed = true;

    for (int component = 0; component < 3; ++component)
    {
        bool comp_test_passed = true;
        for (int b1_x = 0; b1_x < Nx; ++b1_x)
        {
            for (int b1_y = 0; b1_y < Ny; ++b1_y)
            {
                bool cell_test_passed = test_single_point_field_to_particle(component,\
                    E0, b1_x,b1_y,Nx,Ny,L_x,L_y,dx,dy);
                comp_test_passed = comp_test_passed && cell_test_passed;
                if (!cell_test_passed)
                {
                    std::cout << "interpolation failed at cell " << \
                    b1_x << ", " << b1_y << std::endl;
                }
            }
        }
        test_passed = test_passed && comp_test_passed;
        if (comp_test_passed)
        {
            std::cout << "interpolation works for component " << component << std::endl;
        }
        else
        {
            std::cout << "interpolation fails for component " << component << std::endl;
        }
    }
    if (test_passed)
    {
        std::cout << "field_to_particle is passing its test!\n";
    }
    else
    {
        std::cout << "field_to_particle failed its test!\n";
    }
    
    return 0;
}