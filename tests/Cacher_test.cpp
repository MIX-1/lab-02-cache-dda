// Copyright 2020 MIX-1 <danilonil1@yandex.ru>

#include <Cacher.hpp>
#include <gtest/gtest.h>

TEST(Cacher, Start){
    std::vector<Type> types = {Straight, Reverse, Random};
    Cacher c(types);
    cout << c;
}
