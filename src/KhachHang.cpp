#include "KhachHang.h"

KhachHang::KhachHang() : soThuTu_(0), soGhe_(0) {}
KhachHang::KhachHang(int soThuTu, const std::string& cmnd, const std::string& hoTen,
                     const std::string& maChuyenBay, int soGhe)
    : soThuTu_(soThuTu), cmnd_(cmnd), hoTen_(hoTen),
      maChuyenBay_(maChuyenBay), soGhe_(soGhe) {}

int KhachHang::laySoThuTu() const { return soThuTu_; }
const std::string& KhachHang::layCmnd() const { return cmnd_; }
const std::string& KhachHang::layHoTen() const { return hoTen_; }
const std::string& KhachHang::layMaChuyenBay() const { return maChuyenBay_; }
int KhachHang::laySoGhe() const { return soGhe_; }
