//MlpNetwork.h

#ifndef MLPNETWORK_H
#define MLPNETWORK_H

#include "Matrix.h"
#include "Dense.h"
#include "Digit.h"

#define MLP_SIZE 4

//
const matrix_dims img_dims = {28, 28};
const matrix_dims weights_dims[] = {{128, 784},
                                    {64, 128},
                                    {20, 64},
                                    {10, 20}};
const matrix_dims bias_dims[]    = {{128, 1},
                                    {64, 1},
                                    {20, 1},
                                    {10, 1}};

class MlpNetwork{
private:
    Dense layer1;
    Dense layer2;
    Dense layer3;
    Dense layer4;
public:
    //constructor
    // Accepts 2 arrays, size 4 each.
    //one for weights and one for biases.
    //constructs the network
    MlpNetwork(const Matrix w[], const Matrix b[]):
    layer1(w[0], b[0], RELU),
    layer2(w[1], b[1], RELU),
    layer3(w[2], b[2], RELU),
    layer4(w[3], b[3], SOFTMAX){};

  //Applies the entire network on input
  //returns digit struct
    digit operator()(const Matrix& img);
};
#endif // MLPNETWORK_H
