
#include "Activation.h"
#include <cmath>

//Applies activation function on input.
//Does not change input.
Matrix Activation::operator() (const Matrix &input) const
{
  if (type == 0)
    {
      //do relu
      Matrix output = Matrix (input.get_rows (), input.get_cols ());
      for (int i = 0; i < output.get_rows () * output.get_cols (); ++i)
        {
          if (input[i] >= 0)
            {
              output[i] = input[i];
            }
        }
      return output;
    }
  else
    {
//do softmax
      Matrix output = Matrix (input.get_rows (), input.get_cols ());
      float temp;
      float sum = 0;
      for (int i = 0; i < input.get_rows () * input.get_cols (); ++i)
        {
          temp = std::exp (input[i]);
          output[i] = temp;
          sum += temp;
        }
      float final_sum = 1 / sum;
      for (int j = 0; j < output.get_rows () * output.get_cols (); ++j)
        {
          temp = output[j];
          output[j] = final_sum * temp;
        }
      return output;
    }
}
