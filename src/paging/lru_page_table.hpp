#pragma once

#include "page_table.hpp"

class LRUPageTable final : virtual public PageTable {
public:
    explicit LRUPageTable(const int cap) : PageTable(Algorithm::LRU, cap) {}

    void run_algorithm(const int page) override {
        if (m_page_map.find(page) != m_page_map.end()) {
            remove(page);
            insert(page);
            return;
        }

        if (m_pages.size() == m_capacity)
            remove(m_pages.back());

        insert(page);
        m_page_faults++;
    }

private:
    void insert(const int page) override {
        m_pages.push_front(page);
        m_page_map[page] = m_pages.begin();
    }

    void remove(const int page) override {
        if (m_page_map.find(page) == m_page_map.end())
            return;

        m_pages.erase(m_page_map[page]);
        m_page_map.erase(page);
    }
};
