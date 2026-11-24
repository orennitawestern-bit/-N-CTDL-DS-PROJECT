#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

namespace Utils {
    std::string catKhoangTrang(const std::string& value);
    bool hopLeVanBan(const std::string& value);
    bool hopLeMa(const std::string& value);
    bool hopLeCmnd(const std::string& value);
    bool hopLeNgay(const std::string& value);
    bool chuyenSoNguyen(const std::string& value, int& result);
    std::vector<std::string> tachChuoi(const std::string& line, char separator);
    std::string tenTrangThai(int trangThai);
}

#endif
