#ifndef C___PROJECT_DENSE_H
#define C___PROJECT_DENSE_H

#include "Activation.h"

class Dense {
 private:
  Matrix weight;
  Matrix bias;
  Activation act;
 public:
  //constructor

  Dense (const Matrix &w, const Matrix &b,
         const ActivationType a) : weight (w), bias (b), act (Activation(a))
  {};

  //Returns the weights of this layer
  Matrix get_weights () const
  { return weight; };
  //Returns the bias of this layer
  Matrix get_bias () const
  { return bias; };
  //Returns the activation function of this
  //layer
  Activation get_activation () const
  { return act; };
  //Applies the layer on input and returns
  //output matrix
  Matrix operator() (const Matrix &other) const;

};
#endif //C___PROJECT_DENSE_H
