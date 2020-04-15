//
// Created by oleg on 15.04.2020.
//
#include "metro.h"
#include <stdexcept>
#include <set>

    metro_map::metro_map(std::vector<metro_move> &moves, uint stations_count) :
            stations_count_(stations_count),
            directions_(stations_count, std::vector<metro_move>(0)) {
        for (uint i = 0; i < moves.size(); ++i) {
            if (moves[i].start_station < 0 || moves[i].start_station >= stations_count) {
                throw std::out_of_range("start station");
            }

            if (moves[i].finish_station < 0 || moves[i].finish_station >= stations_count) {
                throw std::out_of_range("finish station");
            }

            directions_[moves[i].start_station].push_back(moves[i]);
        }
    }

    ll metro_map::get_fastest_route(uint start_station, uint finish_station,
                                    ull time, std::vector<uint> &result) {
        if (start_station < 0 || start_station >= stations_count_) {
            throw std::out_of_range("start station");
        }

        if (finish_station < 0 || finish_station >= stations_count_) {
            throw std::out_of_range("end station");
        }

        std::set<std::pair<int, int> > unreached_stations;
        std::vector<ull> reaching_times(stations_count_, INF_);
        std::vector<int> parents(stations_count_, -1);
        std::vector<int> visited(stations_count_, 0);

        for (uint i = 0; i < stations_count_; ++i) {
            std::pair <int, int> inserted;
            if (i != start_station) {
                unreached_stations.insert({INF_, i});
            } else {
                unreached_stations.insert({0, i});
            }
        }

        reaching_times[start_station] = time;
        bool finish_reached = false;
        while (!unreached_stations.empty() && !finish_reached) {
            auto reached = unreached_stations.begin();
            if (reached->first == INF_) {
                break;
            }

            uint station = reached->second;
            if (station == finish_station) {
                finish_reached = true;
            } else {
                unreached_stations.erase(reached);
                visited[station] = true;

                for (int i = 0; i < directions_[station].size(); ++i)
                    if (!visited[directions_[station][i].finish_station]) {
                        ull departure_time = 0;
                        if (directions_[station][i].period == 0 ||
                            reaching_times[station] % directions_[station][i].period == 0) {
                            departure_time = reaching_times[station];
                        } else {
                            departure_time = reaching_times[station] -
                                             reaching_times[station] % directions_[station][i].period +
                                             directions_[station][i].period;
                        }
                        if (departure_time + directions_[station][i].duration <
                            reaching_times[directions_[station][i].finish_station]) {
                            parents[directions_[station][i].finish_station] = station;
                            unreached_stations.erase({reaching_times[directions_[station][i].finish_station],
                                                      directions_[station][i].finish_station});
                            reaching_times[directions_[station][i].finish_station] =
                                    departure_time + directions_[station][i].duration;
                            unreached_stations.insert({reaching_times[directions_[station][i].finish_station],
                                                       directions_[station][i].finish_station});
                        }
                    }
            }
        }
        result.clear();
        if (finish_reached) {
            int current_station = finish_station;
            while (current_station != -1) {
                result.push_back(current_station);
            }
            return reaching_times[finish_station];
        } else {
            return -1;
        }
    }