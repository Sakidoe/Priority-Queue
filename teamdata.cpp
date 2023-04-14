#include <fstream>
#include <iostream>
#include <cstdlib> // abs()
#include <utility>
#include <string>

#include "json.hpp"
#include "teamdata.h"
#include "priorityqueue.h"

using namespace std;

void checkFile (ifstream& file, string fileName) {
    if (!file) { // check if file is good
        cerr << "Error: cannot open file " << fileName << endl;
        exit(1);
    }
}

size_t NCR (int n, int r) {
    if (r > n / 2) return NCR(n, n - r);

    size_t res = 1;

    for (int k = 1; k <= r; ++k)
    {
        res *= n - k + 1;
        res /= k;
    }

    return res;
}

TeamData::TeamData(string filename) {
    ifstream file;

    file.open(filename);
    checkFile(file, filename);

    nlohmann::json jsonObj;

    if(file.is_open()) {
        file >> jsonObj;
    }

    file.close();

    numPlayers_ = jsonObj["metadata"]["numPlayers"];
    max_teams_ = NCR(numPlayers_, 2);
    winPercentages_ = PriorityQueue(max_teams_);

    Key winPercentage;
    Value teamComb;
    double placeHolder = 0;
    for (int i = 0; i < (int)max_teams_; i++) {
        placeHolder = jsonObj["teamStats"][i]["winPercentage"];
        winPercentage = abs(placeHolder - 50);
        teamComb = make_pair(jsonObj["teamStats"][i]["playerOne"], jsonObj["teamStats"][i]["playerTwo"]);
        winPercentages_.insert(make_pair(winPercentage, teamComb));
    }
    /*for (auto arrayItr = jsonObj["teamStats"].begin();
         arrayItr != jsonObj["teamStats"].end();
         ++arrayItr) {
        placeHolder = (*arrayItr)["winPercentage"];
        winPercentage = abs(placeHolder - 50);
        teamComb = make_pair((*arrayItr)["playerOne"], (*arrayItr)["playerTwo"]);
        winPercentages_.insert(make_pair(winPercentage, teamComb));
    } */
    cout << winPercentages_.JSON().dump(2) << endl;
}

int TeamData::numPlayers() const {
    return numPlayers_;
}

//double TeamData::winPercentages(int i, int j) const {

//}

std::pair<int, int> TeamData::extract() {
    KeyValuePair returnVal = winPercentages_.removeMin();
    return returnVal.second;
}

void TeamData::optimizeTeams(int i, int j, int index){ // called after removeMin()
    if (index > (int)winPercentages_.size()) {
        return;
    }
    if ((winPercentages_.getValue(index)).first == i || (winPercentages_.getValue(index)).first == j) {
        //cout << "(winPercentages_.getValue(index)).first: "<< (winPercentages_.getValue(index)).first << endl;
        //cout << "index: " << index << endl;
        winPercentages_.removeNode(index);
        //cout << winPercentages_.JSON().dump(2) << endl;
        optimizeTeams(i, j, index);
    } else if ((winPercentages_.getValue(index)).second == i || (winPercentages_.getValue(index)).second == j){
        //cout << "(winPercentages_.getValue(index)).second: "<< (winPercentages_.getValue(index)).second << endl;
        //cout << "index: " << index << endl;
        winPercentages_.removeNode(index);
        //cout << winPercentages_.JSON().dump(2) << endl;
        optimizeTeams(i, j, index);
    } else {
        optimizeTeams(i, j, index + 1);
    }
}

