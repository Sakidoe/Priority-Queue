#include <iostream>
#include <fstream>
#include <string>

#include "priorityqueue.h"
#include "json.hpp"

using namespace std;

PriorityQueue buildHeap (nlohmann::json instruction) {
    size_t max_size = instruction["metadata"]["maxHeapSize"];
    PriorityQueue heap(max_size);
    int numOperations = instruction["metadata"]["numOperations"];
    string operationCount;
    Key key;
    unsigned int totalZeros = (int) floor(log10((double) numOperations)) + 1; // from createheapoperationdata.cxx
    for (int i = 1; i <=  numOperations; i++) {
        int opZeros = (int) floor(log10((double) i )) + 1; // to add the zeros before the number for accessing operations
        operationCount = "Op" + string(totalZeros - opZeros, '0').append(to_string(i)); // from createheapoperationdata.cxx
        if (instruction[operationCount]["operation"] == "insert") {
            key = instruction[operationCount]["key"];
            heap.insert(key);
        } else if (instruction[operationCount]["operation"] == "removeMin") {
            heap.removeMin();
        }
    }
    return heap;
}

void checkFile (ifstream& file, string fileName) {
    if (!file) { // check if file is good
        cerr << "Error: cannot open file " << fileName << endl;
        exit(1);
    }
}

int main(int argc, char** argv) {
    if (argc != 2) { // input validation
        cerr << "Usage: ./buildheap.exe <file.json>" << endl;
        exit(1);
    }

    ifstream file;

    file.open(argv[1]);
    checkFile(file, argv[1]);

    nlohmann::json heapInstruction;

    if(file.is_open()) {
        file >> heapInstruction;
    }

    file.close();

    PriorityQueue heap = buildHeap(heapInstruction);

    nlohmann::json result = heap.JSON();

    result["metadata"]["maxHeapSize"] = result["metadata"]["max_size"];
    result["metadata"]["numOperations"] = heapInstruction["metadata"]["numOperations"];

    cout << result.dump(2) << endl;

    return 0;
}
