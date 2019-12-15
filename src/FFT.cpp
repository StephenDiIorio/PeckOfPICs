/*
****************************************************************************
*                                                                          *
*   FFT Header file, based on code from www.codeproject.com, originally    *
*   based on code from Numerical Recipes with refinement in speed.         *
*   Added functionality to load in complex and real arrays          	   *
*   separately. This requires a number of values which is a power of 2.    *
*   This version is also normalized so that the 1/N is taken into account. *
*   AGRT 2010                                                              *
*                                                                          *
****************************************************************************

    data_re -> float array that represent the real array of complex samples
    data_im -> float array that represent the imag array of complex samples
    NVALS -> length of real or imaginary arrays (N^2 order number)
    isign -> 1 to calculate FFT and -1 to calculate Reverse FFT

    The function returns an integer, 0 if FFT ran, 1 otherwise. It will be
    one if NVALS is not a power of 2
*/

#include "FFT.h"

#define SWAP(a, b) \
    tempr = (a);   \
    (a) = (b);     \
    (b) = tempr
//tempr is a variable from our FFT function

/**
 * @brief Computes the value of sinc(x)
 *
 * @param x The position to evaluate the sinc function
 * @return double The value of sinc(x)
 */
double sinc(const double x)
{
    if (x != 0.0)
    {
        return sin(x) / x;
    }
    else
    {
        return 1.0;
    }
}

/**
 * @brief Performs a 1D (inverse) Fourier transform of a grid
 *
 * @param data_re The real part of the grid to (i)FFT
 * @param data_im The imaginary part of the grid to (i)FFT
 * @param isign Whether to perform an FFT or an IFFT
 * @return int An error code or 0 if it worked correctly
 */
int FFT_1D(std::vector<double>& data_re, std::vector<double>& data_im,
                FFT_Dir isign)
{
    // assumes real_part and im_part are the same size!
    const std::size_t NVALS = data_re.size();

    // TEST THAT NVALS IS A POWER OF 2
    const int err = !(NVALS && !(NVALS & (NVALS - 1)));
    if (!err)
    {
        /*
	    the complex array is real+complex so the array
	    as a size n = 2 * number of samples
	    real part is data[index] and the complex part is data[index+1]
        */
        const std::size_t n = NVALS << 1; // bitwise multiply by 2

        /*
	    Pack components into double array of size n - the ordering
	    is data[0]=real[0], data[1]=imag[0] .......
        */
        std::vector<double> data(n);
        for (std::size_t i = 0, j = 0; j < n; ++i, j += 2)
        {
            data[j] = data_re[i];
            data[j + 1] = data_im[i];
        }
        /*
	    binary inversion (note that
	    the indexes start from 1 which means that the
	    real part of the complex is on the odd-indexes
	    and the complex part is on the even-indexes
        */

        std::size_t j = 0;
        for (std::size_t i = 0; i < n / 2; i += 2)
        {
            if (j > i)
            {
                double tempr; // for the SWAP macro

                //swap the real part
                SWAP(data[j], data[i]);

                //swap the complex part
                SWAP(data[j + 1], data[i + 1]);

                // checks if the changes occurs in the first half
                // and use the mirrored effect on the second half
                if ((j / 2) < (n / 4))
                {
                    //swap the real part
                    SWAP(data[(n - (i + 2))], data[(n - (j + 2))]);

                    //swap the complex part
                    SWAP(data[(n - (i + 2)) + 1], data[(n - (j + 2)) + 1]);
                }
            }

            std::size_t m = NVALS;
            while (m >= 2 && j >= m)
            {
                j -= m;
                m >>= 1;
            }
            j += m;
        }

        //Danielson-Lanzcos routine
        std::size_t mmax = 2;
        while (n > mmax)
        {
            std::size_t istep = mmax << 1;
            double theta = isign * (2.0 * M_PI / mmax);
            double wtemp = sin(0.5 * theta);
            double wpr = -2.0 * wtemp * wtemp;
            double wpi = sin(theta);
            double wr = 1.0;
            double wi = 0.0;
            //internal loops

            for (std::size_t m = 1; m < mmax; m += 2)
            {
                for (std::size_t i = m; i <= n; i += istep)
                {
                    std::size_t j = i + mmax;
                    double tempr = wr * data[j - 1] - wi * data[j];
                    double tempi = wr * data[j] + wi * data[j - 1];
                    data[j - 1] = data[i - 1] - tempr;
                    data[j] = data[i] - tempi;
                    data[i - 1] += tempr;
                    data[i] += tempi;
                }
                wr = (wtemp = wr) * wpr - wi * wpi + wr;
                wi = wi * wpr + wtemp * wpi + wi;
            }
            mmax = istep;
        }
        /*
	    Return elements to real and complex components
        */
        switch (isign)
        {
            case FFT_Dir::FFT:
                for (std::size_t i = 0, j = 0; j < n; ++i, j += 2)
                {
                    data_re[i] = data[j];
                    data_im[i] = data[j + 1];
                }
                break;
            case FFT_Dir::iFFT:
                double invNVALs = 1.0 / NVALS;
                for (std::size_t i = 0, j = 0; j < n; ++i, j += 2)
                {
                    data_re[i] = data[j] * invNVALs;
                    data_im[i] = data[j + 1] * invNVALs;
                }
                break;
        }
    }
    return err;
}

/**
 * @brief Performs a 2D (inverse) Fourier transform of a grid
 *
 * @param real_part The real part of the grid to (i)FFT
 * @param imag_part The imaginary part of the grid to (i)FFT
 * @param transform_dir Whether to perform an FFT or an IFFT
 * @return int An error code or 0 if it worked correctly
 */
int FFT_2D(GridObject& real_part, GridObject& imag_part,
                FFT_Dir transform_dir)
{
    int err = 0;

    // assumes real_part and im_part are the same size!
    const std::size_t Nx = real_part.get_Nx();
    const std::size_t Ny = real_part.get_Ny();

    // spectral solve: Fourier transform rows, then columns
    // for each row: collect data, Fourier transform, return, and store
    std::vector<double> xs_re(Ny), xs_im(Ny);

    for (std::size_t xi = 0; xi < Nx; ++xi)
    {
        for (std::size_t yj = 0; yj < Ny; ++yj)
        {
            xs_re[yj] = real_part.get_comp(xi, yj);
            xs_im[yj] = imag_part.get_comp(xi, yj);
        }
        err = FFT_1D(xs_re, xs_im, transform_dir);
        if (err)
        {
            return err;
        }
        for (std::size_t yj = 0; yj < Ny; ++yj)
        {
            real_part.set_comp(xi, yj, xs_re[yj]);
            imag_part.set_comp(xi, yj, xs_im[yj]);
        }
    }

    // now columns
    std::vector<double> ys_re(Nx), ys_im(Nx);
    for (std::size_t yj = 0; yj < Ny; ++yj)
    {
        for (std::size_t xi = 0; xi < Nx; ++xi)
        {
            ys_re[xi] = real_part.get_comp(xi, yj);
            ys_im[xi] = imag_part.get_comp(xi, yj);
        }
        err = FFT_1D(ys_re, ys_im, transform_dir);
        if (err)
        {
            return err;
        }
        for (std::size_t xi = 0; xi < Nx; ++xi)
        {
            real_part.set_comp(xi, yj, ys_re[xi]);
            imag_part.set_comp(xi, yj, ys_im[xi]);
        }
    }

    return err;
}

std::vector<double> get_k_vec(const std::size_t size, const double dx)
{
    std::vector<double> k = std::vector<double>(size);
    const double kmax = M_PI / dx;

    //TODO: verify divide by (size/2) and not just size
    for (std::size_t i = 0; i < size / 2; ++i)
    {
        k[i] = kmax * i / (size / 2);
    }
    for (std::size_t i = size / 2; i < size; ++i)
    {
        k[i] = kmax * i / (size / 2) - 2 * kmax;
    }

    return k;
}

std::vector<double> get_K2_vec(const std::vector<double>& k, const double dx)
{
    std::vector<double> K2 = std::vector<double>(k.size());
    double val;

    for (std::size_t i = 0; i < k.size(); ++i)
    {
        val = k[i] * sinc(k[i] * dx / 2.0);
        K2[i] = val * val;
    }

    return K2;
}

std::vector<double> get_kappa_vec(const std::vector<double>& k, const double dx)
{
    std::vector<double> kappa = std::vector<double>(k.size());

    for (std::size_t i = 0; i < k.size(); ++i)
    {
        kappa[i] = k[i] * sinc(k[i] * dx);
    }

    return kappa;
}
