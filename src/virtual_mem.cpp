#include <iostream>
#include <vector>

#include "page_table.hpp"
#include "read_references.hpp"

enum class Algorithm {
    OPTIMAL,
    FIFO,
    LRU,
    LRU_CLOCK,
};

void simulate(const std::vector<int> &references, const int frames, const Algorithm algorithm) {
    PageTable page_table(frames);

    switch (algorithm) {
        case Algorithm::OPTIMAL: {
            for (int i = 0; i < references.size(); ++i)
                page_table.optimal(references, i);

            std::cout << "Algorithm: OPTIMAL";
            break;
        }
        case Algorithm::FIFO: {
            for (const int reference : references)
                page_table.fifo(reference);

            std::cout << "Algorithm: FIFO";
            break;
        }
        case Algorithm::LRU: {
            for (const int reference : references)
                page_table.lru(reference);

            std::cout << "Algorithm: LRU";
            break;
        }
        case Algorithm::LRU_CLOCK: {
            std::vector clock_bits(1000, 0);
            for (const int reference : references)
                page_table.lru_clock(reference, clock_bits);

            std::cout << "Algorithm: LRU_CLOCK";
            break;
        }
    }

    std::cout << ", Page faults: " << page_table.get_page_faults() << std::endl;
}

int main() {
    constexpr int frames = 4;
    const std::vector references = {1, 2, 3, 4, 1, 2, 5, 2, 7, 8, 9, 10, 2, 11, 10, 12};

    simulate(references, frames, Algorithm::FIFO);
    simulate(references, frames, Algorithm::LRU);
    simulate(references, frames, Algorithm::LRU_CLOCK);
    simulate(references, frames, Algorithm::OPTIMAL);

    return 0;
}
