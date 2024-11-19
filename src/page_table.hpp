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

public:
    explicit PageTable(int const cap) : m_capacity(cap) {}

    int get_page_faults() const {
        return m_page_faults;
    }

    void optimal(const std::vector<int> &references, const int current) {
        if (m_page_map.find(references[current]) != m_page_map.end())
            return;

        if (m_pages.size() == m_capacity) {
            // TODO: is this right?
            auto farthest = static_cast<std::vector<int>::difference_type>(-1);
            int page_to_remove = -1;

            for (int page : m_pages) {
                auto it = find(references.begin() + current + 1, references.end(), page);

                if (it == references.end()) {
                    page_to_remove = page;
                    break;
                }

                if (it - references.begin() > farthest) {
                    farthest = it - references.begin();
                    page_to_remove = page;
                }
            }

            if (page_to_remove != -1)
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
            insert(page);
            return;
        }

        if (m_pages.size() == m_capacity)
            remove(m_pages.back());

        insert(page);
        m_page_faults++;
    }

    void lru_clock(const int page, std::vector<int> &clock_bits) {
        if (m_page_map.find(page) != m_page_map.end()) {
            clock_bits[page] = 1;
            return;
        }

        while (!m_pages.empty() && clock_bits[m_pages.front()] == 1) {
            clock_bits[m_pages.front()] = 0;
            m_pages.push_back(m_pages.front());
            m_pages.pop_front();
        }

        if (m_pages.size() == m_capacity) {
            clock_bits[m_pages.front()] = 0;
            remove(m_pages.front());
        }

        insert(page);
        clock_bits[page] = 1;
        m_page_faults++;
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
    }
};
