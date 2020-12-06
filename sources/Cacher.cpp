// Copyright 2020 MIX-1 <danilonil1@yandex.ru>

#include <Cacher.hpp>

void Cacher::Find_Exp_Size() {
  std::vector<int> experiment_size;
  experiment_size.push_back(Cache_size[0] / Formul_Num[0]);
  experiment_size.push_back(Cache_size[2] * Formul_Num[1]);
  int size = static_cast<int>(pow(2, 0) * Num_b_in_kb);
  for (int i = 1; size < Cache_size[2]; ++i){
    experiment_size.insert(experiment_size.begin()+i, size);
    size = static_cast<int>(pow(2, i) * Num_b_in_kb);
  }
  num_experiment = experiment_size.size();
  l1_experiment_size = Cache_size[0] * Num_b_in_kb / Num_byte_in_Long_double;
}

Cacher::Cacher(std::vector<Type> types) {
  Find_Exp_Size();
  duration.resize(Num_Investigation);
  auto arr = new long double[l1_experiment_size];
  long double k = 0;
  for (int i = 0; i < l1_experiment_size; i += Num_Data_In_Line){
    arr[i] = random();
  }
  // Warming
  for (int i = 0; i < l1_experiment_size; i += Num_Data_In_Line){
    k += arr[i];
  }
  for (int t = 0; t < 3; ++t){
    switch (types[t]) {
      case Straight:
        Straight_Experiment(t, arr);
        break;
      case Reverse:
        Reverse_Experiment(t, arr);
        break;
      case Random:
        Random_Experiment(t, arr);
        break;
    }
  }
  delete [] arr;
}

std::ostream& operator<<(std::ostream& tab, const Cacher& cacher) {
  for (int i = 0; i != Num_Investigation; ++i) {
    tab << "investigation:\n\t travel_variant: " <<
        cacher.experiment_type_name[i] << "\n\t experiments:\n";
    for (int j = 0; j != cacher.num_experiment; ++j) {
      tab << "\t- do_experiment:\n\t\tnumber: " << j+1 <<
          "\n\t\tresults:\n\t\t\tduration: " <<
          cacher.duration[i][j] <<
          " nanoseconds\n";
    }
  }
  return tab;
}

void Cacher::Straight_Experiment(int t,  const long double arr[]) {
  experiment_type_name[t] = "Straight";
  for (int j = 0; j < num_experiment; ++j) {
    long double k = 0;
    // Experiment
    std::chrono::system_clock::time_point start =
        std::chrono::high_resolution_clock::now();
    for (int i = 0;
         i < l1_experiment_size * Num_Repeat;
         i += Num_Data_In_Line) {
      k += arr[i % l1_experiment_size];
    }
    std::chrono::system_clock::time_point end =
        std::chrono::high_resolution_clock::now();
    duration.at(t).push_back(static_cast<int>(std::chrono::
    nanoseconds((end - start)/Num_Repeat).count()));
  }
}

void Cacher::Reverse_Experiment(int t, const long double arr[]) {
  experiment_type_name[t] = "Reverse";
  for (int j = 0; j < num_experiment; ++j) {
    long double k = 0;
    // Experiment
    std::chrono::system_clock::time_point start =
        std::chrono::high_resolution_clock::now();
    for (int i = l1_experiment_size * Num_Repeat;
         i > 0;
         i -= Num_Data_In_Line) {
      k += arr[i % l1_experiment_size];
    }
    std::chrono::system_clock::time_point end =
        std::chrono::high_resolution_clock::now();
    duration.at(t).push_back(static_cast<int>(std::chrono::
    nanoseconds((end - start)/Num_Repeat).count()));
  }
}

void Cacher::Random_Experiment(int t, const long double arr[]) {
  experiment_type_name[t] = "Random";
  for (int j = 0; j < num_experiment; ++j) {
    long double k = 0;
    std::set<int> used_num;
    int n = 0;
    // Experiment
    std::chrono::system_clock::time_point start =
        std::chrono::high_resolution_clock::now();
    for (int i = 0;
         i < l1_experiment_size * Num_Repeat;
         i += Num_Data_In_Line){
      n = random() % l1_experiment_size;
      while (used_num.find(n) != used_num.end()) {
        break;
      }
      k += arr[n % l1_experiment_size];
    }
    std::chrono::system_clock::time_point end =
        std::chrono::high_resolution_clock::now();
    duration.at(t).push_back(static_cast<int>(std::chrono::
    nanoseconds((end - start)/Num_Repeat).count()));
  }
}
