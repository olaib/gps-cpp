#pragma once
#include <iostream>
#include <vector>

constexpr size_t BITS_NUM = 1023;
constexpr double RATE_PER_CHIP = 1.0;
constexpr auto INVALID_ARGUMENTS = " <satellite number> <sampling frequency>";
constexpr size_t CA_CODE_BITS = 1023;
constexpr size_t REGISTER_BITS = 10;
constexpr size_t SV_NUMBERS = 37;
//========================================
using BitsVec = std::vector<bool>;
using IntVec = std::vector<int>;
using LFSR = std::array<int, REGISTER_BITS>;
// 1 + x3 + x10
// 1 + x2 + x3 + x6 + x8 + x9 + x10
const auto G1_POSITIONS = IntVec{10};
using OutputPosition = IntVec;
// ========================================
 // G1 LFSR: 1 + x3 + x10
constexpr const LFSR G1_LFSR = {0, 0, 1, 0, 0, 0, 0, 0, 0, 1};
 // G2 LFSR: 1 + x2 + x3 + x6 + x8 + x9 + x10
constexpr const LFSR G2_LFSR = {0, 1, 1, 0, 0, 1, 0, 1, 1, 1};
// ========================================
//Table of C/A code for each satellite (sets delay of G2 generator)
const std::vector<IntVec> SV_OUTPUTS{
        OutputPosition{2, 6}, // PRN #1
        OutputPosition{3, 7}, // PRN #2
        OutputPosition{4, 8}, // PRN #3
        OutputPosition{5, 9}, // PRN #4
        OutputPosition{1, 9}, // PRN #5
        OutputPosition{2, 10},// PRN #6
        OutputPosition{1, 8}, // PRN #7
        OutputPosition{2, 9}, // PRN #8
        OutputPosition{3, 10},// PRN #9
        OutputPosition{2, 3}, // PRN #10
        OutputPosition{3, 4}, // PRN #11
        OutputPosition{5, 6}, // PRN #12
        OutputPosition{6, 7}, // PRN #13
        OutputPosition{7, 8}, // PRN #14
        OutputPosition{8, 9}, // PRN #15
        OutputPosition{9, 10},// PRN #16
        OutputPosition{1, 4}, // PRN #17
        OutputPosition{2, 5}, // PRN #18
        OutputPosition{3, 6}, // PRN #19
        OutputPosition{4, 7}, // PRN #20
        OutputPosition{5, 8}, // PRN #21
        OutputPosition{6, 9}, // PRN #22
        OutputPosition{1, 3}, // PRN #23
        OutputPosition{4, 6}, // PRN #24
        OutputPosition{5, 7}, // PRN #25
        OutputPosition{6, 8}, // PRN #26
        OutputPosition{7, 9}, // PRN #27
        OutputPosition{8, 10},// PRN #28
        OutputPosition{1, 6}, // PRN #29
        OutputPosition{2, 7}, // PRN #30
        OutputPosition{3, 8}, // PRN #31
        OutputPosition{4, 9}, // PRN #32
        OutputPosition{5, 10},// PRN #33
        OutputPosition{4, 10},// PRN #34
        OutputPosition{1, 7}, // PRN #35
        OutputPosition{2, 8}, // PRN #36
        OutputPosition{4, 10} // PRN #37
};

