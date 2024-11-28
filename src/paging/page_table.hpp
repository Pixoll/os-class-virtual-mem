#pragma once

#include <list>
#include <string>
#include <unordered_map>

enum class Algorithm {
    OPTIMAL,
    FIFO,
    LRU,
    LRU_CLOCK,
};

class PageTable {
protected:
    std::unordered_map<int, std::list<int>::iterator> m_page_map;
    std::list<int> m_pages;
    int m_capacity;
    int m_page_faults = 0;

public:
    const std::string m_algo_name;

    explicit PageTable(const Algorithm algorithm, const int cap)
        : m_capacity(cap),
          m_algo_name(algo_to_name(algorithm)) {}

    virtual ~PageTable() = default;

    virtual int get_page_faults() const {
        return m_page_faults;
    }

    virtual void run_algorithm(int page) = 0;

private:
    virtual void insert(int page) = 0;
    virtual void remove(int page) = 0;

    static std::string algo_to_name(const Algorithm algorithm) {
        switch (algorithm) {
            case Algorithm::OPTIMAL:
                return "OPTIMAL";
            case Algorithm::FIFO:
                return "FIFO";
            case Algorithm::LRU:
                return "LRU";
            case Algorithm::LRU_CLOCK:
                return "LRU_CLOCK";
        }
        __builtin_unreachable();
    }
};
