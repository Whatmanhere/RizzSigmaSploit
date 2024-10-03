#pragma once
#include "include.hpp"
//im cooked
namespace helper {
    int valueToHex(int n) {
        std::stringstream ss;
        ss << std::hex << n;
        int hexValue;
        ss >> hexValue;
        return hexValue;
    }
}