#include "roulette.hpp"
#include <random>
#include <chrono>

std::mt19937 roulette::gen(time(nullptr));

void roulette::add_sector(float probability, vertex *vrtx) {
    m_weel.push_back(std::make_pair(vrtx, (total_probability += probability) ));
}

vertex *roulette::spin_and_win() {
    std::uniform_real_distribution<float> urd(0.f, 100.f);
    float rand_n = urd(gen);

    for(std::vector<std::pair<vertex *, float>>::iterator sector = m_weel.begin(); sector != m_weel.end(); sector++){
        if(rand_n <= sector->second){
            return sector->first;
        }
    }
    return nullptr;
}
