#include "Field.h"


/**********************************************************
CONSTRUCTORS/DESTRUCTORS
***********************************************************/
Field::Field() //TODO: See if this can be removed
{
}

Field::Field(uint Nx, uint Ny, double dx, double dy)
{
    this->size = Nx * Ny;
    this->Nx = Nx;
    this->Ny = Ny;
    this->dx = dx;
    this->dy = dy;

    this->total_U = 0.0;

    this->f1 = GridObject(this->Nx, this->Ny);
    this->f2 = GridObject(this->Nx, this->Ny);
    this->f3 = GridObject(this->Nx, this->Ny);
}

Field::Field(uint Nx, uint Ny, double dx, double dy,\
            uint component, double value)
{
    this->size = Nx * Ny;
    this->Nx = Nx;
    this->Ny = Ny;
    this->dx = dx;
    this->dy = dy;

    this->total_U = 0.0;

    switch(component)
    {
        case 1:
            this->f1 = GridObject(this->Nx, this->Ny);
            this->f2 = GridObject(this->Nx, this->Ny, value);
            this->f3 = GridObject(this->Nx, this->Ny);
            break;
        case 2:
            this->f1 = GridObject(this->Nx, this->Ny);
            this->f2 = GridObject(this->Nx, this->Ny);
            this->f3 = GridObject(this->Nx, this->Ny, value);
            break;
        default:
            this->f1 = GridObject(this->Nx, this->Ny, value);
            this->f2 = GridObject(this->Nx, this->Ny);
            this->f3 = GridObject(this->Nx, this->Ny);
            break;
    }
}

Field::Field(uint Nx, uint Ny, double dx, double dy,
    std::function<void(Field &, uint, uint)> init_fcn)
{
    this->size = Nx * Ny;
    this->Nx = Nx;
    this->Ny = Ny;
    this->dx = dx;
    this->dy = dy;

    this->total_U = 0.0;

    init_field(init_fcn);
}

Field::~Field()
{
}
//-----------------------------------------

/**
 * @brief Home-made 2d dft used in the spectral field solve
 * 
 * @param real_part 
 * @param imag_part 
 * @param transform_direction 
 * @return int 
 */
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
            xs_re.at(yj) = real_part.get_comp(xi,yj);
            xs_im.at(yj) = imag_part.get_comp(xi,yj);
        }
        err = FFT(xs_re, xs_im, this->Ny, transform_direction);
        if (err)
        {
            return err;
        }
        for (int yj = 0; yj < this->Ny; ++yj)
        {
            real_part.set_comp(xi,yj,xs_re.at(yj));
            imag_part.set_comp(xi,yj,xs_im.at(yj));
        }
    }
    // now columns
    std::vector<double> ys_re(this->Nx), ys_im(this->Nx);
    for (int yj = 0; yj < this->Ny; ++yj)
    {
        for (int xi = 0; xi < this->Nx; ++xi)
        {
            ys_re.at(xi) = real_part.get_comp(xi,yj);
            ys_im.at(xi) = imag_part.get_comp(xi,yj);
        }
        err = FFT(ys_re, ys_im, this->Nx, transform_direction);
        if (err)
        {
            return err;
        }
        for (int xi = 0; xi < this->Nx; ++xi)
        {
            real_part.set_comp(xi,yj,ys_re.at(xi));
            imag_part.set_comp(xi,yj,ys_im.at(xi));
        }
    }

    return 0;
}

/**
 * @brief solve_field is a spectral solver for the 2d Poisson equation with periodic BCs
 *
 * @param charge_density
 * @return int
 */
int Field::solve_field(const GridObject &charge_density)
{
    int err = 0;
    const uint fft = 1;   // to perform fft
    const uint ifft = -1; // to perform ifft

    phi_dens_re = GridObject(charge_density);
    phi_dens_im = GridObject(this->Nx, this->Ny);

    // A phi = density
    // 1 fourier transform density
    err = FFT_2d(phi_dens_re, phi_dens_im, fft);

    // set some values of phi to 0.  phi[0,0]?
    phi_dens_re.set_comp(0,0,0);
    phi_dens_im.set_comp(0,0,0);


    for (int xi =0; xi < this->Nx; ++xi)
    {
        double kx = (2. * M_PI * xi) / (this->Nx * this->dx);
        double sinckx2 = sinc(kx* this->dx / 2.);
        double Klsq = kx*kx *sinckx2 * sinckx2;

        for (int yj = 0; yj < this->Ny; ++yj)
        {
            if (xi == 0 and yj == 0)
            {
                continue;
            }
            double ky = (2. * M_PI * yj) / (this->Ny * this->dy);
            double sincky2 = sinc(ky* this->dy / 2.);

            double Klmsq_ij = Klsq + (ky*ky * sincky2*sincky2);
            // energy is rhobar * phibar conj = |rhobar|^2 / Klm^2
            this->total_U += (phi_dens_re.get_comp(xi,yj) *
                phi_dens_re.get_comp(xi,yj) +
                phi_dens_im.get_comp(xi,yj) *
                phi_dens_im.get_comp(xi,yj) ) / Klmsq_ij;
            phi_dens_re.comp_multiply_by(xi,yj, 1./Klmsq_ij);
            phi_dens_im.comp_multiply_by(xi,yj, 1./Klmsq_ij);
        }
    }
    // then Ex, Ey are phi times appropriate value
    // Ex(l,m) = -i kl sinc(kl dx) phi(l,m)
    // Ey(l,m) = -i km sinc(km dy) phi(l,m)
    // or:
    // Ex_re(l,m) = kl sinc(kl dx) phi_im
    // Ex_im(l,m) = - kl sinc(kl dx) phi_re
    // Ey_re(l,m) = km sinc(km dy) phi_im
    // Ey_im(l,m) = - km sinc(km dy) phi_re

    f1 = GridObject(phi_dens_im);
    GridObject Ex_im(phi_dens_re);
    f2 = phi_dens_im;
    GridObject Ey_im = phi_dens_re;

    for (int xi =0; xi < this->Nx; ++xi)
    {
        double kappa_x = (2. * M_PI * xi) / (this->Nx * this->dx);
        double sinc_kappa_x = sinc(kappa_x* this->dx);
        double Kappa_i = kappa_x *sinc_kappa_x;

        for (int yj = 0; yj < this->Ny; ++yj)
        {
            if (xi == 0 and yj == 0)
            {
                continue;
            }
            double kappa_y = (2. * M_PI * yj) / (this->Ny * this->dy);
            double sinc_kappa_y = sinc(kappa_y* this->dy);

            double Kappa_j = kappa_y * sinc_kappa_y;
            // is negative sign in the f_ or the E__im ? 
            // the current ordering of signs is theoretically dubious
            // but it gives the electric field the correct sign
            f1.comp_multiply_by(xi,yj, -Kappa_i);
            Ex_im.comp_multiply_by(xi,yj, Kappa_i);
            f2.comp_multiply_by(xi,yj, -Kappa_j);
            Ey_im.comp_multiply_by(xi,yj, Kappa_j);
        }
    }

    // then Ex, Ey are inverse Fourier transformed.
    err = FFT_2d(f1, Ex_im, ifft);
    err = FFT_2d(f2, Ey_im, ifft);
    // then inverse Fourier transform back each of Ex, Ey
    // for each row: collect data, Fourier transform, return, and store

    // For total electrostatic energy diagnostic
    this->total_U *= 0.5;

    return err;
}

/**
 * @brief !Not implemented yet! intended to be a spectral solver 
 * for the 2d Poisson equation using the 2d dft of fftw
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
    return 1;
}


void Field::print_field()
{
    std::cout << "F1:" << std::endl;
	f1.print();
    std::cout << "F2:" << std::endl;
	f2.print();
    std::cout << "F3:" << std::endl;
	f3.print();
}

/**********************************************************
PRIVATE FUNCTIONS
***********************************************************/
void Field::init_field(std::function<void(Field &, uint, uint)> init_fcn)
{
    init_fcn(*this, this->Nx, this->Ny);
}


