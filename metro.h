//
// Created by oleg on 15.04.2020.
//

#ifndef INC_1_METRO_H
#define INC_1_METRO_H


#include <vector>

    typedef unsigned int uint;
    typedef unsigned long long ull;
    typedef long long ll;

    struct metro_move {
        uint start_station;
        uint finish_station;
        ull duration;
        ull period;
    };

    class metro_map {
    private:
        // metro_moves[i] - возможные переезды из i-й станции
        std::vector<std::vector<metro_move> > directions_;
        int stations_count_;
        const ull INF_ = 1000000000000000000ll;
    public:
        metro_map(std::vector<metro_move> &moves, uint stations_count);

        ll get_fastest_route(uint first_station, uint second_station,
                             ull time, std::vector<uint> &result);
    };
#endif //INC_1_METRO_H
