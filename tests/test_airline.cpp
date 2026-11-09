#include "AirlineManager.h"
#include "LinkedList.h"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

int main() {
    namespace fs = std::filesystem;
    LinkedList<int> numbers;
    numbers.themCuoi(5);
    numbers.themCuoi(1);
    numbers.themCuoi(3);
    numbers.sapXep([](int a, int b) { return a < b; });
    assert(numbers.dauDanhSach() && *numbers.dauDanhSach() == 1);
    assert(numbers.xoaDauTienNeu([](int n) { return n == 3; }));
    assert(numbers.kichThuoc() == 2);

    const fs::path folder = fs::temp_directory_path() / "ctdl5_test_manager_2026";
    fs::remove_all(folder);
    fs::create_directories(folder);
    { std::ofstream admin(folder / "Admin.txt"); admin << "admin|pw\n"; }
    { std::ofstream planes(folder / "MayBay.txt"); }
    { std::ofstream flights(folder / "ChuyenBay.txt"); }
    { std::ofstream waiting(folder / "KhachHang.txt"); }

    std::string msg;
    AirlineManager manager(folder);
    assert(manager.docDuLieu(msg));
    assert(manager.dangNhap("admin", "pw"));
    assert(!manager.dangNhap("admin", "no"));
    assert(!manager.themMayBay("!!", 2, msg));
    assert(manager.themMayBay("MBTEST", 2, msg));
    assert(!manager.themMayBay("MBTEST", 2, msg));
    assert(!manager.themChuyenBay("CBTEST", "MBTEST", "2026-02-30", "Ha Noi", msg));
    assert(manager.themChuyenBay("CBTEST", "MBTEST", "2026-12-31", "Ha Noi", msg));
    assert(!manager.themChuyenBay("CBTEST", "MBTEST", "2026-12-31", "Ha Noi", msg));
    assert(!manager.datVe("CBTEST", "123456789", "    ", 1, msg));
    assert(!manager.datVe("CBTEST", "abc", "A", 1, msg));
    assert(!manager.datVe("CBTEST", "123456789", "A", 3, msg));
    assert(manager.datVe("CBTEST", "123456789", "Nguyen Van A", 1, msg));
    assert(!manager.datVe("CBTEST", "123456780", "Nguoi khac", 1, msg));
    assert(!manager.datVe("CBTEST", "123456789", "Nguoi khac", 2, msg));
    assert(manager.datVe("CBTEST", "123456780", "Nguyen Van B", 2, msg));
    assert(manager.layHangDoiKhachHang().kichThuoc() == 2);

    std::string ticket;
    assert(manager.xuLyDatVe(ticket, msg));
    assert(ticket == "CBTEST_1");
    assert(fs::exists(folder / "Ve" / "CBTEST_1.txt"));
    assert(manager.timChuyenBay("CBTEST")->layTrangThai() == 1);
    assert(manager.xuLyDatVe(ticket, msg));
    assert(ticket == "CBTEST_2");
    assert(manager.timChuyenBay("CBTEST")->layTrangThai() == 2);
    assert(manager.timChuyenBay("CBTEST")->laySoGheTrong() == 0);
    assert(manager.layHangDoiKhachHang().rong());
    assert(!manager.datVe("CBTEST", "555555555", "Nguoi C", 1, msg));
    assert(manager.traVe("CBTEST_1", msg));
    assert(!fs::exists(folder / "Ve" / "CBTEST_1.txt"));
    assert(manager.timChuyenBay("CBTEST")->layTrangThai() == 1);
    assert(manager.timChuyenBay("CBTEST")->gheConTrong(1));

    AirlineManager loaded(folder);
    assert(loaded.docDuLieu(msg));
    assert(loaded.timChuyenBay("CBTEST")->laySoVe() == 1);
    assert(loaded.timChuyenBay("CBTEST")->gheConTrong(1));
    assert(loaded.timChuyenBay("CBTEST")->layTrangThai() == 1);
    assert(loaded.capNhatTrangThaiChuyenBay("CBTEST", 3, msg));
    assert(loaded.thongKeSoChuyenBay("MBTEST", true) == 1);
    assert(loaded.thongKeSoChuyenBay("MBTEST", false) == 1);
    assert(!loaded.traVe("CBTEST_2", msg));
    assert(!loaded.datVe("CBTEST", "555555555", "C", 1, msg));
    assert(!loaded.capNhatTrangThaiChuyenBay("CBTEST", 0, msg));

    assert(loaded.themChuyenBay("CBNEW", "MBTEST", "2026-12-30", "Hue", msg));
    assert(loaded.datVe("CBNEW", "888888888", "D", 1, msg));
    assert(loaded.layHangDoiKhachHang().kichThuoc() == 1);
    // Yeu cau dang cho phai duoc doc lai, va khong duoc hoan tat chuyen bay khi con cho.
    AirlineManager taiLaiHangDoi(folder);
    assert(taiLaiHangDoi.docDuLieu(msg));
    assert(taiLaiHangDoi.layHangDoiKhachHang().kichThuoc() == 1);
    assert(taiLaiHangDoi.layHangDoiKhachHang().dauHang()->layCmnd() == "888888888");
    assert(!taiLaiHangDoi.capNhatTrangThaiChuyenBay("CBNEW", 3, msg));
    assert(taiLaiHangDoi.timChuyenBay("CBNEW")->layTrangThai() == 1);
    assert(loaded.huyYeuCauDatVe(msg));
    assert(loaded.layHangDoiKhachHang().rong());
    assert(loaded.capNhatTrangThaiChuyenBay("CBNEW", 0, msg));
    assert(!loaded.datVe("CBNEW", "888888888", "D", 1, msg));

    // Thieu file du lieu phai bao loi, khong am tham coi danh sach la rong.
    fs::remove(folder / "MayBay.txt");
    AirlineManager thieuFile(folder);
    assert(!thieuFile.docDuLieu(msg));

    fs::remove_all(folder);
    std::cout << "ALL TESTS PASSED\n";
    return 0;
}
