#include "Field.h"


/**********************************************************
CONSTRUCTORS/DESTRUCTORS
***********************************************************/

/**
 * @brief Constructor for Field object
 *
 */
Field::Field() //TODO: See if this can be removed
{
}

/**
 * @brief Constructor for Field object - sets all values to 0
 *
 * @param Nx Number of grid spaces in x direction
 * @param Ny Number of grid spaces in y direction
 * @param dx Spatial grid step in x direction
 * @param dy Spatial grid step in y direction
 */
Field::Field(const std::size_t Nx, const std::size_t Ny,
             const double dx, const double dy)
{
    this->total_U = 0.0;

    std::vector<double> k_x = get_k_vec(Nx, dx);
    std::vector<double> k_y = get_k_vec(Ny, dy);

    this->K_x2 = get_K2_vec(k_x, dx);
    this->K_y2 = get_K2_vec(k_y, dy);
    this->Kappa_x = get_kappa_vec(k_x, dx);
    this->Kappa_y = get_kappa_vec(k_y, dy);

    this->f1 = GridObject(Nx, Ny);
    this->f2 = GridObject(Nx, Ny);
    this->f3 = GridObject(Nx, Ny);
}

/**
 * @brief Constructor for Field object
 *
 * @param Nx Number of grid spaces in x direction
 * @param Ny Number of grid spaces in y direction
 * @param dx Spatial grid step in x direction
 * @param dy Spatial grid step in y direction
 * @param component Which field component to initialize
 * @param value The value to initialize the field component to
 */
Field::Field(const std::size_t Nx, const std::size_t Ny,
             const double dx, const double dy,
             std::size_t component, double value)
{
    this->total_U = 0.0;

    std::vector<double> k_x = get_k_vec(Nx, dx);
    std::vector<double> k_y = get_k_vec(Ny, dy);

    this->K_x2 = get_K2_vec(k_x, dx);
    this->K_y2 = get_K2_vec(k_y, dy);
    this->Kappa_x = get_kappa_vec(k_x, dx);
    this->Kappa_y = get_kappa_vec(k_y, dy);

    switch(component)
    {
        case x1_accessor:
            this->f1 = GridObject(Nx, Ny, value);
            this->f2 = GridObject(Nx, Ny);
            this->f3 = GridObject(Nx, Ny);
            break;
        case x2_accessor:
            this->f1 = GridObject(Nx, Ny);
            this->f2 = GridObject(Nx, Ny, value);
            this->f3 = GridObject(Nx, Ny);
            break;
        case x3_accessor:
            this->f1 = GridObject(Nx, Ny);
            this->f2 = GridObject(Nx, Ny);
            this->f3 = GridObject(Nx, Ny, value);
            break;
        default:
            throw std::runtime_error(no_dimension_err);
            break;
    }
}

/**
 * @brief Constructor for Field object
 *
 * @param Nx Number of grid spaces in x direction
 * @param Ny Number of grid spaces in y direction
 * @param dx Spatial grid step in x direction
 * @param dy Spatial grid step in y direction
 * @param init_fcn User provided function which initializes the field to the
 *                 user's specification
 */
Field::Field(const std::size_t Nx,const  std::size_t Ny,
             const double dx, const double dy,
             std::function<void(Field &, std::size_t, std::size_t)> init_fcn)
{
    this->total_U = 0.0;

    std::vector<double> k_x = get_k_vec(Nx, dx);
    std::vector<double> k_y = get_k_vec(Ny, dy);

    this->K_x2 = get_K2_vec(k_x, dx);
    this->K_y2 = get_K2_vec(k_y, dy);
    this->Kappa_x = get_kappa_vec(k_x, dx);
    this->Kappa_y = get_kappa_vec(k_y, dy);

    init_field(init_fcn, Nx, Ny);
}

/**
 * @brief Destructor for Field object
 *
 */
Field::~Field()
{
}
//-----------------------------------------


/**********************************************************
CLASS METHODS
***********************************************************/

/**
 * @brief Solves Poisson equation with periodic BCs
 * @param charge_density The charge densitt distribution to calculate the
 *                       resulting field of
 * @param dx Spatial grid step in x direction
 * @param dy Spatial grid step in y direction
 * @return int An error code or 0 if it worked correctly
 */
int Field::solve_field(const GridObject& charge_density,
                       const double dx, const double dy)
{
    int err = 0;

    const std::size_t Nx = charge_density.get_Nx();
    const std::size_t Ny = charge_density.get_Ny();

    phi_dens_re = GridObject(charge_density);
    phi_dens_im = GridObject(Nx, Ny);

    // A phi = density
    // 1 fourier transform density
    err = FFT_2D(phi_dens_re, phi_dens_im, FFT_Dir::FFT);

    // Set k=0 mode to zero
    phi_dens_re.set_comp(0, 0, 0);
    phi_dens_im.set_comp(0, 0, 0);


    for (std::size_t xi = 0; xi < Nx; ++xi)
    {
        // double kx = (2. * M_PI * xi) / (Nx * dx);
        // double sinckx2 = sinc(kx * dx / 2.);
        // double Klsq = kx*kx * sinckx2*sinckx2;

        for (std::size_t yj = 0; yj < Ny; ++yj)
        {
            // Don't overwrite the k=0 mode
            if (xi == 0 and yj == 0)
            {
                continue;
            }

            // double ky = (2. * M_PI * yj) / (Ny * dy);
            // double sincky2 = sinc(ky * dy / 2.);

            // double Klmsq_ij = Klsq + (ky*ky * sincky2*sincky2);
            double Klmsq_ij = this->K_x2[xi] + this->K_y2[yj];

            // energy is rhobar * phibar conj = |rhobar|^2 / Klm^2
            this->total_U += (phi_dens_re.get_comp(xi,yj) *
                              phi_dens_re.get_comp(xi,yj) +
                              phi_dens_im.get_comp(xi,yj) *
                              phi_dens_im.get_comp(xi,yj) ) / Klmsq_ij;

            phi_dens_re.comp_multiply_by(xi, yj, 1./Klmsq_ij);
            phi_dens_im.comp_multiply_by(xi, yj, 1./Klmsq_ij);
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

    for (std::size_t xi = 0; xi < Nx; ++xi)
    {
        // double kappa_x = (2. * M_PI * xi) / (Nx * dx);
        // double sinc_kappa_x = sinc(kappa_x * dx);
        // double Kappa_i = kappa_x * sinc_kappa_x;

        for (std::size_t yj = 0; yj < Ny; ++yj)
        {
            // Don't overwrite the k=0 mode
            if (xi == 0 and yj == 0)
            {
                continue;
            }

            // double kappa_y = (2. * M_PI * yj) / (Ny * dy);
            // double sinc_kappa_y = sinc(kappa_y * dy);

            // double Kappa_j = kappa_y * sinc_kappa_y;
            double Kappa_i = Kappa_x[xi];
            double Kappa_j = Kappa_y[yj];

            // TODO: is negative sign in the f_ or the E__im ?
            f1.comp_multiply_by(xi, yj, -Kappa_i);
            Ex_im.comp_multiply_by(xi, yj, Kappa_i);
            f2.comp_multiply_by(xi, yj, -Kappa_j);
            Ey_im.comp_multiply_by(xi, yj, Kappa_j);
        }
    }

    // then Ex, Ey are inverse Fourier transformed.
    err = FFT_2D(f1, Ex_im, FFT_Dir::iFFT);
    err = FFT_2D(f2, Ey_im, FFT_Dir::iFFT);

    // For total electrostatic energy diagnostic
    this->total_U *= 0.5;

    return err;
}

/**
 * @brief Prints all of the components of the field
 *
 */
void Field::print_field()
{
    std::cout << "F1:" << std::endl;
	f1.print();
    std::cout << "F2:" << std::endl;
	f2.print();
    std::cout << "F3:" << std::endl;
	f3.print();
}
//-----------------------------------------


/**********************************************************
PRIVATE FUNCTIONS
***********************************************************/
void Field::init_field(std::function<void(Field &, std::size_t, std::size_t)> init_fcn, std::size_t Nx, std::size_t Ny)
{
    init_fcn(*this, Nx, Ny);
}
//-----------------------------------------


/**********************************************************
TO BE IMPLEMENTED
***********************************************************/

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
    return 1;
}

int Field::solve_field_spectral(std::vector<double> re, std::vector<double> im)
{
    int err = 0;

    // // For total electrostatic energy diagnostic
    // this->total_U = 0.0;

    // const uint fft = 1;   // to perform fft
    // const uint ifft = -1; // to perform ifft

    // err = FFT(re, im, this->size, fft);
    // if (err)
    // {
    //     return err;
    // }

    // re.at(0) = im.at(0) = 0.0; // set offset density pert to 0

    // for (uint i = 1; i < this->size; ++i) // avoid divide by 0, start at 1
    // {
    //     // For total electrostatic energy diagnostic
    //     // Can ignore at i=0 since the values are 0
    //     this->total_U += ((re.at(i) * re.at(i)) + (im.at(i) * im.at(i))) / this->K2.at(i);

    //     //calculate phi
    //     re.at(i) /= this->K2.at(i);
    //     im.at(i) /= this->K2.at(i);

    //     //calculate E
    //     re.at(i) *= -this->kappa.at(i);
    //     im.at(i) *= this->kappa.at(i); //multiply i by i so get extra factor of -1
    // }
    // // By the end of this, the real part has become the imaginary part,
    // // and vice versa, so we switch the order in the inverse fft
    // err = FFT(im, re, this->size, ifft);
    // if (err)
    // {
    //     return err;
    // }
    // this->f1 = GridObject(this->size, 0, im);

    // // For total electrostatic energy diagnostic
    // this->total_U *= 0.5;

    return err;
}
//-----------------------------------------
