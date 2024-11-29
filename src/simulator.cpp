#include <cstdlib>
#include <cstring>
#include <iostream>
#include <thread>
#include <vector>

#include "monitor_queue.hpp"

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
void producer_function(MonitorQueue &queue, int id, int item_count);
void consumer_function(MonitorQueue &queue);

auto usage_string = "Usage: simulapc -p <int:producers> -c <int:consumers> -s <int:queue_size> -t <int:max_wait_time>";

int main(const int argc, const char *argv[]) {
    const auto [producers, consumers, queue_size, max_wait_time] = parse_args(argc, argv);

    MonitorQueue monitor(queue_size, max_wait_time, "monitor.log");

    std::vector<std::thread> producer_threads;
    std::vector<std::thread> consumer_threads;
    producer_threads.reserve(producers);
    consumer_threads.reserve(consumers);

    for (int i = 0; i < producers; i++)
        producer_threads.emplace_back(producer_function, std::ref(monitor), i + 1, 10);

    for (int i = 0; i < consumers; i++)
        consumer_threads.emplace_back(consumer_function, std::ref(monitor));

    for (auto &thread : producer_threads)
        thread.join();

    for (auto &thread : consumer_threads)
        thread.join();

    std::cout << "Simulation over. Check log file monitor.log" << std::endl;

    return 0;
}

void producer_function(MonitorQueue &queue, const int id, const int item_count) {
    queue.producer_started();

    for (int i = 0; i < item_count; ++i) {
        const int value = id * 1000 + i;
        queue.push(value);
        std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 100));
    }

    queue.producer_finished();
}

void consumer_function(MonitorQueue &queue) {
    while (true) {
        auto value = queue.pop();
        if (!value.has_value()) {
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 100));
    }
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
