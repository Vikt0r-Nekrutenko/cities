#include "roulette.hpp"
#include <algorithm>
#include <random>
#include <chrono>
#include <iostream>

std::mt19937 roulette::gen;

void roulette::add_sector(float probability, vertex *vrtx) {
    push_back(make_pair(vrtx, (total_probability += probability) ));
//    std::cout << vrtx->back().word << (vrtx->back().word.length() > 7 ? "\t\t" : "\t\t\t") << probability << " " << total_probability << std::endl;
}

vertex *roulette::spin_and_win() {
    std::uniform_real_distribution<float> urd(0.f, 100.f);
    float rand_n = urd(gen);

    for(std::vector<std::pair<vertex*, float>>::iterator sector = begin(); sector != end(); sector++){
        if(rand_n <= sector->second){
            return sector->first;
        }
    }
    return nullptr;
}
