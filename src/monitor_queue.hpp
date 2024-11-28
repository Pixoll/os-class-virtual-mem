#pragma once

#include <chrono>
#include <condition_variable>
#include <fstream>
#include <mutex>
#include <optional>
#include <vector>
#include <iostream>

class MonitorQueue {
    const int m_max_wait_time;
    std::vector<int> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_producer_go, m_consumer_go;
    std::ofstream m_log_file;
    int m_head = 0, m_tail = 0, m_items_stored = 0, m_size = 0;
    int m_active_producers = 0;

public:
    explicit MonitorQueue(const int initial_size, const int max_wait_time, const std::string &log_filename)
        : m_max_wait_time(max_wait_time),
          m_queue(initial_size),
          m_size(initial_size) {
        m_log_file.open(log_filename, std::ios::out);
        if (!m_log_file.is_open()) {
            std::cerr << "Wazaaaaaa: No se pudo abrir el archivo de log " << log_filename << std::endl;
        }
    }

    ~MonitorQueue() {
        if (m_log_file.is_open()) {
            m_log_file.close();
        }
    }

    void producer_started() {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_active_producers++;
    }

    void producer_finished() {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_active_producers--;
        if (m_active_producers == 0) {
            m_consumer_go.notify_all();
        }
    }

    void push(const int value) {
        std::unique_lock<std::mutex> lock(m_mutex);

        m_producer_go.wait(lock, [this]() { return m_items_stored < m_size; });

        m_queue[m_tail] = value;
        m_tail = (m_tail + 1) % m_size;
        m_items_stored++;

        if (m_items_stored == m_size) {
            resize(m_size * 2, "doblada");
        }

        log_event("Producido: " + std::to_string(value) + " (Elementos: " + std::to_string(m_items_stored) + ")");

        m_consumer_go.notify_one();
    }

    std::optional<int> pop() {
        const std::chrono::seconds timeout(m_max_wait_time);
        std::unique_lock<std::mutex> lock(m_mutex);

        while (m_items_stored == 0 && m_active_producers > 0) {
            if (!m_consumer_go.wait_for(lock, timeout, [this] { return m_items_stored > 0 || m_active_producers == 0; })) {
                log_event("Consumo agotado por tiempo de espera");
                return std::nullopt;
            }
        }

        if (m_items_stored == 0 && m_active_producers == 0) {
            return std::nullopt;
        }

        const int value = m_queue[m_head];
        m_head = (m_head + 1) % m_size;
        m_items_stored--;

        if (m_items_stored * 4 < m_size && m_size > 1) {
            resize(m_size / 2, "reducida");
        }

        log_event("Consumido: " + std::to_string(value) + " (Elementos: " + std::to_string(m_items_stored) + ")");

        m_producer_go.notify_one();
        return value;
    }

private:
    void resize(const int new_size, const std::string& action) {
        std::vector<int> new_queue(new_size);
        for (int i = 0; i < m_items_stored; ++i) {
            new_queue[i] = m_queue[(m_head + i) % m_size];
        }
        m_queue = std::move(new_queue);
        m_head = 0;
        m_tail = m_items_stored;
        m_size = new_size;

        log_event("Tamaño de la cola " + action + " a " + std::to_string(m_size) + " elementos");
    }

    void log_event(const std::string& event) {
        if (m_log_file.is_open()) {
            m_log_file << event << std::endl;
        } else {
            std::cerr << "Error: No se pudo registrar el evento, asi que xd nomas." << std::endl;
        }
    }
};
