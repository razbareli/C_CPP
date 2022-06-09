#ifndef EX6_VL_VECTOR_H
#define EX6_VL_VECTOR_H

#include <iostream>
#include <algorithm>

#define STATIC_CAP 16

//questions: ovveride doesnt work, return const_iterator, cbegin vs begin
//todo check forward iterator, check operator []

template<class T, size_t StaticCapacity = STATIC_CAP>
class vl_vector {

 protected:
  T static_data[StaticCapacity];
  T *dynamic_data;
  size_t _capacity;
  size_t _size;
  bool _is_dynamic;

 public:
  //print function for testing
  void print_vector() const{//todo erase before submission
    if (_is_dynamic){
        std::cout <<"dynamic"<<std::endl;
      }else{
        std::cout <<"static"<<std::endl;
      }
    std::cout <<"capacity: "<<_capacity<<std::endl;
    std::cout <<"size: "<<_size<<std::endl;
    for (int i = 0; i < _size; ++i) {
        if (data()[i] == '\0'){
            std::cout<< '@';
          }
        std::cout<< data()[i];
      }
    std::cout<< std::endl;
  }
  //constructors:

  /**
   * default constructor
   * @brief creates an empty vector
   */
  vl_vector() : _capacity(StaticCapacity),
                _size(0), _is_dynamic(false) {};

  /**
   * copy constructor
   * @brief creates a new vector as a copy of an existing vector
   * @param other: the vector to copy
   */
  vl_vector(const vl_vector<T, StaticCapacity> &other)
      :vl_vector(){
    *this = other;
  }

  /**
   * Sequence based Constructor
   * @brief: initializes a vector with the sequence provided
   * @param first: start of sequence
   * @param last: end of sequence
   */
  template<class InputIterator>
  vl_vector(InputIterator first, InputIterator last):
      _capacity(StaticCapacity), _size(0), _is_dynamic(false){
    size_t num_of_elements = std::distance(first, last);
    if (num_of_elements > _capacity){
        expand(num_of_elements);
      }
    for (InputIterator it = first; it != last ; ++it){
        push_back(*it);
      }
  }

  /**
   * Single-value initialized constructor
   * @param T type parameter
   * @param count - number of items to initialize
   */
  vl_vector(const size_t count, const T value) {
    _capacity = StaticCapacity;
    if (count <= _capacity) {
        _is_dynamic = false;
        for (int i = 0; i < count; ++i) {
            static_data[i] = value;
          }
      } else {
        _is_dynamic = true;
        _size = 0;
        _capacity = cap_c(count);
        dynamic_data = new T[_capacity];
        for (int i = 0; i < count; ++i) {
            dynamic_data[i] = value;
          }
      }
    _size = count;
  }

  /**
   * destructor
   *@brief: frees the dynamic allocated memory
   */
  virtual ~vl_vector() {
    if (_is_dynamic) {
        delete[] dynamic_data;
      }
  }
  //operators:
  /**
   * copy assignment operator
   * @param: vector to copy
   */
  vl_vector& operator=(const vl_vector& other){
    if (this != &other){
        if (other._is_dynamic){
            if (_is_dynamic){
                delete[] dynamic_data;
              }else{
                _is_dynamic = true;
              }
            dynamic_data = new T[other._capacity];
            for (size_t i = 0; i < other._size; ++i) {
                dynamic_data[i] = other.dynamic_data[i];
              }
          }else {
            if (!_is_dynamic) {
                for (size_t i = 0; i < other._size; ++i) {
                    static_data[i] = other.static_data[i];
                  }
              }else{
                delete[] dynamic_data;
                _is_dynamic = false;
              }
          }
        _capacity = other._capacity;
        _size = other._size;
      }
    return *this;
  }
  /**
   * bracket operator non - const version
   * @param: index
   * @return: value in index
   */
  T &operator[] (int ind){
    return data()[ind];
  }
  /**
   * bracket operator const version
   * @param: index
   * @return: value in index
   */
  const T &operator[] (int ind) const{
    return data()[ind];
  }
  /**
   * '==' operator
   * @param: vector to compare
   * @return: true if equal, false if not
   */
  bool operator==(const vl_vector& other) const {
    if (_size == other._size){
        for (size_t i = 0; i < _size; ++i) {
            if (data()[i] != other.data()[i]){
                return false;
              }
          }
        return true;
      }
    return false;
  }
  /**
   * != operator
   * @param: vector to compare
   * @return: true if not equal, false if is
   */
  bool operator!=(const vl_vector& other) const {
    return !(*this == other);
  }

  //iterators:

  /**
   * random access iterator
   */
  typedef T* iterator;
  typedef const T* const_iterator;
//    using iterator = T*;
//    using const_iterator = T const*;
  iterator begin(){return data();}
  iterator end(){return data() + _size;}
  const_iterator begin() const{return data();}
  const_iterator end() const {return data() + _size;}
  const_iterator cbegin() const{return data();}
  const_iterator cend() const{return data() + _size;}

  /**
   * reverse iterator
   */
//     using reverse_iterator = std::reverse_iterator<iterator>;
//     using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
  reverse_iterator rbegin(){return reverse_iterator (end());}
  reverse_iterator rend(){return reverse_iterator (begin());}
  const_reverse_iterator rbegin() const{
    return const_reverse_iterator (end());
  }
  const_reverse_iterator rend() const{
    return const_reverse_iterator (begin());
  }
  const_reverse_iterator crbegin() const{
    return const_reverse_iterator (end());
  }
  const_reverse_iterator crend() const{
    return const_reverse_iterator (begin());
  }
  //methods:
  /**
   * @return: the current size of the vector
   */
  virtual size_t size() const{
    return _size;
  }

  /**
   * @brief: calculates the new capacity of the vector
   * @param: num of items to add
   * @return: new capacity of vector
   */
  size_t cap_c(const size_t k) const{
    return 3 * (_size + k) / 2;
  }

  /**
  * @return: the capacity of the vector
  */
  size_t capacity() const{
    return _capacity;
  }

  /**
   * @return: true if the vector is empty
   * else, returns false
   */
  virtual bool empty() const{
    return _size == 0;
  }

  /**
  * at operator const version
  * @param ind - the index of the value
  * @returns the value at the index
  */
  T at(const size_t ind) const{
    if (ind >= _size) {
        throw std::out_of_range("index out of range");
      }
    return data()[ind];
  }

  /**
  * at operator non const version
  * @param ind - the index of the value
  * @returns the value at the index
  */
  T& at(const size_t ind) {
    if (ind >= _size) {
        throw std::out_of_range("index out of range");
      }
    return data()[ind];
  }

  /**
   * @brief: inserts value at the end of the vector
   * @param value: the value we want to add
   */
  virtual void push_back(const T value) {
    if (_size < _capacity) {
        data()[_size] = value;
        _size++;
      } else { //if we need to expand the vector
        expand(1);
        dynamic_data[_size] = value;
        _size++;
      }
  }
  /** insert 1
   * @brief: insert the value before the iterator position
   * @param position: iterator in position of insertion
   * @param value: value to insert
   * @returns: iterator to the inserted value
   */
  iterator insert(const_iterator position, T value){
    iterator temp = begin();
    size_t ind = 0;
    while (temp != position){
        ind++;
        temp++;
      }
    if (_size == _capacity){
        expand(1);
      }
    for (size_t i = _size; i > ind; --i) {
        data()[i] = data()[i-1];
      }
    data()[ind] = value;
    _size++;
    return data()+ind;
  }
  /** insert 2
   * @brief: insert the sequence before the iterator position
   * @param position: iterator in position of insertion
   * @param first: start of sequence
   * @param last: end of sequence
   * @returns: iterator to the inserted value
   */
  template<class InputIterator>
  iterator insert(const_iterator position,
                  InputIterator first, InputIterator last) {
    iterator temp = begin();
    size_t ind = 0;
    while (temp != position){
        ind++;
        temp++;
      }
    size_t values_to_move = _size - ind;
    size_t values_to_insert = std::distance(first, last);
    if (_size + values_to_insert > _capacity){
        expand(values_to_insert);
      }
    for (size_t i = 0; i < values_to_move; ++i) {
        data()[_size-1+values_to_insert-i] = data()[_size-1-i];
      }
    for (size_t i = ind; i < ind+values_to_insert; ++i) {
        data()[i] = *first;
        first++;
      }
    _size = _size+values_to_insert;
    return data()+ind;
  }
  /**
   * @brief: removes the last element in the vector
   */
  virtual void pop_back(){
    if (_size == 0){return;}
    _size--;
    if (_is_dynamic && _size <= StaticCapacity){
        move_to_static();
      }
  }

  /** erase(1)
   * @param position: the value to be deleted
   * @return iterator to the right of the deleted element
   */
  iterator erase(const_iterator position) {
    size_t return_ind = std::distance(cbegin(), position);
    size_t values_to_move = std::distance(position, cend());
    for (size_t i = 0; i < values_to_move; ++i)
      {
        data()[return_ind+i]=data()[return_ind+i+1];
      }
    _size--;
    if (_is_dynamic && _size <= StaticCapacity){
        move_to_static();
      }
    return &data()[return_ind];
  }

  /**
   * erase(2)
   * @param first iterator
   * @param last iterator
   * @return iterator to the right of the deleted element
   */
  iterator erase(const_iterator first, const_iterator last){
    size_t values_to_delete = std::distance(first, last);
    size_t return_ind = std::distance(cbegin(), first);
    size_t values_to_move = std::distance(last, cend());
    for (size_t i = 0; i < values_to_move; ++i)
      {
        data()[return_ind+i]=data()[return_ind+i+values_to_delete];
      }
    _size -= values_to_delete;
    if (_is_dynamic && _size <= StaticCapacity){
        move_to_static();
      }
    return &data()[return_ind];
  }

  /**
   * @brief: deletes all elements in the vector
   */
  virtual void clear(){
    if (_is_dynamic){
        delete[] dynamic_data;
        _is_dynamic = false;
      }
    _size = 0;
    _capacity = StaticCapacity;
  }
  /**
   * @returns: a pointer to the array holding the data - const version
   */
  const T* data() const{ //todo check why the first const is needed
    if (_is_dynamic){
        return  dynamic_data;
      }
    return static_data;
  }

  /**
  * @returns: a pointer to the array holding the data - non-const version
  */
  T* data(){
    if (_is_dynamic){
        return  dynamic_data;
      }
    return static_data;
  }

  /**
   *
   * @param value: the value to look for
   * @return true or false
   */
  virtual bool contains(const T& value) const{
    for (size_t i = 0; i < _size; ++i)
      {
        if (data()[i] == value){
            return true;
          }
      }
    return false;
  }

  //my functions
  /**
   * expands the vector capacity
   * @param k: number of items to insert
   */
  void expand(size_t k){
    size_t new_cap = cap_c(k);
    _capacity = new_cap;
    T *temp = new T[new_cap]; //todo check alloc failure
    for (size_t i = 0; i < _size; ++i) {
        temp[i] = data()[i];
      }
    if (_is_dynamic)//delete the previous dynamic array
      {
        delete[] dynamic_data;
      } else {
        _is_dynamic = true;
      }
    dynamic_data = temp;
  }
  /**
   * copies the data to the static memory
   * and deletes the allocated data
   */
  void move_to_static(){
    for (size_t i = 0; i < _size; ++i){
        static_data[i] = dynamic_data[i];
      }
    delete [] dynamic_data;
    _is_dynamic = false;
    _capacity = StaticCapacity;
  };
};

#endif //EX6_VL_VECTOR_H