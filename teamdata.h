#ifndef _TEAMDATA_H_
#define _TEAMDATA_H_

#include <string>
#include <utility>
#include "priorityqueue.h"
#include "json.hpp"

class TeamData {
 public:
    TeamData(std::string filename); // input from json to class
    int numPlayers() const;
    //double winPercentages(int i, int j) const;
    std::pair<int, int> extract();
    void optimizeTeams(int i, int j);

 private:
    // TODO: decide the type of this variable.
    PriorityQueue winPercentages_; // modify the existing file for this so that the key would be an array of size 2
    size_t max_teams_;
    size_t numPlayers_; // straight from the file

};  // class TeamData

#endif  // _TEAMDATA_H_
