#include "AirlineManager.h"

#include "Utils.h"

#include <fstream>
#include <system_error>
#include <vector>

namespace {

bool moFileGhi(const std::filesystem::path& duongDan,
               std::ofstream& file, std::string& loi) {
    file.open(duongDan, std::ios::out | std::ios::trunc);
    if (!file) {
        loi = "Khong mo duoc file de ghi: " + duongDan.string();
        return false;
    }
    return true;
}

bool ghiThanhCong(std::ofstream& file, const std::filesystem::path& duongDan,
                   std::string& loi) {
    file.flush();
    if (!file) {
        loi = "Ghi file that bai: " + duongDan.string();
        return false;
    }
    return true;
}

} // namespace

AirlineManager::AirlineManager(const std::filesystem::path& thuMucDuLieu)
    : thuMucDuLieu_(thuMucDuLieu), soThuTuTiepTheo_(1) {}

bool AirlineManager::docDuLieu(std::string& thongBao) {
    thongBao.clear();
    mayBay_.xoaTatCa();
    chuyenBay_.xoaTatCa();
    khachHangCho_ = Queue<KhachHang>();
    soThuTuTiepTheo_ = 1;
    taiKhoanAdmin_.clear();
    matKhauAdmin_.clear();

    std::error_code ec;
    std::filesystem::create_directories(thuMucDuLieu_ / "Ve", ec);
    if (ec) {
        thongBao = "Khong tao duoc thu muc du lieu: " + ec.message();
        return false;
    }

    std::ifstream admin(thuMucDuLieu_ / "Admin.txt");
    std::string line;
    if (!admin || !std::getline(admin, line)) {
        thongBao = "Thieu Admin.txt hoac file rong.";
        return false;
    }
    const std::vector<std::string> login = Utils::tachChuoi(line, '|');
    if (login.size() != 2 || !Utils::hopLeVanBan(login[0]) || login[1].empty()) {
        thongBao = "Admin.txt sai dinh dang. Can: user|password";
        return false;
    }
    taiKhoanAdmin_ = Utils::catKhoangTrang(login[0]);
    matKhauAdmin_ = login[1];

    int boQua = 0;
    std::ifstream fileMayBay(thuMucDuLieu_ / "MayBay.txt");
    if (!fileMayBay) {
        thongBao = "Khong doc duoc MayBay.txt.";
        return false;
    }
    while (std::getline(fileMayBay, line)) {
        if (Utils::catKhoangTrang(line).empty()) continue;
        const std::vector<std::string> parts = Utils::tachChuoi(line, '|');
        int seats = 0;
        if (parts.size() != 2 || !Utils::hopLeMa(parts[0]) ||
            !Utils::chuyenSoNguyen(parts[1], seats) || seats <= 0 || seats > 1000 ||
            timMayBay(parts[0]) != nullptr) {
            ++boQua;
            continue;
        }
        mayBay_.themCuoi(MayBay(parts[0], seats));
    }

    std::ifstream fileChuyenBay(thuMucDuLieu_ / "ChuyenBay.txt");
    if (!fileChuyenBay) {
        thongBao = "Khong doc duoc ChuyenBay.txt.";
        return false;
    }
    while (std::getline(fileChuyenBay, line)) {
        if (Utils::catKhoangTrang(line).empty()) continue;
        const std::vector<std::string> parts = Utils::tachChuoi(line, '|');
        int status = 0;
        if (parts.size() != 7 || !Utils::hopLeMa(parts[0]) ||
            !Utils::hopLeMa(parts[1]) || !Utils::hopLeNgay(parts[2]) ||
            !Utils::hopLeVanBan(parts[3]) || !Utils::chuyenSoNguyen(parts[4], status) ||
            status < 0 || status > 3 || timChuyenBay(parts[0]) != nullptr) {
            ++boQua;
            continue;
        }
        const MayBay* plane = timMayBay(parts[1]);
        if (plane == nullptr) {
            ++boQua;
            continue;
        }
        ChuyenBay flight(parts[0], parts[1], parts[2], parts[3], plane->laySoCho());

        // Danh sach ghe trong duoc tai tao tu suc chua va ve da luu.
        // Truong gheTrong (cot thu 7) van duoc ghi trong ChuyenBay.txt.
        if (!parts[5].empty()) {
            for (const std::string& maVe : Utils::tachChuoi(parts[5], ',')) {
                if (!Utils::hopLeMa(maVe)) {
                    ++boQua;
                    continue;
                }
                std::ifstream ticketFile(thuMucDuLieu_ / "Ve" / (maVe + ".txt"));
                std::string ticketLine;
                if (!ticketFile || !std::getline(ticketFile, ticketLine)) {
                    ++boQua;
                    continue;
                }
                const std::vector<std::string> fields = Utils::tachChuoi(ticketLine, '|');
                int seat = 0;
                if (fields.size() != 5 || fields[0] != maVe ||
                    fields[1] != parts[0] || !Utils::hopLeCmnd(fields[2]) ||
                    !Utils::hopLeVanBan(fields[3]) ||
                    !Utils::chuyenSoNguyen(fields[4], seat) || seat <= 0) {
                    ++boQua;
                    continue;
                }
                Ve ve(fields[1], fields[2], fields[3], seat);
                if (ve.layMaVe() != maVe || !flight.themVeKhiDocFile(ve)) ++boQua;
            }
        }
        flight.phucHoiTrangThai(status);
        chuyenBay_.themCuoi(flight);
    }

    std::ifstream fileKhachHang(thuMucDuLieu_ / "KhachHang.txt");
    if (!fileKhachHang) {
        thongBao = "Khong doc duoc KhachHang.txt.";
        return false;
    }
    while (std::getline(fileKhachHang, line)) {
        if (Utils::catKhoangTrang(line).empty()) continue;
        const std::vector<std::string> parts = Utils::tachChuoi(line, '|');
        int serial = 0;
        int seat = 0;
        if (parts.size() != 5 || !Utils::chuyenSoNguyen(parts[0], serial) || serial < 1 ||
            !Utils::hopLeCmnd(parts[1]) || !Utils::hopLeVanBan(parts[2]) ||
            !Utils::hopLeMa(parts[3]) || !Utils::chuyenSoNguyen(parts[4], seat) ||
            seat < 1) {
            ++boQua;
            continue;
        }
        const ChuyenBay* chuyen = timChuyenBay(parts[3]);
        bool trungYeuCau = gheDangCho(parts[3], seat);
        khachHangCho_.duyet([&](const KhachHang& khach) {
            if (khach.layMaChuyenBay() == parts[3] && khach.layCmnd() == parts[1]) {
                trungYeuCau = true;
            }
        });
        if (chuyen == nullptr || chuyen->layTrangThai() != 1 ||
            !chuyen->gheConTrong(seat) || chuyen->daCoKhach(parts[1]) || trungYeuCau) {
            ++boQua;
            continue;
        }
        khachHangCho_.vaoHang(KhachHang(serial, parts[1], parts[2], parts[3], seat));
        if (serial >= soThuTuTiepTheo_) soThuTuTiepTheo_ = serial + 1;
    }

    thongBao = "Tai du lieu thanh cong. Bo qua " + std::to_string(boQua) +
               " dong/ve khong hop le.";
    return true;
}

bool AirlineManager::dangNhap(const std::string& user,
                              const std::string& password) const {
    return user == taiKhoanAdmin_ && password == matKhauAdmin_;
}

const LinkedList<MayBay>& AirlineManager::layDanhSachMayBay() const { return mayBay_; }
const LinkedList<ChuyenBay>& AirlineManager::layDanhSachChuyenBay() const { return chuyenBay_; }
const Queue<KhachHang>& AirlineManager::layHangDoiKhachHang() const {
    return khachHangCho_;
}

const MayBay* AirlineManager::timMayBay(const std::string& soHieu) const {
    return mayBay_.timNeu([&soHieu](const MayBay& mayBay) {
        return mayBay.laySoHieu() == soHieu;
    });
}

const ChuyenBay* AirlineManager::timChuyenBay(const std::string& maChuyenBay) const {
    return chuyenBay_.timNeu([&maChuyenBay](const ChuyenBay& chuyenBay) {
        return chuyenBay.layMaChuyenBay() == maChuyenBay;
    });
}

ChuyenBay* AirlineManager::timChuyenBayDeSua(const std::string& maChuyenBay) {
    return chuyenBay_.timNeu([&maChuyenBay](const ChuyenBay& chuyenBay) {
        return chuyenBay.layMaChuyenBay() == maChuyenBay;
    });
}

bool AirlineManager::gheDangCho(const std::string& maChuyenBay, int soGhe) const {
    bool occupied = false;
    khachHangCho_.duyet([&](const KhachHang& customer) {
        if (customer.layMaChuyenBay() == maChuyenBay && customer.laySoGhe() == soGhe)
            occupied = true;
    });
    return occupied;
}

bool AirlineManager::luuMayBay(std::string& loi) const {
    const auto path = thuMucDuLieu_ / "MayBay.txt";
    std::ofstream file;
    if (!moFileGhi(path, file, loi)) return false;
    mayBay_.duyet([&file](const MayBay& plane) {
        file << plane.laySoHieu() << '|' << plane.laySoCho() << '\n';
    });
    return ghiThanhCong(file, path, loi);
}

bool AirlineManager::luuChuyenBay(std::string& loi) const {
    const auto path = thuMucDuLieu_ / "ChuyenBay.txt";
    std::ofstream file;
    if (!moFileGhi(path, file, loi)) return false;
    chuyenBay_.duyet([&file](const ChuyenBay& flight) {
        file << flight.layMaChuyenBay() << '|'
             << flight.laySoHieuMayBay() << '|'
             << flight.layNgayKhoiHanh() << '|'
             << flight.laySanBayDen() << '|'
             << flight.layTrangThai() << '|';
        bool first = true;
        flight.layDanhSachVe().duyet([&](const Ve& ve) {
            if (!first) file << ',';
            file << ve.layMaVe();
            first = false;
        });
        file << '|';
        first = true;
        flight.layGheTrong().duyet([&](int ghe) {
            if (!first) file << ',';
            file << ghe;
            first = false;
        });
        file << '\n';
    });
    return ghiThanhCong(file, path, loi);
}

bool AirlineManager::luuKhachHang(std::string& loi) const {
    const auto path = thuMucDuLieu_ / "KhachHang.txt";
    std::ofstream file;
    if (!moFileGhi(path, file, loi)) return false;
    khachHangCho_.duyet([&file](const KhachHang& customer) {
        file << customer.laySoThuTu() << '|'
             << customer.layCmnd() << '|'
             << customer.layHoTen() << '|'
             << customer.layMaChuyenBay() << '|'
             << customer.laySoGhe() << '\n';
    });
    return ghiThanhCong(file, path, loi);
}

bool AirlineManager::luuVe(const Ve& ve, std::string& loi) const {
    const auto path = thuMucDuLieu_ / "Ve" / (ve.layMaVe() + ".txt");
    if (std::filesystem::exists(path)) {
        loi = "File ve da ton tai, khong ghi de: " + path.string();
        return false;
    }
    std::ofstream file;
    if (!moFileGhi(path, file, loi)) return false;
    file << ve.layMaVe() << '|' << ve.layMaChuyenBay() << '|'
         << ve.layCmnd() << '|' << ve.layHoTen() << '|'
         << ve.laySoGhe() << '\n';
    return ghiThanhCong(file, path, loi);
}

bool AirlineManager::themMayBay(const std::string& soHieu, int soCho,
                                std::string& loi) {
    if (!Utils::hopLeMa(soHieu) || soCho < 1 || soCho > 1000) {
        loi = "So hieu chi gom chu/so/_/-; so cho trong [1,1000].";
        return false;
    }
    if (timMayBay(soHieu) != nullptr) {
        loi = "So hieu may bay da ton tai.";
        return false;
    }
    mayBay_.themCuoi(MayBay(soHieu, soCho));
    if (luuMayBay(loi)) return true;
    mayBay_.xoaDauTienNeu([&](const MayBay& mayBay) {
        return mayBay.laySoHieu() == soHieu;
    });
    return false;
}

bool AirlineManager::themChuyenBay(const std::string& maChuyenBay,
                                   const std::string& soHieuMayBay,
                                   const std::string& ngayKhoiHanh,
                                   const std::string& sanBayDen,
                                   std::string& loi) {
    if (!Utils::hopLeMa(maChuyenBay) || !Utils::hopLeNgay(ngayKhoiHanh) ||
        !Utils::hopLeVanBan(sanBayDen)) {
        loi = "Ma, ngay YYYY-MM-DD hoac san bay den khong hop le.";
        return false;
    }
    if (timChuyenBay(maChuyenBay) != nullptr) {
        loi = "Ma chuyen bay da ton tai.";
        return false;
    }
    const MayBay* plane = timMayBay(soHieuMayBay);
    if (plane == nullptr) {
        loi = "Khong tim thay may bay.";
        return false;
    }
    chuyenBay_.themCuoi(ChuyenBay(maChuyenBay, soHieuMayBay,
                                 ngayKhoiHanh, Utils::catKhoangTrang(sanBayDen),
                                 plane->laySoCho()));
    if (luuChuyenBay(loi)) return true;
    chuyenBay_.xoaDauTienNeu([&](const ChuyenBay& chuyenBay) {
        return chuyenBay.layMaChuyenBay() == maChuyenBay;
    });
    return false;
}

bool AirlineManager::capNhatTrangThaiChuyenBay(const std::string& maChuyenBay,
                                            int trangThai, std::string& loi) {
    ChuyenBay* flight = timChuyenBayDeSua(maChuyenBay);
    if (flight == nullptr) {
        loi = "Khong tim thay chuyen bay.";
        return false;
    }
    bool coKhachDangCho = false;
    khachHangCho_.duyet([&](const KhachHang& khach) {
        if (khach.layMaChuyenBay() == maChuyenBay) coKhachDangCho = true;
    });
    if (coKhachDangCho) {
        loi = "Con khach dang cho. Hay xu ly/huy yeu cau truoc khi doi trang thai.";
        return false;
    }
    const ChuyenBay trangThaiCu = *flight;
    if (!flight->capNhatTrangThai(trangThai, loi)) return false;
    if (luuChuyenBay(loi)) return true;
    *flight = trangThaiCu;
    return false;
}

bool AirlineManager::datVe(const std::string& maChuyenBay,
                           const std::string& cmnd, const std::string& hoTen,
                           int soGhe, std::string& loi) {
    const ChuyenBay* flight = timChuyenBay(maChuyenBay);
    if (flight == nullptr) {
        loi = "Khong tim thay chuyen bay.";
        return false;
    }
    if (flight->layTrangThai() != 1 || !flight->gheConTrong(soGhe)) {
        loi = "Chuyen bay khong con nhan ve hoac ghe khong trong.";
        return false;
    }
    if (!Utils::hopLeCmnd(cmnd) || !Utils::hopLeVanBan(hoTen)) {
        loi = "CMND/CCCD phai co 9 hoac 12 chu so; ten khong duoc rong.";
        return false;
    }
    if (flight->daCoKhach(cmnd)) {
        loi = "Khach hang nay da co ve tren chuyen bay.";
        return false;
    }
    if (gheDangCho(maChuyenBay, soGhe)) {
        loi = "Ghe nay da co nguoi dang cho xu ly.";
        return false;
    }
    bool duplicate = false;
    khachHangCho_.duyet([&](const KhachHang& customer) {
        if (customer.layMaChuyenBay() == maChuyenBay && customer.layCmnd() == cmnd)
            duplicate = true;
    });
    if (duplicate) {
        loi = "Khach hang da co yeu cau cho tren chuyen bay.";
        return false;
    }
    Queue<KhachHang> hangDoiCu = khachHangCho_;
    khachHangCho_.vaoHang(KhachHang(soThuTuTiepTheo_, cmnd,
                                 Utils::catKhoangTrang(hoTen), maChuyenBay, soGhe));
    if (luuKhachHang(loi)) {
        ++soThuTuTiepTheo_;
        return true;
    }
    khachHangCho_ = hangDoiCu;
    return false;
}

bool AirlineManager::xuLyDatVe(std::string& maVe, std::string& loi) {
    const KhachHang* customer = khachHangCho_.dauHang();
    if (customer == nullptr) {
        loi = "Hang doi khach hang dang rong.";
        return false;
    }
    ChuyenBay* flight = timChuyenBayDeSua(customer->layMaChuyenBay());
    if (flight == nullptr) {
        loi = "Khong tim thay chuyen bay cua khach dang cho.";
        return false;
    }
    Ve ve(customer->layMaChuyenBay(), customer->layCmnd(),
          customer->layHoTen(), customer->laySoGhe());
    const ChuyenBay chuyenBayCu = *flight;
    const Queue<KhachHang> hangDoiCu = khachHangCho_;
    if (!flight->themVe(ve, loi)) return false;
    if (!luuVe(ve, loi)) {
        *flight = chuyenBayCu;
        return false;
    }
    khachHangCho_.raHang();
    if (luuChuyenBay(loi) && luuKhachHang(loi)) {
        maVe = ve.layMaVe();
        return true;
    }
    // Khong de du lieu trong RAM bi cap nhat nua chung khi ghi file loi.
    *flight = chuyenBayCu;
    khachHangCho_ = hangDoiCu;
    std::error_code ec;
    std::filesystem::remove(thuMucDuLieu_ / "Ve" / (ve.layMaVe() + ".txt"), ec);
    std::string loiKhoiPhuc;
    if (!luuChuyenBay(loiKhoiPhuc) || !luuKhachHang(loiKhoiPhuc)) {
        loi += "; khong khoi phuc duoc file: " + loiKhoiPhuc;
    }
    return false;
}

bool AirlineManager::huyYeuCauDatVe(std::string& loi) {
    Queue<KhachHang> hangDoiCu = khachHangCho_;
    if (!khachHangCho_.raHang()) {
        loi = "Hang doi khach hang dang rong.";
        return false;
    }
    if (luuKhachHang(loi)) return true;
    khachHangCho_ = hangDoiCu;
    return false;
}

bool AirlineManager::traVe(const std::string& maVe, std::string& loi) {
    ChuyenBay* selected = nullptr;
    chuyenBay_.duyet([&](ChuyenBay& flight) {
        if (selected == nullptr && flight.timVe(maVe) != nullptr) selected = &flight;
    });
    if (selected == nullptr) {
        loi = "Khong tim thay ma ve.";
        return false;
    }
    if (selected->layTrangThai() == 3) {
        loi = "Chuyen bay da hoan tat, khong the tra ve.";
        return false;
    }
    const auto path = thuMucDuLieu_ / "Ve" / (maVe + ".txt");
    if (!std::filesystem::exists(path)) {
        loi = "File ve khong ton tai.";
        return false;
    }
    const ChuyenBay chuyenBayCu = *selected;
    if (!selected->traVe(maVe)) {
        loi = "Khong the tra ve.";
        return false;
    }
    if (!luuChuyenBay(loi)) {
        *selected = chuyenBayCu;
        return false;
    }
    std::error_code ec;
    const bool daXoa = std::filesystem::remove(path, ec);
    if (!daXoa || ec) {
        loi = "Khong xoa duoc file ve: " + path.string();
        *selected = chuyenBayCu;
        std::string loiKhoiPhuc;
        if (!luuChuyenBay(loiKhoiPhuc)) loi += "; " + loiKhoiPhuc;
        return false;
    }
    return true;
}

int AirlineManager::thongKeSoChuyenBay(const std::string& soHieu, bool chiHoanTat) const {
    int total = 0;
    chuyenBay_.duyet([&](const ChuyenBay& flight) {
        if (flight.laySoHieuMayBay() == soHieu &&
            (!chiHoanTat || flight.layTrangThai() == 3)) ++total;
    });
    return total;
}
