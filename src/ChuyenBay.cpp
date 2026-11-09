#include "ChuyenBay.h"

ChuyenBay::ChuyenBay() : trangThai_(1), soCho_(0) {}

ChuyenBay::ChuyenBay(const std::string& maChuyenBay,
                     const std::string& soHieuMayBay,
                     const std::string& ngayKhoiHanh,
                     const std::string& sanBayDen, int soCho)
    : maChuyenBay_(maChuyenBay), soHieuMayBay_(soHieuMayBay),
      ngayKhoiHanh_(ngayKhoiHanh), sanBayDen_(sanBayDen),
      trangThai_(1), soCho_(soCho) {
    for (int i = 1; i <= soCho_; ++i) gheTrong_.themCuoi(i);
}

const std::string& ChuyenBay::layMaChuyenBay() const { return maChuyenBay_; }
const std::string& ChuyenBay::laySoHieuMayBay() const { return soHieuMayBay_; }
const std::string& ChuyenBay::layNgayKhoiHanh() const { return ngayKhoiHanh_; }
const std::string& ChuyenBay::laySanBayDen() const { return sanBayDen_; }
int ChuyenBay::layTrangThai() const { return trangThai_; }
int ChuyenBay::laySoCho() const { return soCho_; }
std::size_t ChuyenBay::laySoVe() const { return danhSachVe_.kichThuoc(); }
std::size_t ChuyenBay::laySoGheTrong() const { return gheTrong_.kichThuoc(); }
const LinkedList<Ve>& ChuyenBay::layDanhSachVe() const { return danhSachVe_; }
const LinkedList<int>& ChuyenBay::layGheTrong() const { return gheTrong_; }

bool ChuyenBay::gheConTrong(int soGhe) const {
    return gheTrong_.timNeu([soGhe](int ghe) { return ghe == soGhe; }) != nullptr;
}

bool ChuyenBay::daCoKhach(const std::string& cmnd) const {
    return danhSachVe_.timNeu([&cmnd](const Ve& ve) {
        return ve.layCmnd() == cmnd;
    }) != nullptr;
}

const Ve* ChuyenBay::timVe(const std::string& maVe) const {
    return danhSachVe_.timNeu([&maVe](const Ve& ve) {
        return ve.layMaVe() == maVe;
    });
}

void ChuyenBay::capNhatTrangThaiTuGhe() {
    if (trangThai_ == 1 || trangThai_ == 2) {
        trangThai_ = gheTrong_.rong() ? 2 : 1;
    }
}

bool ChuyenBay::themVe(const Ve& ve, std::string& loi) {
    if (trangThai_ != 1) {
        loi = "Chuyen bay khong o trang thai con ve.";
        return false;
    }
    if (ve.layMaChuyenBay() != maChuyenBay_ || !gheConTrong(ve.laySoGhe())) {
        loi = "So ghe khong hop le hoac da duoc ban.";
        return false;
    }
    if (daCoKhach(ve.layCmnd())) {
        loi = "Khach da co ve tren chuyen bay nay.";
        return false;
    }
    danhSachVe_.themCuoi(ve);
    gheTrong_.xoaDauTienNeu([&ve](int ghe) { return ghe == ve.laySoGhe(); });
    capNhatTrangThaiTuGhe();
    return true;
}

bool ChuyenBay::themVeKhiDocFile(const Ve& ve) {
    if (ve.layMaChuyenBay() != maChuyenBay_ || !gheConTrong(ve.laySoGhe()) ||
        daCoKhach(ve.layCmnd())) return false;
    danhSachVe_.themCuoi(ve);
    gheTrong_.xoaDauTienNeu([&ve](int ghe) { return ghe == ve.laySoGhe(); });
    capNhatTrangThaiTuGhe();
    return true;
}

bool ChuyenBay::traVe(const std::string& maVe) {
    if (trangThai_ == 3) return false;
    const Ve* ve = timVe(maVe);
    if (ve == nullptr) return false;
    const int soGhe = ve->laySoGhe();
    danhSachVe_.xoaDauTienNeu([&maVe](const Ve& item) {
        return item.layMaVe() == maVe;
    });
    gheTrong_.themCuoi(soGhe);
    gheTrong_.sapXep([](int a, int b) { return a < b; });
    capNhatTrangThaiTuGhe();
    return true;
}

bool ChuyenBay::capNhatTrangThai(int trangThai, std::string& loi) {
    if (trangThai != 0 && trangThai != 3) {
        loi = "Chi co the danh dau 0 (huy) hoac 3 (hoan tat); 1/2 tu dong.";
        return false;
    }
    if (trangThai_ == 0 || trangThai_ == 3) {
        loi = "Chuyen bay da huy/hoan tat, khong the doi lai.";
        return false;
    }
    trangThai_ = trangThai;
    return true;
}

void ChuyenBay::phucHoiTrangThai(int trangThai) {
    if (trangThai == 0 || trangThai == 3) trangThai_ = trangThai;
    else {
        trangThai_ = 1;
        capNhatTrangThaiTuGhe();
    }
}
