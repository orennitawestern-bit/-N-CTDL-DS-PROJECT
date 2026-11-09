#include "Utils.h"

#include <algorithm>
#include <cctype>

namespace Utils {

std::string catKhoangTrang(const std::string& value) {
    const std::size_t first = value.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    const std::size_t last = value.find_last_not_of(" \t\r\n");
    return value.substr(first, last - first + 1);
}

bool hopLeVanBan(const std::string& value) {
    const std::string trimmed = catKhoangTrang(value);
    return !trimmed.empty() && trimmed.find_first_of("|\r\n") == std::string::npos;
}

bool hopLeMa(const std::string& value) {
    if (value.empty()) return false;
    return std::all_of(value.begin(), value.end(), [](unsigned char c) {
        return std::isalnum(c) != 0 || c == '_' || c == '-';
    });
}

bool hopLeCmnd(const std::string& value) {
    if (value.size() != 9 && value.size() != 12) return false;
    return std::all_of(value.begin(), value.end(), [](unsigned char c) {
        return std::isdigit(c) != 0;
    });
}

bool hopLeNgay(const std::string& date) {
    if (date.size() != 10 || date[4] != '-' || date[7] != '-') return false;
    for (std::size_t i = 0; i < date.size(); ++i) {
        if (i != 4 && i != 7 && !std::isdigit(static_cast<unsigned char>(date[i]))) return false;
    }
    const int year = std::stoi(date.substr(0, 4));
    const int month = std::stoi(date.substr(5, 2));
    const int day = std::stoi(date.substr(8, 2));
    if (year < 2000 || month < 1 || month > 12) return false;
    const int days[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    const bool leap = year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
    return day >= 1 && day <= days[month] + ((month == 2 && leap) ? 1 : 0);
}

bool chuyenSoNguyen(const std::string& value, int& result) {
    const std::string s = catKhoangTrang(value);
    if (s.empty()) return false;
    try {
        std::size_t length = 0;
        const int parsed = std::stoi(s, &length);
        if (length != s.size()) return false;
        result = parsed;
        return true;
    } catch (...) {
        return false;
    }
}

std::vector<std::string> tachChuoi(const std::string& line, char separator) {
    std::vector<std::string> tokens;
    std::string token;
    for (char c : line) {
        if (c == separator) {
            tokens.push_back(token);
            token.clear();
        } else token.push_back(c);
    }
    tokens.push_back(token);
    return tokens;
}

std::string tenTrangThai(int trangThai) {
    switch (trangThai) {
        case 0: return "Huy chuyen";
        case 1: return "Con ve";
        case 2: return "Het ve";
        case 3: return "Hoan tat";
        default: return "Khong ro";
    }
}

} // namespace Utils
