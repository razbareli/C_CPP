#include "MlpNetwork.h"
#define OUT_LEN 10

//Applies the entire network on input
//returns digit struct
digit MlpNetwork::operator() (const Matrix &img)
{
  digit ans;
  Matrix output = layer4 (layer3 (layer2 (layer1 (img))));
  //find the value with maximum probability
  float max = 0;
  int number = 0;
  for (int i = 0; i < OUT_LEN; ++i)
    {
      if (output[i] > max)
        {
          max = output[i];
          number = i;
        }
    }
  ans.value = number;
  ans.probability = max;
  return ans;
}
