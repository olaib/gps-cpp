#pragma once
#include <iostream>
#include <utility>
#include <vector>
#include <utities.h>
using CACodeTyoe = std::vector<bool>;

class CACode {
private:
    CACodeTyoe m_caCode;
public:
    explicit CACode(CACodeTyoe caCode = CACodeTyoe(BITS_NUM)) : m_caCode(std::move(caCode)) {}
    void reset() {
        m_caCode = std::vector<bool>(BITS_NUM);
    }
    CACodeTyoe& operator()() {
        return m_caCode;
    }
    const CACodeTyoe& operator()() const {
        return m_caCode;
    }
    CACodeTyoe::iterator begin() {
        return m_caCode.begin();
    }
    CACodeTyoe::iterator end() {
        return m_caCode.end();
    }
    void display() const {
        std::cout << "C/A code: ";
        for (int i = 0; i < BITS_NUM; ++i) {
            std::cout << m_caCode[i] << " ";
            if (i % 80 == 0) std::cout << std::endl;
        }
        std::cout << std::endl;
    }
};

