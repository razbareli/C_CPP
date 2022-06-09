//Activation.h
#ifndef ACTIVATION_H
#define ACTIVATION_H

#include "Matrix.h"
/**
 * @enum ActivationType
 * @brief Indicator of activation function.
 */
enum ActivationType {
    RELU,
    SOFTMAX
};

class Activation {
 private:
  ActivationType type;

 public:
  //constructor: Accepts an ActivationType enum with
  //one of two legal values:
  //RELU/SOFTMAX.
  //Defines this instance's activation
  //accordingly
  Activation (ActivationType act_type) : type (act_type)
  {};

  //Returns this activation's type
  ActivationType get_activation_type () const
  { return type; };

  //Applies activation function on input.
  //Does not change input.
  Matrix operator() (const Matrix &input) const;
};
#endif //ACTIVATION_H
