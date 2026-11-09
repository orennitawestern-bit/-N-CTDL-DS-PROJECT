// Thu tu include theo yeu cau cua nhom.
#include "ChuyenBay.h"
#include "KhachHang.h"
#include "MayBay.h"
#include "AirlineManager.h"

#include "Utils.h"

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

namespace {

constexpr const char* MAU_XANH = "\033[1;36m";
constexpr const char* MAU_LUC = "\033[1;32m";
constexpr const char* MAU_DO = "\033[1;31m";
constexpr const char* MAU_VANG = "\033[1;33m";
constexpr const char* MAU_RESET = "\033[0m";

void batMauConsole() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    const HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (handle != INVALID_HANDLE_VALUE && GetConsoleMode(handle, &mode)) {
        SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
#endif
}

void xoaManHinh() {
    std::cout << "\033[2J\033[H";
}

void tieuDe(const std::string& name) {
    std::cout << MAU_XANH
              << "\n===============================================================\n"
              << "  " << name << '\n'
              << "===============================================================\n"
              << MAU_RESET;
}

void baoKetQua(bool thanhCong, const std::string& noiDung) {
    std::cout << (thanhCong ? MAU_LUC : MAU_DO)
              << (thanhCong ? "[THANH CONG] " : "[LOI] ")
              << noiDung << MAU_RESET << '\n';
}

std::string nhapDong(const std::string& prompt) {
    std::cout << prompt;
    std::string input;
    if (!std::getline(std::cin, input)) {
        std::cin.clear();
        std::cout << '\n';
        std::exit(0);
    }
    return Utils::catKhoangTrang(input);
}

int nhapSo(const std::string& prompt, int minValue, int maxValue) {
    for (;;) {
        const std::string input = nhapDong(prompt);
        int answer = 0;
        if (Utils::chuyenSoNguyen(input, answer) && answer >= minValue && answer <= maxValue)
            return answer;
        std::cout << MAU_DO << "Nhap so trong khoang [" << minValue
                  << ", " << maxValue << "].\n" << MAU_RESET;
    }
}

void tamDung() {
    std::cout << MAU_VANG << "\nNhan Enter de tiep tuc..." << MAU_RESET;
    std::string input;
    std::getline(std::cin, input);
}

std::string nhapMatKhau(bool& nhanEsc) {
    nhanEsc = false;
    std::cout << "Password (ESC de thoat): ";
#ifdef _WIN32
    std::string password;
    while (true) {
        const int c = _getch();
        if (c == 27) { nhanEsc = true; std::cout << '\n'; return ""; }
        if (c == '\r' || c == '\n') { std::cout << '\n'; return password; }
        if (c == 8 || c == 127) {
            if (!password.empty()) {
                password.pop_back();
                std::cout << "\b \b" << std::flush;
            }
        } else if (c >= 32 && c < 127) {
            password.push_back(static_cast<char>(c));
            std::cout << '*' << std::flush;
        }
    }
#else
    if (!isatty(STDIN_FILENO)) {
        std::string input;
        std::getline(std::cin, input);
        nhanEsc = (input == "ESC" || input == "\033");
        return input;
    }
    termios oldSettings{};
    if (tcgetattr(STDIN_FILENO, &oldSettings) != 0) return nhapDong("");
    termios newSettings = oldSettings;
    newSettings.c_lflag &= static_cast<tcflag_t>(~(ICANON | ECHO));
    tcsetattr(STDIN_FILENO, TCSANOW, &newSettings);
    std::string password;
    while (true) {
        const int c = std::getchar();
        if (c == 27 || c == EOF) {
            nhanEsc = true;
            break;
        }
        if (c == '\r' || c == '\n') break;
        if (c == 8 || c == 127) {
            if (!password.empty()) {
                password.pop_back();
                std::cout << "\b \b" << std::flush;
            }
        } else if (c >= 32 && c <= 126) {
            password.push_back(static_cast<char>(c));
            std::cout << '*' << std::flush;
        }
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldSettings);
    std::cout << '\n';
    return password;
#endif
}

void hienThiMayBay(const AirlineManager& quanLy) {
    tieuDe("DANH SACH MAY BAY");
    std::cout << "+----------------------+------------+\n"
              << "| So hieu may bay      | So cho     |\n"
              << "+----------------------+------------+\n";
    quanLy.layDanhSachMayBay().duyet([](const MayBay& mayBay) {
        std::cout << "| " << std::left << std::setw(20) << mayBay.laySoHieu()
                  << " | " << std::right << std::setw(10) << mayBay.laySoCho() << " |\n";
    });
    std::cout << "+----------------------+------------+\n";
}

void hienThiChuyenBay(const AirlineManager& quanLy) {
    tieuDe("DANH SACH CHUYEN BAY");
    std::cout << "+------------+------------+------------+-------------------+-------------+-------+-------+\n"
              << "| Ma CB      | May bay    | Ngay di    | San bay den        | Trang thai  | Ve    | Trong |\n"
              << "+------------+------------+------------+-------------------+-------------+-------+-------+\n";
    quanLy.layDanhSachChuyenBay().duyet([](const ChuyenBay& flight) {
        std::cout << "| " << std::left << std::setw(10) << flight.layMaChuyenBay()
                  << " | " << std::setw(10) << flight.laySoHieuMayBay()
                  << " | " << std::setw(10) << flight.layNgayKhoiHanh()
                  << " | " << std::setw(17) << flight.laySanBayDen()
                  << " | " << std::setw(11) << Utils::tenTrangThai(flight.layTrangThai())
                  << " | " << std::right << std::setw(5) << flight.laySoVe()
                  << " | " << std::setw(5) << flight.laySoGheTrong() << " |\n";
    });
    std::cout << "+------------+------------+------------+-------------------+-------------+-------+-------+\n";
    std::cout << "Danh sach ma ve theo tung chuyen bay:\n";
    quanLy.layDanhSachChuyenBay().duyet([](const ChuyenBay& flight) {
        std::cout << "  " << flight.layMaChuyenBay() << ": ";
        bool first = true;
        flight.layDanhSachVe().duyet([&](const Ve& ve) {
            if (!first) std::cout << ", ";
            std::cout << ve.layMaVe();
            first = false;
        });
        if (first) std::cout << "(chua co ve)";
        std::cout << '\n';
    });
}

void hienThiGheTrong(const AirlineManager& quanLy, const ChuyenBay& flight,
                bool chiGheChuaCho) {
    std::cout << "Ghe trong (" << flight.laySoGheTrong() << "): ";
    int count = 0;
    flight.layGheTrong().duyet([&](int seat) {
        if (!chiGheChuaCho || !quanLy.gheDangCho(flight.layMaChuyenBay(), seat)) {
            std::cout << seat << ' ';
            ++count;
        }
    });
    if (count == 0) std::cout << "(khong co)";
    std::cout << '\n';
    if (chiGheChuaCho)
        std::cout << "Ghe da co yeu cau cho se khong hien thi o danh sach tren.\n";
}

void hienThiKhachHangCho(const AirlineManager& quanLy) {
    tieuDe("HANG DOI KHACH HANG CHO MUA VE (FIFO)");
    std::cout << "STT | CMND/CCCD    | Ho ten                  | Ma CB   | So ghe\n";
    quanLy.layHangDoiKhachHang().duyet([](const KhachHang& khach) {
        std::cout << std::setw(3) << khach.laySoThuTu() << " | "
                  << std::setw(12) << khach.layCmnd() << " | "
                  << std::left << std::setw(23) << khach.layHoTen()
                  << " | " << std::setw(7) << khach.layMaChuyenBay()
                  << " | " << khach.laySoGhe() << std::right << '\n';
    });
    if (quanLy.layHangDoiKhachHang().rong()) std::cout << "(Danh sach rong)\n";
}

void datVe(AirlineManager& quanLy) {
    xoaManHinh();
    hienThiChuyenBay(quanLy);
    tieuDe("GHI NHAN YEU CAU DAT VE");
    const std::string code = nhapDong("Ma chuyen bay: ");
    const ChuyenBay* flight = quanLy.timChuyenBay(code);
    if (flight == nullptr) {
        baoKetQua(false, "Ma chuyen bay khong ton tai.");
        return;
    }
    if (flight->layTrangThai() != 1) {
        baoKetQua(false, "Chuyen bay khong nhan dat ve.");
        return;
    }
    hienThiGheTrong(quanLy, *flight, true);
    const int seat = nhapSo("Chon so ghe: ", 1, flight->laySoCho());
    const std::string cmnd = nhapDong("CMND/CCCD (9 hoac 12 chu so): ");
    const std::string name = nhapDong("Ho ten: ");
    std::string message;
    const bool result = quanLy.datVe(code, cmnd, name, seat, message);
    baoKetQua(result, result ? "Da them vao hang doi. Admin se xu ly va phat hanh ve."
                               : message);
}

void xuLyDatVe(AirlineManager& quanLy) {
    hienThiKhachHangCho(quanLy);
    if (quanLy.layHangDoiKhachHang().rong()) return;
    std::cout << "1. Duyet yeu cau dau hang va cap ve\n"
              << "2. Huy yeu cau dau hang\n"
              << "0. Quay lai\n";
    const int option = nhapSo("Chon: ", 0, 2);
    std::string message;
    std::string maVe;
    if (option == 1) {
        const bool result = quanLy.xuLyDatVe(maVe, message);
        baoKetQua(result, result ? "Da cap ve: " + maVe : message);
    } else if (option == 2) {
        const bool result = quanLy.huyYeuCauDatVe(message);
        baoKetQua(result, result ? "Da huy yeu cau cho dau hang." : message);
    }
}

void thongKe(AirlineManager& quanLy) {
    for (;;) {
        xoaManHinh();
        tieuDe("THONG KE");
        std::cout << "1. Danh sach khach (ve) theo ma chuyen bay\n"
                  << "2. Danh sach ghe trong theo ma chuyen bay\n"
                  << "3. So lan thuc hien chuyen bay cua may bay\n"
                  << "0. Quay lai\n";
        const int option = nhapSo("Chon: ", 0, 3);
        if (option == 0) return;
        if (option == 3) {
            const std::string code = nhapDong("So hieu may bay: ");
            if (quanLy.timMayBay(code) == nullptr) {
                baoKetQua(false, "Khong tim thay may bay.");
            } else {
                std::cout << "So chuyen da hoan tat (trang thai 3): "
                          << quanLy.thongKeSoChuyenBay(code, true) << '\n'
                          << "Tong so chuyen duoc phan cong: "
                          << quanLy.thongKeSoChuyenBay(code, false) << '\n';
            }
        } else {
            const std::string code = nhapDong("Ma chuyen bay: ");
            const ChuyenBay* flight = quanLy.timChuyenBay(code);
            if (flight == nullptr) {
                baoKetQua(false, "Khong tim thay chuyen bay.");
            } else if (option == 1) {
                std::cout << "Ma ve          | CMND/CCCD    | Ho ten                 | So ghe\n";
                flight->layDanhSachVe().duyet([](const Ve& ve) {
                    std::cout << std::left << std::setw(14) << ve.layMaVe() << " | "
                              << std::setw(12) << ve.layCmnd() << " | "
                              << std::setw(22) << ve.layHoTen() << " | "
                              << ve.laySoGhe() << '\n';
                });
                if (flight->laySoVe() == 0) std::cout << "(Chua co khach)\n";
            } else hienThiGheTrong(quanLy, *flight, false);
        }
        tamDung();
    }
}

void themMayBay(AirlineManager& quanLy) {
    const std::string code = nhapDong("So hieu may bay moi: ");
    const int seats = nhapSo("So cho (1..1000): ", 1, 1000);
    std::string message;
    const bool result = quanLy.themMayBay(code, seats, message);
    baoKetQua(result, result ? "Da them may bay." : message);
}

void themChuyenBay(AirlineManager& quanLy) {
    hienThiMayBay(quanLy);
    const std::string code = nhapDong("Ma chuyen bay moi: ");
    const std::string plane = nhapDong("So hieu may bay: ");
    const std::string date = nhapDong("Ngay khoi hanh (YYYY-MM-DD): ");
    const std::string arrival = nhapDong("San bay den: ");
    std::string message;
    const bool result = quanLy.themChuyenBay(code, plane, date, arrival, message);
    baoKetQua(result, result ? "Da them chuyen bay." : message);
}

void menuQuanLy(AirlineManager& quanLy) {
    for (;;) {
        xoaManHinh();
        tieuDe("MENU QUAN LY - NHAN VIEN BAN HANG");
        std::cout << "1. Xu ly dat ve (FIFO)\n"
                  << "2. Xu ly tra ve\n"
                  << "3. Thong ke\n"
                  << "4. Them may bay\n"
                  << "5. Them chuyen bay\n"
                  << "6. Huy/hoan tat chuyen bay\n"
                  << "7. Hien thi danh sach may bay\n"
                  << "8. Hien thi danh sach khach dang cho\n"
                  << "0. Dang xuat\n";
        const int option = nhapSo("Chon: ", 0, 8);
        if (option == 0) return;
        xoaManHinh();
        if (option == 1) xuLyDatVe(quanLy);
        if (option == 2) {
            tieuDe("TRA VE");
            const std::string ticket = nhapDong("Ma ve can tra: ");
            std::string message;
            const bool result = quanLy.traVe(ticket, message);
            baoKetQua(result, result ? "Da tra ve, ghe duoc mo lai." : message);
        }
        if (option == 3) thongKe(quanLy);
        if (option == 4) themMayBay(quanLy);
        if (option == 5) themChuyenBay(quanLy);
        if (option == 6) {
            tieuDe("CAP NHAT TRANG THAI");
            const std::string code = nhapDong("Ma chuyen bay: ");
            const int status = nhapSo("0 = Huy, 3 = Hoan tat: ", 0, 3);
            std::string message;
            const bool result = quanLy.capNhatTrangThaiChuyenBay(code, status, message);
            baoKetQua(result, result ? "Da cap nhat trang thai." : message);
        }
        if (option == 7) hienThiMayBay(quanLy);
        if (option == 8) hienThiKhachHangCho(quanLy);
        tamDung();
    }
}

// Tra ve true neu sai mat khau 3 lan: dung chuong trinh.
bool dangNhapQuanLy(AirlineManager& quanLy) {
    for (int attempt = 1; attempt <= 3; ++attempt) {
        xoaManHinh();
        tieuDe("DANG NHAP HE THONG");
        std::cout << "Lan nhap " << attempt << "/3. Go ESC o User hoac bam ESC o Password de thoat.\n";
        const std::string user = nhapDong("User: ");
        if (user == "ESC" || user == "\033") return false;
        bool esc = false;
        const std::string password = nhapMatKhau(esc);
        if (esc) return false;
        if (quanLy.dangNhap(user, password)) {
            baoKetQua(true, "Dang nhap thanh cong.");
            menuQuanLy(quanLy);
            return false;
        }
        baoKetQua(false, "Sai user hoac password.");
        if (attempt < 3) tamDung();
    }
    baoKetQua(false, "Sai 3 lan. He thong tu dong thoat.");
    return true;
}

} // namespace

int main(int argc, char* argv[]) {
    batMauConsole();
    const std::string folder = (argc >= 2) ? argv[1] : "data";
    AirlineManager quanLy(folder);
    std::string info;
    if (!quanLy.docDuLieu(info)) {
        baoKetQua(false, info);
        std::cout << "Hay chay chuong trinh tai thu muc goc cua du an.\n";
        return 1;
    }
    std::cout << MAU_VANG << info << MAU_RESET << '\n';
    for (;;) {
        xoaManHinh();
        tieuDe("QUAN LY CHUYEN BAY NOI DIA - PROJECT CTDL 5");
        std::cout << "1. Hien thi danh sach chuyen bay\n"
                  << "2. Dat ve (vao hang cho)\n"
                  << "3. Quan ly (dang nhap)\n"
                  << "0. Thoat\n";
        const int option = nhapSo("Chon: ", 0, 3);
        if (option == 0) break;
        xoaManHinh();
        if (option == 1) hienThiChuyenBay(quanLy);
        if (option == 2) datVe(quanLy);
        if (option == 3 && dangNhapQuanLy(quanLy)) return 0;
        tamDung();
    }
    std::cout << MAU_LUC << "Tam biet!\n" << MAU_RESET;
    return 0;
}
