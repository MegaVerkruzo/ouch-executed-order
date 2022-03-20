#pragma once

#include <cmath>
#include <string>
#include <vector>

inline unsigned decode_binary4(const std::vector<unsigned char> & message, size_t & offset)
{
    unsigned result = 0;
    for (size_t i = 0; i < 4; i++) {
        result <<= 8;
        result += message[offset++];
    }
    return result;
}

inline double decode_price(const std::vector<unsigned char> & message, size_t & offset)
{
    return static_cast<double>(decode_binary4(message, offset)) / 10000;
}

inline std::string decode_text(const std::vector<unsigned char> & message, const size_t len, size_t & offset)
{
    size_t actual_len = len;
    while (actual_len > 0 && message[offset + actual_len - 1] == ' ') {
        actual_len--;
    }
    std::string result = std::string{reinterpret_cast<const char *>(&message[offset]), actual_len};
    offset += len;
    return result;
}

inline void decode_mmt(const std::vector<unsigned char> & message, char (&mmt)[15], size_t & offset)
{
#define FIELD(_, default_value, index) mmt[index] = default_value;

#include "fields_mmt.inl"

#define FIELD(_, index) mmt[index] = static_cast<char>(message[offset++]);

#include "opt_fields.inl"
}
