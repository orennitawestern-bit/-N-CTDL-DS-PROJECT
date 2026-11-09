#ifndef KHACH_HANG_H
#define KHACH_HANG_H

#include <string>

/** Thong tin khach cho; ma chuyen va so ghe dung de xu ly hang doi. */
class KhachHang {
private:
    int soThuTu_;
    std::string cmnd_;
    std::string hoTen_;
    std::string maChuyenBay_;
    int soGhe_;

public:
    KhachHang();
    KhachHang(int soThuTu, const std::string& cmnd, const std::string& hoTen,
              const std::string& maChuyenBay, int soGhe);

    int laySoThuTu() const;
    const std::string& layCmnd() const;
    const std::string& layHoTen() const;
    const std::string& layMaChuyenBay() const;
    int laySoGhe() const;
};

#endif
