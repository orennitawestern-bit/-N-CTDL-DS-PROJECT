#ifndef AIRLINE_MANAGER_H
#define AIRLINE_MANAGER_H

#include "ChuyenBay.h"
#include "KhachHang.h"
#include "MayBay.h"
#include "LinkedList.h"
#include "Queue.h"

#include <filesystem>
#include <string>

class AirlineManager {
private:
    std::filesystem::path thuMucDuLieu_;
    LinkedList<MayBay> mayBay_;
    LinkedList<ChuyenBay> chuyenBay_;
    Queue<KhachHang> khachHangCho_;
    int soThuTuTiepTheo_;
    std::string taiKhoanAdmin_;
    std::string matKhauAdmin_;

    ChuyenBay* timChuyenBayDeSua(const std::string& maChuyenBay);
    bool luuMayBay(std::string& loi) const;
    bool luuChuyenBay(std::string& loi) const;
    bool luuKhachHang(std::string& loi) const;
    bool luuVe(const Ve& ve, std::string& loi) const;

public:
    explicit AirlineManager(const std::filesystem::path& thuMucDuLieu);
    bool docDuLieu(std::string& thongBao);
    bool dangNhap(const std::string& user, const std::string& password) const;

    const LinkedList<MayBay>& layDanhSachMayBay() const;
    const LinkedList<ChuyenBay>& layDanhSachChuyenBay() const;
    const Queue<KhachHang>& layHangDoiKhachHang() const;
    const MayBay* timMayBay(const std::string& soHieu) const;
    const ChuyenBay* timChuyenBay(const std::string& maChuyenBay) const;
    bool gheDangCho(const std::string& maChuyenBay, int soGhe) const;

    bool themMayBay(const std::string& soHieu, int soCho, std::string& loi);
    bool themChuyenBay(const std::string& maChuyenBay, const std::string& soHieuMayBay,
                      const std::string& ngayKhoiHanh, const std::string& sanBayDen,
                      std::string& loi);
    bool capNhatTrangThaiChuyenBay(const std::string& maChuyenBay, int trangThai,
                              std::string& loi);
    bool datVe(const std::string& maChuyenBay, const std::string& cmnd,
               const std::string& hoTen, int soGhe, std::string& loi);
    bool xuLyDatVe(std::string& maVe, std::string& loi);
    bool huyYeuCauDatVe(std::string& loi);
    bool traVe(const std::string& maVe, std::string& loi);
    int thongKeSoChuyenBay(const std::string& soHieu, bool chiHoanTat) const;
};

#endif
