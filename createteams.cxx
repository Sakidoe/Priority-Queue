#include <iostream>
#include <utility> //pairs
#include <vector> // json return variable

#include "teamdata.h"
#include "json.hpp"

using namespace std;

int main(int argc, char** argv) {
    if (argc != 2) { // input validation
        cerr << "Usage: ./createteams.exe <file.json>" << endl;
        exit(1);
    }
    TeamData allTeams(argv[1]); // read files into class priority queue

    vector<pair<int, int>> finalTeams;
    
    pair<int, int> currentPair; // place holder

    for (int i = 0; i < allTeams.numPlayers() / 2; i++ ) { // for each team that will be selected
        currentPair = allTeams.extract(); // extract the minimum and only get the team combination
        finalTeams.push_back(currentPair); // input the combination into the vector container
        allTeams.optimizeTeams(currentPair.first, currentPair.second, 1); // delete all other teams containing previous team members
    }

    nlohmann::json result;
    result["teams"] = finalTeams; // output result

    cout << result.dump(2) << endl;
}
