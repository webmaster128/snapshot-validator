#pragma once

#include <string>
#include <vector>
#include <iomanip>
#include <sstream>

inline std::vector<unsigned char> hex2Bytes(const std::string hex) {
    std::vector<unsigned char> out(hex.size() / 2);
    for (int i = 0; i < hex.size(); i += 2)
    {
        std::istringstream iss(hex.substr(i, 2));
        int temp;
        iss >> std::hex >> temp;
        out[i/2] = static_cast<unsigned char>(temp);
    }
    return out;
}

inline std::string bytes2Hex(const std::vector<unsigned char> data) {
    std::stringstream out;
    out << std::setfill('0') << std::hex;
    for(size_t i = 0; i < data.size(); ++i) {
        out << std::setw(2) << static_cast<int>(data[i]);
    }
    return out.str();
}
