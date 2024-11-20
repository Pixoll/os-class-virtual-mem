#pragma once

#include <condition_variable>
#include <fstream>
#include <mutex>
#include <vector>

class MonitorQueue {
    std::vector<int> m_queue;
    int m_front;
    int m_rear;
    int m_items_stored;
    int m_size;
    int m_max_wait_time;
    std::mutex m_mutex;
    std::condition_variable m_producer_go;
    std::condition_variable m_consumer_go;
    std::fstream m_log_file;

public:
    explicit MonitorQueue(int initial_size);

    void set_log_file(std::fstream new_log_file);
    void push();
    int pop();

private:
    void resize(int new_size);
};
