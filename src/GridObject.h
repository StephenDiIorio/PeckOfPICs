#ifndef GRID_H
#define GRID_H

#include <stdio.h>
#include <vector>

class GridObject 
{
  private:
    std::vector<double> gridded_data;
    void init_grid_obj(std::function<void(GridObject &, uint)> init_fcn);
  public:

  

};
#endif
