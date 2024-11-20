#pragma once

#include <mutex>
#include <condition_variable>
#include <chrono>
#include <fstream>
#include <optional>
class MonitorQueue{
private:
    int* queue; //this is an array
    size_t front,rear,items_stored,size,max_wait_time;
    std::mutex mutex;
    std::condition_variable consumer_go;
    std::ofstream* log_file;
    void resize(size_t new_size);
public:
    MonitorQueue(size_t initial_size, size_t wait_time);
    //pass by reference opened log file, return if it is succesful
    bool set_log_file(std::ofstream& new_log_file);
    void push(int value);
    //pop returns empty if timed out
    std::optional<int> pop();
};