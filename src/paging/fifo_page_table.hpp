#pragma once

#include "page_table.hpp"

class FIFOPageTable final : virtual public PageTable {
public:
    explicit FIFOPageTable(const int cap) : PageTable(Algorithm::FIFO, cap) {}

    void run_algorithm(const int page) override {
        if (m_page_map.find(page) != m_page_map.end())
            return;

        if (m_pages.size() == m_capacity)
            remove(m_pages.front());

        insert(page);
        m_page_faults++;
    }

private:
    void insert(const int page) override {
        m_pages.push_back(page);
        m_page_map[page] = --m_pages.end();
    }

    void remove(const int page) override {
        if (m_page_map.find(page) == m_page_map.end())
            return;

        m_pages.erase(m_page_map[page]);
        m_page_map.erase(page);
    }
};
