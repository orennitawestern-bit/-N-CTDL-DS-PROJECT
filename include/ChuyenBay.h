#ifndef CHUYEN_BAY_H
#define CHUYEN_BAY_H

#include "LinkedList.h"
#include "Ve.h"

#include <string>

class ChuyenBay {
private:
    std::string maChuyenBay_;
    std::string soHieuMayBay_;
    std::string ngayKhoiHanh_;
    std::string sanBayDen_;
    int trangThai_;
    int soCho_;
    LinkedList<Ve> danhSachVe_;
    LinkedList<int> gheTrong_;

    void capNhatTrangThaiTuGhe();

public:
    ChuyenBay();
    ChuyenBay(const std::string& maChuyenBay, const std::string& soHieuMayBay,
              const std::string& ngayKhoiHanh, const std::string& sanBayDen,
              int soCho);

    const std::string& layMaChuyenBay() const;
    const std::string& laySoHieuMayBay() const;
    const std::string& layNgayKhoiHanh() const;
    const std::string& laySanBayDen() const;
    int layTrangThai() const;
    int laySoCho() const;
    std::size_t laySoVe() const;
    std::size_t laySoGheTrong() const;
    const LinkedList<Ve>& layDanhSachVe() const;
    const LinkedList<int>& layGheTrong() const;

    bool gheConTrong(int soGhe) const;
    bool daCoKhach(const std::string& cmnd) const;
    const Ve* timVe(const std::string& maVe) const;
    bool themVe(const Ve& ve, std::string& loi);
    bool themVeKhiDocFile(const Ve& ve);
    bool traVe(const std::string& maVe);
    bool capNhatTrangThai(int trangThai, std::string& loi);
    void phucHoiTrangThai(int trangThai);
};

#endif
