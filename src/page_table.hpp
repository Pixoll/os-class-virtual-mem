#pragma once

#include <algorithm>
#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>

class PageTable {
    std::unordered_map<int, std::list<int>::iterator> m_page_map;
    std::list<int> m_pages;
    int m_capacity;
    int m_page_faults = 0;
    std::unordered_map<int, int> m_clock_bits;

public:
    explicit PageTable(const int cap) : m_capacity(cap) {}

    int get_page_faults() const {
        return m_page_faults;
    }

    void optimal(const std::vector<int> &references, const int current) {
        if (m_page_map.find(references[current]) != m_page_map.end()) {
            return;
        }

        if (m_pages.size() == m_capacity) {
            int farthest = -1;
            int page_to_remove = -1;
            for (int page : m_pages) {
                auto it = std::find(references.begin() + current + 1, references.end(), page);
                const int distance = it == references.end() ? references.size() : it - references.begin();

                if (distance > farthest) {
                    farthest = distance;
                    page_to_remove = page;
                }
            }

            remove(page_to_remove);
        }

        insert(references[current]);
        m_page_faults++;
    }

    void fifo(const int page) {
        if (m_page_map.find(page) != m_page_map.end())
            return;

        if (m_pages.size() == m_capacity)
            remove(m_pages.front());

        insert(page);
        m_page_faults++;
    }

    void lru(const int page) {
        if (m_page_map.find(page) != m_page_map.end()) {
            m_pages.erase(m_page_map[page]);
            m_pages.push_front(page);
            m_page_map[page] = m_pages.begin();
            return;
        }

        if (m_pages.size() == m_capacity) {
            const int lru_page = m_pages.back();
            remove(lru_page);
        }

        insert(page);
        m_page_faults++;
    }

    void lru_clock(const int page) {
        if (m_page_map.find(page) != m_page_map.end()) {
            m_clock_bits[page] = 1;
            return;
        }

        m_page_faults++;

        while (!m_pages.empty() && m_clock_bits[m_pages.front()] == 1) {
            m_clock_bits[m_pages.front()] = 0;
            m_pages.push_back(m_pages.front());
            m_pages.pop_front();
        }

        if (m_pages.size() == m_capacity) {
            const int to_remove = m_pages.front();
            remove(to_remove);
        }

        insert(page);
        m_clock_bits[page] = 1;
    }

private:
    void insert(const int page) {
        m_pages.push_back(page);
        m_page_map[page] = --m_pages.end();
    }

    void remove(const int page) {
        if (m_page_map.find(page) == m_page_map.end())
            return;

        m_pages.erase(m_page_map[page]);
        m_page_map.erase(page);
        m_clock_bits.erase(page);
    }
};
