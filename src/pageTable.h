#pragma once

#include <unordered_map>
#include <list>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

class PageTable {
private:
    unordered_map<int, list<int>::iterator> pageMap;
    list<int> pages;
    int capacity;
    int pageFaults;

public:
    explicit PageTable(int cap) : capacity(cap), pageFaults(0) {}

    int getPageFaults() const { return pageFaults; }

    void optimal(const vector<int>& referenceString, int current) {
        if (pageMap.find(referenceString[current]) == pageMap.end()) {
            if (pages.size() == capacity) {
                auto farthest = static_cast<std::vector<int>::difference_type>(-1); // ns como hacerlo aqui
                int pageToRemove = -1;
                for (int page : pages) {
                    auto it = find(referenceString.begin() + current + 1, referenceString.end(), page);
                    if (it == referenceString.end()) {
                        pageToRemove = page;
                        break;
                    } else if (it - referenceString.begin() > farthest) {
                        farthest = it - referenceString.begin();
                        pageToRemove = page;
                    }
                }
                if (pageToRemove != -1) {
                    remove(pageToRemove);
                }
            }
            insert(referenceString[current]);
            pageFaults++;
        }
    }


    void fifo(int page) {
        if (pageMap.find(page) == pageMap.end()) {
            if (pages.size() == capacity) {
                remove(pages.front());
            }
            insert(page);
            pageFaults++;
        }
    }

    void lru(int page) {
        if (pageMap.find(page) == pageMap.end()) {
            if (pages.size() == capacity) {
                remove(pages.back());
            }
            insert(page);
            pageFaults++;
        } else {
            pages.erase(pageMap[page]);
            insert(page);
        }
    }

    void lruClock(int page, vector<int>& clockBits) {
        if (pageMap.find(page) == pageMap.end()) {
            while (!pages.empty() && clockBits[pages.front()] == 1) {
                clockBits[pages.front()] = 0;
                pages.push_back(pages.front());
                pages.pop_front();
            }
            if (pages.size() == capacity) {
                clockBits[pages.front()] = 0;
                remove(pages.front());
            }
            insert(page);
            clockBits[page] = 1;
            pageFaults++;
        } else {
            clockBits[page] = 1;
        }
    }

private:
    void insert(int page) {
        pages.push_back(page);
        pageMap[page] = --pages.end();
    }

    void remove(int page) {
        if (pageMap.find(page) != pageMap.end()) {
            pages.erase(pageMap[page]);
            pageMap.erase(page);
        }
    }
};
