#ifndef VE_H
#define VE_H

#include <string>

class Ve {
private:
    std::string maVe_;
    std::string maChuyenBay_;
    std::string cmnd_;
    std::string hoTen_;
    int soGhe_;

public:
    Ve();
    Ve(const std::string& maChuyenBay, const std::string& cmnd,
       const std::string& hoTen, int soGhe);

    const std::string& layMaVe() const;
    const std::string& layMaChuyenBay() const;
    const std::string& layCmnd() const;
    const std::string& layHoTen() const;
    int laySoGhe() const;
};

#endif
