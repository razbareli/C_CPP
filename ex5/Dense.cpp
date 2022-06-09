
#include "Dense.h"
//Applies the layer on input and returns
//output matrix
Matrix Dense::operator() (const Matrix &other) const
{
  return act((weight*other)+bias);
}




