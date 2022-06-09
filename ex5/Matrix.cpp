#include "Matrix.h"
#include <iostream>
#include <cmath>

#define DARK 0.1

//functions
/**
   * getters
   * returns the amount of cols / rows as ints
   */
int Matrix::get_rows () const
{
  return num_of_rows;
}

int Matrix::get_cols () const
{
  return num_of_cols;
}
/**
   * Transforms a matrix into its transpose
   * matrix.
   */
Matrix &Matrix::transpose ()
{
  Matrix temp_mat = *this;
  Matrix new_mat = Matrix (num_of_cols, num_of_rows);
  for (int i = 0; i < temp_mat.get_rows (); ++i)
    {
      for (int j = 0; j < temp_mat.get_cols (); ++j)
        {
          new_mat (j, i) = temp_mat (i, j);
        }
    }
  *this = new_mat;
  return *this;
}
/**
   * Transforms a matrix into a column
   * vector
   */
Matrix &Matrix::vectorize ()
{
  num_of_rows = num_of_rows * num_of_cols;
  num_of_cols = 1;
  return *this;
}
/**
   * Prints matrix elements, no return
   * value.
   * Prints space after each element
   */
void Matrix::plain_print () const
{
  for (int i = 0; i < num_of_rows; ++i)
    {
      for (int j = 0; j < num_of_cols; ++j)
        {
          std::cout << this->mat[i * num_of_cols + j] << " ";
        }
      std::cout << std::endl;
    }
}
/**
   * Returns a matrix which is the dot
   * product of this matrix and another
   * matrix m:
   */
Matrix Matrix::dot (const Matrix &other) const
{
  if (num_of_rows != other.num_of_rows ||
      num_of_cols != other.num_of_cols)
    {
      std::cerr << "Error: dot product failure";
      exit (EXIT_FAILURE);
    }
  Matrix dot_mat (num_of_rows, num_of_cols);
  for (int i = 0; i < num_of_rows; ++i)
    {
      for (int j = 0; j < num_of_cols; ++j)
        {
          float a = this->mat[i * num_of_cols + j];
          float b = other.mat[i * num_of_cols + j];
          dot_mat.mat[i * num_of_cols + j] = a * b;
        }
    }
  return dot_mat;
}
/**
   * Returns the Frobenius norm of the
   * given matrix
   */
float Matrix::norm () const
{
  float sum = 0;
  for (int i = 0; i < num_of_cols * num_of_rows; ++i)
    {
      sum += ((this->mat[i]) * (this->mat[i]));
    }
  float ans = std::sqrt (sum);
  return ans;
}

//operators
Matrix Matrix::operator+ (const Matrix &other) const
{
  if (this->num_of_rows != other.get_rows() ||
      this->num_of_cols != other.get_cols())
    {
      std::cerr << "Error: matrix addition failure";
      exit (EXIT_FAILURE);
    }
  Matrix new_mat (this->num_of_rows, this->num_of_cols);
  for (int i = 0; i < new_mat.num_of_rows * new_mat.num_of_cols; ++i)
    {
      new_mat.mat[i] = (this->mat[i] + other[i]);
    }
  return new_mat;
}

Matrix &Matrix::operator= (const Matrix &other)
{
  if (this == &other)
    {
      return *this;
    }
  delete[] this->mat;
  this->num_of_rows = other.num_of_rows;
  this->num_of_cols = other.num_of_cols;
  this->mat = new float[other.num_of_cols * other.num_of_rows];
  if(this->mat == nullptr){
      std::cerr << "Error: Allocation problem in '=' operator";
      exit (EXIT_FAILURE);
    }
  for (int i = 0; i < this->num_of_cols * this->num_of_rows; ++i)
    {
      this->mat[i] = other.mat[i];
    }
  return *this;
}

Matrix operator* (const Matrix &lhs, const Matrix &rhs)
{
  if (lhs.num_of_cols != rhs.num_of_rows)
    {
      std::cerr << "Error: matrix multiplication failure";
      exit (EXIT_FAILURE);
    }
  Matrix new_mat (lhs.num_of_rows, rhs.num_of_cols);
  for (int i = 0; i < lhs.num_of_rows; ++i)
    {
      for (int j = 0; j < rhs.num_of_cols; ++j)
        {
          for (int k = 0; k < lhs.num_of_cols; ++k)
            {
              new_mat (i, j) += (lhs (i, k) * rhs (k, j));
            }
        }
    }
  return new_mat;
}

Matrix operator* (const Matrix &m, const float x)
{
  Matrix new_mat (m.num_of_rows, m.num_of_cols);
  for (int i = 0; i < new_mat.num_of_cols * new_mat.num_of_rows; ++i)
    {
      new_mat.mat[i] = x * m.mat[i];
    }
  return new_mat;
}

Matrix operator* (const float x, const Matrix &m)
{
  Matrix new_mat (m * x);
  return new_mat;
}

Matrix &Matrix::operator+= (const Matrix &m)
{
  if (num_of_rows != m.num_of_rows ||
      num_of_cols != m.num_of_cols)
    {
      std::cerr << "Error: operator += failure";
      exit (EXIT_FAILURE);
    }
  for (int i = 0; i < m.num_of_cols * m.num_of_rows; ++i)
    {
      this->mat[i] += m.mat[i];
    }
  return *this;
}

float &Matrix::operator() (const int row, const int col)
{
  if (num_of_rows - 1 < row || num_of_cols - 1 < col)
    {
      std::cerr << "Error: operator () out of range";
      exit (EXIT_FAILURE);
    }
  return mat[row * num_of_cols + col];
}

const float &Matrix::operator() (const int row, const int col) const
{
  if (num_of_rows - 1 < row || num_of_cols - 1 < col)
    {
      std::cerr << "Error: operator () out of range";
      exit (EXIT_FAILURE);
    }
  return mat[row * num_of_cols + col];
}

float &Matrix::operator[] (const int ind)
{
  if (ind > (num_of_rows * num_of_cols) - 1)
    {
      std::cerr << "Error: operator [] out of range";
      exit (EXIT_FAILURE);
    }
  return mat[ind];
}

const float &Matrix::operator[] (const int ind) const
{
  if (ind > (num_of_rows * num_of_cols) - 1)
    {
      std::cerr << "Error: operator [] out of range";
      exit (EXIT_FAILURE);
    }
  return mat[ind];
}

std::ostream &operator<< (std::ostream &os, const Matrix &m)
{
  for (int i = 0; i < m.num_of_rows; ++i)
    {
      for (int j = 0; j < m.num_of_cols; ++j)
        {
          if (m (i, j) >= DARK)
            {
              os << "  ";
            }
          else
            {
              os << "**";
            }
        }
      os << std::endl;
    }
  return os;
}

std::istream &read_binary_file (std::istream &is, const Matrix &m)
{
  is.read ((char *) &m[0], 4 * m.get_rows () * m.get_cols ());
  if (!is.good ())
    {
      std::cerr << "Error: problem with "
                   "reading binary file" << std::endl;
      return is;
    }
  return is;
}






