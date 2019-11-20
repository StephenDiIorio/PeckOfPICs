#include "Field.h"


/**********************************************************
CONSTRUCTORS/DESTRUCTORS
***********************************************************/
Field::Field() //TODO: See if this can be removed
{
}

Field::Field(uint Nx, uint Ny, double dx, double dy,
    std::function<void(Field &, uint, uint)> init_fcn)
{
    this->size = Nx * Ny;

    this->total_U = 0.0;

    // std::vector<double> k = get_k_vec(this->size, dx);
    // this->K2 = get_K2_vec(k, this->size, dx);
    // this->kappa = get_kappa_vec(k, this->size, dx);

    init_field(init_fcn);
}

Field::~Field()
{
}
//-----------------------------------------


int Field::FFT_2d(GridObject &real_part, GridObject &imag_part,
                  const uint transform_direction)
{
    int err = 0;
    // const uint fft = 1;   // to perform fft
    // const uint ifft = -1; // to perform ifft

    // assumes real_part and im_part are the same size!

    // spectral solve: Fourier transform rows, then columns
    // for each row: collect data, Fourier transform, return, and store
    std::vector<double> xs_re(this->Ny), xs_im(this->Ny);
    for (int xi = 0; xi < this->Nx; ++xi)
    {
        for (int yj = 0; yj < this->Ny; ++yj)
        {
            xs_re.at(yj) = real_part.get_grid_data(xi,yj);
            xs_im.at(yj) = imag_part.get_grid_data(xi,yj);
        }    
        err = FFT(xs_re, xs_im, this->Ny, transform_direction);
        if (err)
        {
            return err;
        }
        for (int yj = 0; yj < this->Ny; ++yj)
        {
            real_part.set_grid_data(xi,yj,xs_re.at(yj));
            imag_part.set_grid_data(xi,yj,xs_im.at(yj));
        }  
    }
    // now columns
    std::vector<double> ys_re(this->Nx), ys_im(this->Nx);
    for (int yj = 0; yj < this->Ny; ++yj)
    {
        for (int xi = 0; xi < this->Nx; ++xi)
        {
            ys_re.at(xi) = real_part.get_grid_data(xi,yj);
            ys_im.at(xi) = imag_part.get_grid_data(xi,yj);
        }    
        err = FFT(ys_re, ys_im, this->Nx, transform_direction);
        if (err)
        {
            return err;
        }
        for (int xi = 0; xi < this->Nx; ++xi)
        {
            real_part.set_grid_data(xi,yj,ys_re.at(xi));
            imag_part.set_grid_data(xi,yj,ys_im.at(xi));
        }  
    }

    return 0;
}

/**
 * @brief solve_field solves Poisson equation with periodic BCs
 * 
 * @param charge_density 
 * @return int 
 */
int Field::solve_field(GridObject &charge_density)
{
    int err = 0;
    const uint fft = 1;   // to perform fft
    const uint ifft = -1; // to perform ifft

    GridObject dens_im(this->Nx, this->Ny);

    // A phi = density
    // 1 fourier transform density
    err = FFT_2d(charge_density, dens_im, fft);

    // then get phi: divide by appropriate value
    GridObject phi_re(charge_density);
    GridObject phi_im(dens_im);
    // set some values of phi to 0.  phi[0,0]?

    // then Ex, Ey are phi times appropriate value


    // GridObject Ex_re(this->Nx, this->Ny);
    GridObject Ex_im(this->Nx, this->Ny);
    // GridObject Ey_re(this->Nx, this->Ny);
    GridObject Ey_im(this->Nx, this->Ny);

    // then Ex, Ey are inverse Fourier transformed.
    err = FFT_2d(f1, Ex_im, ifft);
    FFT_2d(f2, Ey_im, ifft);
    // then inverse Fourier transform back each of Ex, Ey
    // for each row: collect data, Fourier transform, return, and store

    return err;
}

/**
 * @brief 
 * 
 * @param charge_density 
 * @return int 
 */
int solve_field_fftw(GridObject &charge_density)
{
    // consider building fftw plan outside function loop

    // initialize fftw things
    // load charge_density into fftw pointer
    // perform 2d fft
    // get phibar from densitybar
    // get Ebars from phibar
    // ifft
    return 0;
}

int Field::solve_field_spectral(std::vector<double> re, std::vector<double> im)
{
    // For total electrostatic energy diagnostic
    this->total_U = 0.0;

    int err = 0;
    const uint fft = 1;   // to perform fft
    const uint ifft = -1; // to perform ifft

    err = FFT(re, im, this->size, fft);
    if (err)
    {
        return err;
    }

    re.at(0) = im.at(0) = 0.0; // set offset density pert to 0

    for (uint i = 1; i < this->size; ++i) // avoid divide by 0, start at 1
    {
        // For total electrostatic energy diagnostic
        // Can ignore at i=0 since the values are 0
        this->total_U += ((re.at(i) * re.at(i)) + (im.at(i) * im.at(i))) / this->K2.at(i);

        //calculate phi
        re.at(i) /= this->K2.at(i);
        im.at(i) /= this->K2.at(i);

        //calculate E
        re.at(i) *= -this->kappa.at(i);
        im.at(i) *= this->kappa.at(i); //multiply i by i so get extra factor of -1
    }
    // By the end of this, the real part has become the imaginary part,
    // and vice versa, so we switch the order in the inverse fft
    err = FFT(im, re, this->size, ifft);
    if (err)
    {
        return err;
    }
    this->f1 = GridObject(this->size, 0, im);

    // For total electrostatic energy diagnostic
    this->total_U *= 0.5;

    return err;
}

void Field::print_field()
{
	f1.print_grid_data();
	f2.print_grid_data();
	f3.print_grid_data();
}

/**********************************************************
PRIVATE FUNCTIONS
***********************************************************/
void Field::init_field(std::function<void(Field &, uint, uint)> init_fcn)
{
    init_fcn(*this, this->Nx, this->Ny);
}


