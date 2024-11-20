#pragma once

#include <condition_variable>
#include <fstream>
#include <mutex>
#include <vector>

class MonitorQueue {
    const int m_max_wait_time;
    std::vector<int> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_producer_go;
    std::condition_variable m_consumer_go;
    std::fstream m_log_file;
    int m_head = 0;
    int m_tail = 0;
    int m_items_stored = 0;
    int m_size = 0;

public:
    explicit MonitorQueue(const int initial_size, const int max_wait_time, const std::string &log_filename)
        : m_max_wait_time(max_wait_time),
          m_queue(initial_size),
          m_log_file(log_filename) {}

    // void push() {}
    //
    // int pop() {}

private:
    void resize(int new_size);
};
