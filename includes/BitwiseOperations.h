#pragma once

#include<utities.h>

//class BitwiseOperations {
//public:
//    static bool XOR(const bool& a, const bool& b) {
//        return (a+b) % 2;
//    }
//    static bool XORIndexes(const ShiftRegister& reg, const IntVector& xorIndexes) {
//        bool result = reg[xorIndexes[0]];
//        for (int i = 1; i < xorIndexes.size(); ++i) {
//            result = XOR(result, reg[xorIndexes[i]]);
//        }
//        return result;
//    }
//    static bool SR(const bool& feedback, ShiftRegister& shiftRegister) {
//        std::copy_backward(shiftRegister.begin(), shiftRegister.end() - 1, shiftRegister.end());
//        shiftRegister[0] = feedback;
//        return feedback; // next bit
//    }
//};

class BitwiseOperations {
public:
    static bool Mod2(const int &integer) {
        return integer % 2;
    }

    static bool Mod2(const int &a, const int &b) {
        return (a + b) % 2;
    }

    static bool sumIntegersMod2(const IntVec &integers) {
        return Mod2(std::accumulate(integers.begin(), integers.end(), 0, [](const int &acc, const int &i) {
            return acc + i;
        }));
    }

    // xor of shiftRegister
    static bool xorPolynomials(const IntVec &positions, const BitsVec &shiftRegister) {
        return Mod2(
                std::accumulate(positions.begin(), positions.end(), 0, [shiftRegister](const int &acc, const int &pos) {
                    return acc ^ shiftRegister[pos];
                }));
    }

    static bool xorPolynomials(const LFSR &feedbacks, const BitsVec &shiftRegister) {
        int sum = 0;
        for (size_t i = 0; i < REGISTER_BITS; ++i) {
            if (feedbacks[i]) {
                sum += shiftRegister[i];
            }
        }
        return Mod2(sum);
    }

    static void shiftRight(BitsVec &shiftRegister, const bool &feedback) {
        // shift right by 10
        for (size_t i = REGISTER_BITS - 1; i > 0; --i) {
            shiftRegister[i] = shiftRegister[i - 1];
        }
        shiftRegister[0] = feedback;
    }
};
