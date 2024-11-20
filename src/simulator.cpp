#include <cstdlib>
#include <cstring>
#include <iostream>

struct Args {
    int producers;
    int consumers;
    int queue_size;
    int max_wait_time;
};

struct Arg {
    const char *name = "";
    int index = -1;
    int value = 0;
};

Args parse_args(int argc, const char *argv[]);

auto usage_string = "Usage: simulapc -p <int:producers> -c <int:consumers> -s <int:queue_size> -t <int:max_wait_time>";

int main(const int argc, const char *argv[]) {
    const auto [producers, consumers, queue_size, max_wait_time] = parse_args(argc, argv);

    std::cout << producers << " " << consumers << " " << queue_size << " " << max_wait_time << std::endl;

    return 0;
}

Args parse_args(const int argc, const char *argv[]) {
    if (argc != 9) {
        std::cerr << usage_string << std::endl;
        exit(1);
    }

    Arg args[4] = {{"-p"}, {"-c"}, {"-s"}, {"-t"}};

    for (auto &[name, index, value] : args) {
        if (strcmp(name, argv[index += 2]) != 0
            && strcmp(name, argv[index += 2]) != 0
            && strcmp(name, argv[index += 2]) != 0
            && strcmp(name, argv[index += 2]) != 0
        ) {
            std::cerr << usage_string << std::endl;
            exit(1);
        }

        char *end;
        const char *string = argv[index + 1];
        value = strtol(string, &end, 10);

        if (string == end) {
            std::cerr << usage_string << std::endl;
            exit(1);
        }
    }

    return {args[0].value, args[1].value, args[2].value, args[3].value};
}
