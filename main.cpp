#include <iostream>
#include <cstdlib>
#include <vector>
#include <array>
#include <algorithm>
#include <utities.h>
#include <BitwiseOperations.h>
#include <ctime>
#include <cmath>

constexpr double MHZ = 1.023;
constexpr size_t N = 10; // sequence length
constexpr size_t DATA_BITS = 600;
constexpr double TWO_M_PI = 2 * M_PI;
constexpr std::pair<int, int> FREQ_RANGE = {-5000, 5000};
constexpr double FREQUENCY = 10.23e6 * N;
// 1.0e - 12;
constexpr double SAMPLING_RATE = 1000;
constexpr unsigned int DOPPLER_FREQ = 5000;

// ======================================= Prototype =======================================
template<typename T=bool>
void show(const std::vector<T> &bits);
void generateCAcode(size_t svNumber, BitsVec &caCode);

bool gpsShift(BitsVec &shiftRegister, const LFSR &feedbacks, const IntVec &outputPositions);

void readBitsAtRate(const BitsVec &caCode, BitsVec &antina, const size_t &n = N);

std::vector<double> generateSinTable(double frequency, double samplingRate, size_t tableSize);

std::vector<double>
resampleSinWave(const std::vector<double> &sinWave, double rate, double samplingRate, size_t newSize);

std::vector<int> multiDataWithCaCode(const BitsVec &data, const BitsVec &caCode);
std::vector<double> modulateWithDopler(const std::vector<int> &modulatedData, const std::vector<double> &sinTable);
// ================================================================================
auto isValidRegister = [](const BitsVec &shiftRegister) {
    return shiftRegister.size() == REGISTER_BITS;
};

// ======================================= Main =======================================
void readBitsAtRate(const BitsVec &caCode, BitsVec &antina, const size_t &n) {
    // read bits at 1.023 MHz * N
    for (size_t i = 0, k = 0; i < CA_CODE_BITS; i++) {
        for (size_t j = 0; j < n; k++, j++) {
            antina[k] = caCode[i];
        }
    }
    std::cout << "Antina at N * 1.023 MHz" << std::endl;
    show(antina);
    std::cout << "=============================" << std::endl;
}

int main() {
    try {
        std::srand(std::time(nullptr));
        // C/A code - 1023 bits
        BitsVec caCode(CA_CODE_BITS);
        size_t svNumber = 1;
        // init shift registers G1 and G2 with 10 bits of 1
        generateCAcode(svNumber, caCode);
        std::cout << "C/A code for satellite " << svNumber << std::endl;
        show(caCode);
        std::cout << "=============================" << std::endl;
        BitsVec antina(BITS_NUM * N, 0);

        readBitsAtRate(caCode, antina);
        BitsVec data(DATA_BITS);
        std::generate(data.begin(), data.end(), []() { return std::rand() % 2; });
        std::cout << "Data bits" << std::endl;
        show(data);
        std::cout << "=============================" << std::endl;
        // multiply data bits with antina
        const auto result = multiDataWithCaCode(data, antina);
        std::cout << "Multiplied data with C/A code" << std::endl;
        show(data);
        std::cout << "=============================" << std::endl;
        auto tableSize = 1000;
        auto sampleRate = 1.023e6 * N;
        // generate sin table
        const auto sinTable = generateSinTable(DOPPLER_FREQ, sampleRate, tableSize);
        const auto modulated = modulateWithDopler(result, sinTable);
        std::cout << "Modulated data with Doppler" << std::endl;
        show<double>(modulated);
        std::cout << "=============================" << std::endl;

        return EXIT_SUCCESS;
    } catch (std::exception &e) { std::cerr << e.what() << std::endl; }
}

// ======================================= Functions =======================================
void generateCAcode(size_t svNumber, BitsVec &caCode) {
    // check if the prn values not in range 1-32
    if (svNumber < 1 || svNumber > 32)
        throw std::invalid_argument("PRN values must be in range 1-32");

    // Generate C/A code
    BitsVec G1(REGISTER_BITS, 1), G2(REGISTER_BITS, 1);
    std::cout << "G1:" << std::endl;
    show(G1);
    std::cout << "G2:" << std::endl;
    show(G2);
    std::cout << "Generating ...." << std::endl;
    for (size_t i = 0; i < CA_CODE_BITS; ++i) {
        bool G1OutBit = gpsShift(G1, G1_LFSR, G1_POSITIONS);
        bool G2OutBit = gpsShift(G2, G2_LFSR, SV_OUTPUTS[svNumber - 1]);
        caCode[i] = BitwiseOperations::Mod2(G1OutBit, G2OutBit);
    }
}
// ===============================================================================

bool gpsShift(BitsVec &shiftRegister, const LFSR &feedbacks, const IntVec &outputPositions) {
    // param check
    if (shiftRegister.empty() || feedbacks.empty() || outputPositions.empty())
        throw std::invalid_argument("Empty shift register, feedback or output positions");

    // Register size should be 10
    if (!isValidRegister(shiftRegister)) throw std::invalid_argument("Shift register size should be 10");

    // outputPositions should be in range 1-10 (1-indexed)
    if (std::any_of(outputPositions.begin(), outputPositions.end(),
                    [](const int &pos) { return pos < 1 || pos > REGISTER_BITS; }))
        throw std::invalid_argument("Output positions out of range");

    // calculate output bit {sum(shiftRegister[outputPositions]) }mod 2
    bool out = BitwiseOperations::xorPolynomials(outputPositions, shiftRegister);
    // shift right
    bool feedback = BitwiseOperations::xorPolynomials(feedbacks, shiftRegister);

    BitwiseOperations::shiftRight(shiftRegister, feedback);
    return out;
}

// ===============================================================================
template<typename T>
void show(const std::vector<T> &bits) {
    for (const auto &bit: bits) {
        std::cout << bit << " ";
    }
    std::cout << std::endl;
}



std::vector<double>
resampleSinWave(const std::vector<double> &sinWave, double rate, double samplingRate, size_t newSize) {
    std::vector<double> resampled(newSize);
    double ratio = samplingRate / rate;
    for (int i = 0; i < newSize; i++) {
        double sampleIndex = (i * ratio);
        size_t index1 = static_cast<size_t>(std::floor(sampleIndex));
        size_t index2 = std::min(index1 + 1, sinWave.size() - 1);
        double fraction = sampleIndex - index1;
        resampled[i] = (1 - fraction) * sinWave[index1] + fraction * sinWave[index2];
    }
    return resampled;
}

std::vector<double> generateSinTable(double frequency, double samplingRate, size_t tableSize) {
    std::vector<double> table(tableSize);
    double period = samplingRate / frequency;
    std::generate(table.begin(), table.end(), [i = 0, period]() mutable {
        return std::sin(TWO_M_PI * (i++) / period);
    });
    return table;
}

std::vector<int> multiDataWithCaCode(const BitsVec &data, const BitsVec &caCode) {
    std::vector<int> modulated(data.size() * 20);
    for (size_t i = 0; i < data.size(); i++) {
        for (size_t j = 0; j < 20; j++) {
            modulated[i] = caCode[i * 20 + j] ? 1 : -1;
        }
    }
    return modulated;
}

std::vector<double> modulateWithDopler(const std::vector<int> &modulatedData, const std::vector<double> &sinTable) {
    std::vector<double> modulated(modulatedData.size());
    for (size_t i = 0; i < modulatedData.size(); i++) {
        modulated[i] = modulatedData[i] * sinTable[i % sinTable.size()];
    }
    return modulated;
}


