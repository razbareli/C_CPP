// Matrix.h

#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
/**
 * @struct matrix_dims
 * @brief Matrix dimensions container.
 * Used in MlpNetwork.h and main.cpp
 */
typedef struct matrix_dims {
    int rows, cols;
} matrix_dims;

class Matrix {
 private:
  int num_of_rows, num_of_cols;
  float *mat;

 public:
/**
 * constructor
 * Constructs Matrix rows × col, Inits all elements to 0
 */
  Matrix (int rows, int cols) : num_of_rows (rows),
  num_of_cols (cols)
  {
    mat = new (std::nothrow) float[num_of_rows * num_of_cols];
    if(mat == nullptr){
      std::cerr << "Error: Allocation problem in constructor";
      exit (EXIT_FAILURE);
    }
    int size = num_of_cols * num_of_rows;
    for (int i = 0; i < size; ++i)
      {
        mat[i] = 0;
      }
  }
  /**
   * default constructor
   * Constructs 1×1 Matrix
   * Inits the single element to 0
   */
  Matrix () : Matrix (1, 1)
  {}
  //copy constructor
  Matrix (const Matrix &other) : num_of_rows (other.num_of_rows),
                                 num_of_cols (other.num_of_cols)
  {
    mat =  new float[other.num_of_rows
                   * other.num_of_cols];
    if(mat == nullptr){
        std::cerr << "Error: Allocation problem in copy constructor";
        exit (EXIT_FAILURE);
      }
    int size = num_of_cols * num_of_rows;
    for (int i = 0; i < size; ++i)
      {
        this->mat[i] = other.mat[i];
      }
  }
  /**
   * destructor
   */
  ~Matrix ()
  {
    delete[] mat;
  }
  /**
   * getters
   * returns the amount of cols / rows as ints
   */
  int get_rows () const;
  int get_cols () const;
  //functions
  /**
   * Transforms a matrix into its transpose
   * matrix.
   */
  Matrix &transpose ();
  /**
   * Transforms a matrix into a column
   * vector
   */
  Matrix &vectorize ();
  /**
   * Prints matrix elements, no return
   * value.
   * Prints space after each element
   */
  void plain_print () const;

  /**
   * Returns a matrix which is the dot
   * product of this matrix and another
   * matrix m:
   */
  Matrix dot (const Matrix &other) const;

  /**
   * Returns the Frobenius norm of the
   * given matrix
   */
  float norm () const;

  //operators

  //adds two matrices
  Matrix operator+ (const Matrix &other) const;
  //copy assignment operator
  Matrix &operator= (const Matrix &other);
  //multiplication of two matrices
  friend Matrix operator* (const Matrix &lhs, const Matrix &rhs);
  //scalar mult. from the right
  friend Matrix operator* (const Matrix &m, float x);
  //scalar mult. from the left
  friend Matrix operator* (float x, const Matrix &m);
  //adds to this matrix the value in the other matrix. cell by cell
  Matrix &operator+= (const Matrix &vec);
  //Parenthesis indexing
  float &operator() (int row, int col);
  //Parenthesis indexing const version
  const float &operator() (int row, int col) const;
  //bracketing indexing
  float &operator[] (int ind);
  //bracketing indexing const version
  const float &operator[] (int ind) const;
  //pretty export of matrix
  friend std::ostream &operator<< (std::ostream &os, const Matrix &m);
  //reads from binary file into a matrix
  friend std::istream &read_binary_file (std::istream &is, const Matrix &m);
};

#endif //MATRIX_H

