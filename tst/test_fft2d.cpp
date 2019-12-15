#include "../src/Field.h"


#include <stdlib.h>    /* for exit */
#include <getopt.h>

int main(int argc, char **argv)
{

  int opt;
  int Nx = 4, Ny = 4;
  int mx = 1, my = 1;
  double L_x = 1., L_y = 1.;

  std::string test_options;
  while ((opt = getopt(argc, argv, "n:m:a:b:x:y:")) != -1) {
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
      case 'a':
      {
        mx = atoi(optarg);
      }
      break;
      case 'b':
      {
        my = atoi(optarg);
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
      default: /* '?' */
        fprintf(stderr, "Usage:  [-n number steps][-dt step][-q charge]");
        exit(EXIT_FAILURE);
    }
  }
  double dx = L_x/Nx, dy = L_y/Ny;



/*  // test 1d FFT
  std::vector<double> real(Nx), imag(Nx);
  
  for (int ii = 0; ii < Nx; ++ii)
  {
    real.at(ii) = cos(2*M_PI/Nx*ii);
    std::cout << real.at(ii) << " ";
  }
  std::cout << std::endl;

  int err = FFT(real, imag, Nx, 1);
  std::cout << "real fft" << std::endl;
  for (int ii = 0; ii < Nx; ++ii) std::cout << real.at(ii)<< " ";
  std::cout << std::endl;
  std::cout << "imag fft" << std::endl;
  for (int ii = 0; ii < Nx; ++ii) std::cout << real.at(ii)<< " ";
  std::cout << std::endl;

  err = FFT(real,imag,Nx,-1);
  std::cout << "real post ifft" << std::endl;
  for (int ii = 0; ii < Nx; ++ii) std::cout << real.at(ii)<< " ";
  std::cout << std::endl;
  std::cout << "imag post ifft" << std::endl;
  for (int ii = 0; ii < Nx; ++ii) std::cout << real.at(ii)<< " ";
  std::cout << std::endl;
*/ //end 1d fft test

  // 2d fft test
  Field myfield(Nx, Ny, dx, dy);

  GridObject real(Nx,Ny), imag(Nx,Ny);

  double k_x = mx*2*M_PI/L_x, k_y = my*2*M_PI/L_y;

  for (int ii = 0; ii < Nx; ++ii)
  {
    for (int jj = 0; jj < Ny; ++jj)
    {
      real.set_comp(ii,jj,sin(k_x*ii*dx)*sin(k_y*jj*dy));
    }
  }
  std::cout << "pre transform data" << std::endl;
  real.print();

  int err = myfield.FFT_2d(real,imag,1);
  if (err)
  {
    std::cout << "Problem with fft!" << std::endl;
  }

  std::cout << "Real FFT:" <<std::endl;
  real.print();
  std::cout << "Imaginary FFT:"<<std::endl;
  imag.print();

  err = myfield.FFT_2d(real,imag,-1);
  if (err)
  {
    std::cout << "Problem with fft!" << std::endl;
  }

  std::cout << "Real post iFFT:" <<std::endl;
  real.print();
  std::cout << "Imaginary post iFFT:"<<std::endl;
  imag.print();

  

  return 0;
}