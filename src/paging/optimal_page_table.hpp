#pragma once

#include <algorithm>
#include <iostream>
#include <vector>

#include "page_table.hpp"

class OptimalPageTable final : virtual public PageTable {
    const std::vector<int> m_references;

public:
    explicit OptimalPageTable(const std::vector<int> &references, const int cap)
        : PageTable(Algorithm::OPTIMAL, cap),
          m_references(references) {}

    ~OptimalPageTable() override = default;

    void run_algorithm(const int current) override {
        if (m_page_map.find(m_references[current]) != m_page_map.end()) {
            return;
        }

        if (m_pages.size() == m_capacity) {
            int farthest = -1;
            int page_to_remove = -1;

            for (const int page : m_pages) {
                auto it = std::find(m_references.begin() + current + 1, m_references.end(), page);

                const int distance = it == m_references.end() ? m_references.size() : it - m_references.begin();

                if (distance > farthest) {
                    farthest = distance;
                    page_to_remove = page;
                }
            }

            remove(page_to_remove);
        }

        insert(m_references[current]);
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
