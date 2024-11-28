#pragma once

#include <list>
#include <unordered_map>

#include "page_table.hpp"

class LRUClockPageTable final : virtual public PageTable {
    std::unordered_map<int, bool> m_clock_bits;
    std::list<int>::iterator m_clock_pointer;

public:
    explicit LRUClockPageTable(const int cap) : PageTable(Algorithm::LRU_CLOCK, cap) {
        m_clock_pointer = m_pages.end();
    }

    ~LRUClockPageTable() override = default;

    void run_algorithm(const int page) override {
        if (m_page_map.find(page) != m_page_map.end()) {
            m_clock_bits[page] = true;
            return;
        }

        if (m_pages.size() == m_capacity) {
            while (true) {
                if (m_clock_pointer == m_pages.end()) {
                    m_clock_pointer = m_pages.begin();
                }

                int current_page = *m_clock_pointer;

                if (!m_clock_bits[current_page]) {
                    remove(current_page);
                    break;
                }

                m_clock_bits[current_page] = false;
                ++m_clock_pointer;
            }
        }

        insert(page);
        m_clock_bits[page] = true;

        if (m_clock_pointer == m_pages.end()) {
            m_clock_pointer = m_pages.begin();
        }

        m_page_faults++;
    }

private:
    void insert(const int page) override {
        m_pages.push_back(page);
        m_page_map[page] = std::prev(m_pages.end());
    }

    void remove(const int page) override {
        if (m_page_map.find(page) == m_page_map.end()) {
            return;
        }

        if (*m_clock_pointer == page) {
            m_clock_pointer = m_pages.erase(m_clock_pointer);
            if (m_clock_pointer == m_pages.end() && !m_pages.empty()) {
                m_clock_pointer = m_pages.begin();
            }
        } else {
            m_pages.erase(m_page_map[page]);
        }

        m_page_map.erase(page);
        m_clock_bits.erase(page);
    }
};

