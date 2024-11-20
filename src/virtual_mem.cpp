#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "page_table.hpp"
#include "read_references.hpp"

enum class Algorithm {
    OPTIMAL,
    FIFO,
    LRU,
    LRU_CLOCK,
};

struct Args {
    int frames;
    Algorithm algorithm;
    std::string references_filepath;
};

Args parse_args(int argc, const char *argv[]);

auto usage_string = "Usage: mvirtual -m <int:frames> -a <string:algorithm> -f <string:filepath>";

int main(const int argc, const char *argv[]) {
    const auto [frames, algorithm, filepath] = parse_args(argc, argv);

    const std::vector references = read_references(filepath);

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
            for (const int reference : references)
                page_table.lru_clock(reference);

            std::cout << "Algorithm: LRU_CLOCK";
            break;
        }
    }

    std::cout << ", Page faults: " << page_table.get_page_faults() << std::endl;

    return 0;
}

std::string str_toupper(std::string s);

Args parse_args(const int argc, const char *argv[]) {
    if (argc != 7) {
        std::cerr << usage_string << std::endl;
        exit(1);
    }

    int index_m = -1;
    int index_a = -1;
    int index_f = -1;

    int frames;
    Algorithm algorithm;
    std::string filepath;

    if (strcmp("-m", argv[index_m += 2]) == 0
        || strcmp("-m", argv[index_m += 2]) == 0
        || strcmp("-m", argv[index_m += 2]) == 0
    ) {
        char *end;
        const char *string = argv[index_m + 1];
        frames = strtol(string, &end, 10);

        if (string == end) {
            std::cerr << usage_string << std::endl;
            exit(1);
        }
    } else {
        std::cerr << usage_string << std::endl;
        exit(1);
    }

    if (strcmp("-a", argv[index_a += 2]) == 0
        || strcmp("-a", argv[index_a += 2]) == 0
        || strcmp("-a", argv[index_a += 2]) == 0
    ) {
        const std::string arg_string = str_toupper(argv[index_a + 1]);

        if (arg_string == "OPTIMAL") {
            algorithm = Algorithm::OPTIMAL;
        } else if (arg_string == "FIFO") {
            algorithm = Algorithm::FIFO;
        } else if (arg_string == "LRU") {
            algorithm = Algorithm::LRU;
        } else if (arg_string == "LRU_CLOCK") {
            algorithm = Algorithm::LRU_CLOCK;
        } else {
            std::cerr << usage_string << std::endl;
            exit(1);
        }
    } else {
        std::cerr << usage_string << std::endl;
        exit(1);
    }

    if (strcmp("-f", argv[index_f += 2]) == 0
        || strcmp("-f", argv[index_f += 2]) == 0
        || strcmp("-f", argv[index_f += 2]) == 0
    ) {
        filepath = argv[index_f + 1];
    } else {
        std::cerr << usage_string << std::endl;
        exit(1);
    }

    return {frames, algorithm, filepath};
}

std::string str_toupper(std::string s) {
    std::transform(
        s.begin(),
        s.end(),
        s.begin(),
        [](const unsigned char c) {
            return std::toupper(c);
        }
    );

    return s;
}
