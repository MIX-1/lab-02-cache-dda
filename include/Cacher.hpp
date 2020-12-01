// Copyright 2020 MIX-1 <danilonil1@yandex.ru>

#ifndef INCLUDE_CACHER_HPP_
#define INCLUDE_CACHER_HPP_
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <set>
#include <cmath>
#include <sstream>

using std :: cout;
using string = std :: string;

const int Cache_size[3] = {256, 1024, 8192};
const int Num_Investigation = 3;
const int Num_Repeat = 1000;
const int Num_byte_in_Long_double = 16;
const int Num_b_in_kb = 1024;
const int Num_Data_In_Line = 4;

enum Type {Straight, Reverse, Random};

class Cacher {
 public:
  explicit Cacher(std::vector<Type>);

  void Find_Exp_Size();

  friend std::ostream& operator<<(std::ostream&, const Cacher&);

 private:
    int num_experiment = 0;
    std::vector<std::vector<int>> duration;
    std::vector<int> experiment_size;
    std::vector<string> experiment_size_name;
    string experiment_type_name[Num_Investigation];
};

#endif // INCLUDE_CACHER_HPP_
