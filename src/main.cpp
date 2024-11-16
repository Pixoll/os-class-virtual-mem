#include <iostream>
#include <cstring>
#include "pageTable.h"
#include "readReferences.h"

using namespace std;

void simulate(const vector<int>& referenceString, int frames, const string& algorithm) {
    PageTable pageTable(frames);
    vector<int> clockBits(1000, 0); // Vector para el LRU con reloj

    for (int i = 0; i < referenceString.size(); ++i) {
        if (algorithm == "OPTIMAL") {
            pageTable.optimal(referenceString, i);
        } else if (algorithm == "FIFO") {
            pageTable.fifo(referenceString[i]);
        } else if (algorithm == "LRU") {
            pageTable.lru(referenceString[i]);
        } else if (algorithm == "LRU_CLOCK") {
            pageTable.lruClock(referenceString[i], clockBits);
        } else {
            cerr << "Algoritmo desconocido: " << algorithm << endl;
            return;
        }
    }

    cout << "Algoritmo: " << algorithm << ", Fallos de página: " << pageTable.getPageFaults() << endl;
}

int main() {
    //test
    int frames = 4;

    vector<int> referenceString = {1, 2, 3, 4, 1, 2, 5, 2, 7, 8, 9, 10, 2,11,10,12};

    simulate(referenceString, frames, "FIFO");
    simulate(referenceString, frames, "LRU");
    simulate(referenceString, frames, "LRU_CLOCK");
    simulate(referenceString, frames, "OPTIMAL");
    return 0;
}

