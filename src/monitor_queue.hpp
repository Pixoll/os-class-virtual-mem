#pragma once

#include <mutex>
#include <condition_variable>
#include <vector>
#include <fstream>
class MonitorQueue{
private:
    std::vector<int> queue;
    int front,rear,items_stored,size,max_wait_time;
    std::mutex mutex;
    std::condition_variable producer_go;
    std::condition_variable consumer_go;
    std::fstream log_file;
    void resize(int newsize);
public:
    MonitorQueue(int initial_size);
    void set_log_file(std::fstream new_log_file);
    void push();
    int pop();
};