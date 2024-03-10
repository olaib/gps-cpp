#pragma once
#include <utities.h>
#include <BitwiseOperations.h>

class ShiftRegister {
    BitsVec m_shiftRegister;
public:
    ShiftRegister() : m_shiftRegister(REGISTER_BITS_NUM, 1) {}
    BitsVec::iterator begin() {
        return m_shiftRegister.begin();
    }
    BitsVec::iterator end() {
        return m_shiftRegister.end();
    }

    /***
     * Shift and xor the shift register bits based on feedback positions
     * @param outputPositions : output positions of the shift register
     * - G1 should be 10
     * - G2 should be by satellite number SV(prn) % 2 + 1 and SV(prn) % 2 + 2
     * @return
     */
    bool shift(const FeedbackPositions& feedbackPositions,const BitsVec& outputPositions) {
    }
};