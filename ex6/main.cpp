#include <iostream>
#include <vector>
#include "vl_vector.h"
#include "vl_string.h"
#include "helpers.h"
#include <string>

using std::string;

int main() {

//    vl_vector<int, 5> vec1{};
//    std::vector<int> values{1, 2, 3, 4};
//    auto it1 = values.begin();
//    auto it2 = values.end();
//    vl_vector<int, 5> vec2(it1, it2);
//    vec1.print_vector();
//    vec2.print_vector();
//    vec1 = vec2;
//    vec1.print_vector();
//
//  vl_string<4>st1("Hel");
//  st1.print_vector();
//  std::cout<<"st1 size: "<< st1.size()<<std::endl;
//
//  vl_string<4>st2("lo");
//  st2.print_vector();
//  std::cout<<"st2 size: "<< st2.size()<<std::endl;
//  st1+=st2;
//  st1.print_vector();
  vl_vector<int> v_str (20UL, 5);

  for (int i = 0; i < 5; ++i)
    {
      auto it = v_str.erase (v_str.cbegin ());
    }
  return 0;
}
