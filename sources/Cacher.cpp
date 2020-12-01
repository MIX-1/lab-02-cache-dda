// Copyright 2020 MIX-1 <danilonil1@yandex.ru>

#include <Cacher.hpp>

void Cacher::Find_Exp_Size() {
    experiment_size.push_back(Cache_size[0] / 2);
    experiment_size.push_back(Cache_size[2] * 3 / 2);
    int size = pow(2, 0) * Num_b_in_kb;
    for (int i = 1; size < Cache_size[2]; ++i){
        experiment_size.insert(experiment_size.begin()+i, size);
        size = pow(2, i) * Num_b_in_kb;
    }
    num_experiment = experiment_size.size();
    for (int i = 0; i < num_experiment; ++i) {
        std::ostringstream stream;
        if (experiment_size[i] < Num_b_in_kb) {
            stream << experiment_size[i];
            experiment_size_name.push_back(stream.str() + " kb");
        } else {
            stream << experiment_size[i]/Num_b_in_kb;
            experiment_size_name.push_back(stream.str() + " mb");
        }
    }
    for (int i = 0; i < num_experiment; ++i) {
        experiment_size[i] = experiment_size[i] *
                             Num_b_in_kb /
                             Num_byte_in_Long_double;
    }
}

Cacher::Cacher(std::vector<Type> types) {
    Find_Exp_Size();
    duration.resize(Num_Investigation);
    auto arr = new long double[experiment_size.back()];
    for (int i = 0; i < experiment_size.back(); i += Num_Data_In_Line){
        arr[i] = random();
    }
    for (int t = 0; t < 3; ++t){
        switch (types[t]) {
            case Straight:
                experiment_type_name[t] = "Straight";
                for (int j = 0; j < num_experiment; ++j) {
                    long double k = 0;
                    // Warming
                    for (int i = 0;
                    i < experiment_size[j];
                    i += Num_Data_In_Line){
                        k += arr[i];
                    }
                    // Experiment
                    std::chrono::system_clock::time_point start =
                            std::chrono::high_resolution_clock::now();
                    for (int i = 0;
                         i < experiment_size[j] * Num_Repeat;
                         i += Num_Data_In_Line){
                        k += arr[i % experiment_size[j]];
                    }
                    std::chrono::system_clock::time_point end =
                            std::chrono::high_resolution_clock::now();
                    duration.at(t).push_back(static_cast<int>(std::chrono::
                    nanoseconds((end - start)/Num_Repeat).count()));
                }
                break;
            case Reverse:
                experiment_type_name[t] = "Reverse";
                for (int j = 0; j < num_experiment; ++j) {
                    long double k = 0;
                    // Warming
                    for (int i = 0;
                    i < experiment_size[j];
                    i += Num_Data_In_Line){
                        k += arr[i];
                    }
                    // Experiment
                    std::chrono::system_clock::time_point start =
                            std::chrono::high_resolution_clock::now();
                    for (int i = experiment_size[j] * Num_Repeat;
                         i > 0;
                         i -= Num_Data_In_Line){
                        k += arr[i % experiment_size[j]];
                    }
                    std::chrono::system_clock::time_point end =
                            std::chrono::high_resolution_clock::now();
                    duration.at(t).push_back(static_cast<int>(std::chrono::
                    nanoseconds((end - start)/Num_Repeat).count()));
                }
                break;
            case Random:
                experiment_type_name[t] = "Random";
                for (int j = 0; j < num_experiment; ++j) {
                    long double k = 0;
                    std::set<int> used_num;
                    int n = 0;
                    // Warming
                    for (int i = 0;
                    i < experiment_size[j];
                    i += Num_Data_In_Line){
                        k += arr[i];
                    }
                    // Experiment
                    std::chrono::system_clock::time_point start =
                            std::chrono::high_resolution_clock::now();
                    for (int i = 0;
                         i < experiment_size[j] * Num_Repeat;
                         i += Num_Data_In_Line){
                        n = random() % experiment_size[j];
                        while (used_num.find(n) != used_num.end()) {
                            break;
                        }
                        k += arr[n % experiment_size[j]];
                    }
                    std::chrono::system_clock::time_point end =
                            std::chrono::high_resolution_clock::now();
                    duration.at(t).push_back(static_cast<int>(std::chrono::
                    nanoseconds((end - start)/Num_Repeat).count()));
                }
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
                "\n\t\tinput_data:\n\t\t\tbuffer_size: " <<
                cacher.experiment_size_name[j] <<
                "\n\t\tresults:\n\t\t\tduration: " <<
                cacher.duration[i][j] <<
                " nanoseconds\n";
        }
    }
    return tab;
}
