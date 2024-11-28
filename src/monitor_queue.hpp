#pragma once

#include <chrono>
#include <condition_variable>
#include <fstream>
#include <mutex>
#include <optional>
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

    void push(const int value) {
        m_mutex.lock();

        if (m_items_stored == m_size) {
            resize(m_size * 2);

            if (m_log_file.is_open())
                m_log_file << "Monitor: Queue size doubled.\n";
        }

        m_tail = (m_tail + 1) % m_size;
        m_queue[m_tail] = value;
        m_items_stored++;

        if (m_log_file.is_open())
            m_log_file << "Monitor: [" << value << "] added into queue\n";

        m_log_file.flush();
        m_consumer_go.notify_one();
        m_mutex.unlock();
    }

    // pop returns empty if timed out
    std::optional<int> pop() {
        const std::chrono::seconds timeout(m_max_wait_time);
        std::unique_lock lock(m_mutex);

        if (m_items_stored == 0 && !m_consumer_go.wait_for(
            lock,
            timeout,
            [this] {
                return m_items_stored > 0;
            }
        )) {
            if (m_log_file.is_open())
                m_log_file << "Monitor: Pop timed out\n";
            return std::nullopt;
        }

        const std::optional return_value = m_queue[m_head];
        m_head = (m_head + 1) % m_size;
        m_items_stored--;

        if (m_log_file.is_open())
            m_log_file << "Monitor: [" << *return_value << "] extracted from queue.\n";

        if (m_items_stored * 4 < m_size) {
            resize(m_size / 2);
            if (m_log_file.is_open())
                m_log_file << "Monitor: Queue size halved.\n";
        }

        m_log_file.flush();
        m_mutex.unlock();
        return return_value;
    }

private:
    void resize(const int new_size) {
        // assumed the mutex is already locked, like this whole thing is critical
        const std::vector old_queue = m_queue;
        int old_head = m_head;
        const int old_tail = m_tail;

        m_queue.clear();
        m_queue.reserve(new_size);

        m_head = 0;
        m_tail = 0;
        m_queue[0] = old_queue[old_head];

        while (old_head != old_tail) {
            old_head = (old_head + 1) % m_size;
            m_tail = (m_tail + 1) % new_size;
            m_queue[m_tail] = old_queue[old_head];
        }

        m_size = new_size;
    }
};
