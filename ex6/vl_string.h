#ifndef _VL_STRING_H_
#define _VL_STRING_H_
#include "vl_vector.h"
#include <cstring>

template<size_t StaticCapacity = STATIC_CAP>
class vl_string : public vl_vector<char, StaticCapacity> {
 public:
  /**
     * default constructor
     * @brief creates an empty string
     */
  vl_string () : vl_vector<char, StaticCapacity> ()
  {
    if (this->_capacity > 0)
      {
        this->static_data[0] = '\0';
      }
    else
      {
        this->expand (1);
        this->dynamic_data[0] = '\0';
      }
    this->_size++;
  }
  /**
     * copy constructor
     * @brief creates a new string as a copy of an existing string
     * @param other: the vector to copy
     */
  vl_string (const vl_string<StaticCapacity> &other) : vl_string ()
  {*this = other;}

  /**
   * implicit constructor
   * constructs a string vector from an implicitly given string
   * @param str: pointer to an array of chars
   */
  vl_string (const char *str) : vl_vector<char, StaticCapacity>
                                    (str, str + strlen (str) + 1){}

  /**
  * @return: the current size of the string
  */
  size_t size() const override{
    return this->_size-1;
  }
  /**
     *@brief: checks if a string is a substring of the vl_string
     * @param value: the value to look for
     * @return true or false
     */
  bool contains (const char* value) const {
    return strstr (this->data (), value) != nullptr;
  }

  //operator += vl_vector concatenation
  vl_string& operator+=(const vl_string<StaticCapacity>& other){
    if (this->_size + other._size > this->_capacity){
        this->expand(other.size());
      }
    char* new_string = strcat(this->data(), other.data());
    this->_size += other.size();
    for (size_t i = 0; i < this->_size; ++i)
      {
        this->data()[i] = new_string[i];
      }
    return *this;
  }
  //operator += char* concatenation
  vl_string& operator+=(const char* str){
    vl_string<StaticCapacity> temp(str);
    *this += temp;
    return *this;
  }

  //operator += char concatenation
  vl_string& operator+=(const char c)
  {
    this->push_back(c);
    return *this;
  }

  /**
     * @brief: inserts value at the end of the vector
     * @param value: the value we want to add
     */
  void push_back(const char value) override{
    if (this->_size == this->_capacity){
        this->expand (1);
      }
    this->data()[this->_size-1] = value;
    this->data()[this->_size] = '\0';
    this->_size++;
  }

  /**
     * @brief: removes the last char in the string
     */
  void pop_back() override{
    if (this->_size == 1){return;}
    this->_size--;
    if (this->_is_dynamic && this->_size <= StaticCapacity){
        this->move_to_static();
      }
    this->data()[this->_size-1] = '\0';
  }

  //operator '+' for vl_string
  vl_string operator+(const vl_string<StaticCapacity>& other) const{
    vl_string<StaticCapacity> ans = *this;
    ans += other;
    return ans;
  }
  //operator '+' for char*
  vl_string operator+(const char* str) const{
    vl_string<StaticCapacity> ans = *this;
    ans += str;
    return ans;
  }
  //operator '+' for char
  vl_string operator+(const char c) const{
    vl_string<StaticCapacity> ans = *this;
    ans += c;
    return ans;
  }

  /**
  * @return: true if the vector is empty
  * else, returns false
  */
  bool empty() const override{
    return this->_size == 1;
  }

  //Implicit casting opreator
  operator char* () {
    return (char*)this->data();
  }

  //Implicit casting opreator const version
  operator char* () const{
    return (char*)this->data();
  }

  /** clear
    * @brief: deletes all elements in the vector
    */
  void clear() override{
    if (this->_is_dynamic){
        delete[] this->dynamic_data;
        this->_is_dynamic = false;
      }
    this->_size = 1;
    this->_capacity = StaticCapacity;
    this->data()[0] = '\0';
  }
};


#endif //_VL_STRING_H_
