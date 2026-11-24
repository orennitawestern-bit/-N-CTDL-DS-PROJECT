#include "ConsoleUI.h"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string_view>
#include <iterator>
#include <thread>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <conio.h>
#else
#include <poll.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#endif

namespace ConsoleUI {
namespace {
bool ansi = false;
bool banPhim = false;

struct KichThuoc { int rong, cao; };
KichThuoc kichThuoc() {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO info{};
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info))
        return {info.srWindow.Right - info.srWindow.Left + 1,
                info.srWindow.Bottom - info.srWindow.Top + 1};
#else
    winsize size{};
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) == 0 && size.ws_col && size.ws_row)
        return {size.ws_col, size.ws_row};
#endif
    return {80, 25};
}

// Font 3 x 5, moi ky tu luu nam hang bit tu tren xuong.
const char* chu(char c) {
    static const char* const font[] = {
        "25755","65656","34443","65556","74647","74644","34553",
        "55755","72227","11152","55655","44447","57755","57775",
        "25552","65644","25573","65655","34216","72222","55557",
        "55552","55775","55255","55222","71247"
    };
    if (c >= 'A' && c <= 'Z') return font[c - 'A'];
    if (c == '-') return "00700";
    if (c == '(') return "24442";
    if (c == ')') return "21112";
    return "00000";
}

void mau(std::ostream& out, int x, int frame) {
    const int shade = std::abs((x + frame) % 64 - 32);
    out << "\033[1;38;2;" << 69 + 4 * shade << ';' << 91 + 2 * (32 - shade)
        << ";255m";
}

void canGiua(std::ostream& out, int rong, int dai) {
    out << std::string(std::max(0, (rong - dai) / 2), ' ');
}

int veTieuDe(std::ostream& out, const std::string& ten, KichThuoc size,
             int frame, bool trangChu) {
    const int rong = std::max(1, size.rong - 1);
    const int khung = std::min(111, rong);
    auto vien = [&] {
        canGiua(out, rong, khung);
        out << (ansi ? "\033[38;2;103;111;208m" : "")
            << std::string(khung, '=') << (ansi ? "\033[0m" : "") << '\n';
    };
    vien();
    int rows = 2;
    if (!ansi || khung < 36 || size.cao < 18 ||
        ten.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ -()") != std::string::npos) {
        canGiua(out, rong, static_cast<int>(ten.size()));
        if (ansi) mau(out, 0, frame);
        out << ten << (ansi ? "\033[0m" : "") << '\n';
        ++rows;
    } else {
        std::vector<std::string> lines;
        std::istringstream words(ten);
        for (std::string word; words >> word;) {
            if (lines.empty() || (lines.back().size() + 1 + word.size()) * 4 > static_cast<size_t>(khung))
                lines.push_back(word);
            else lines.back() += " " + word;
        }
        // Hai diem anh tren moi o console. Bong lech mot cot xuong ben phai.
        for (const auto& line : lines) {
            const int width = static_cast<int>(line.size()) * 4;
            auto pixel = [&](int x, int y) {
                return x >= 0 && x < width && y >= 0 && y < 5 && x % 4 < 3 &&
                       ((chu(line[x / 4])[y] - '0') & (4 >> (x % 4)));
            };
            for (int y = 0; y < 6; y += 2) {
                canGiua(out, rong, width);
                for (int x = 0; x < width; ++x) {
                    const int mask = pixel(x,y) + 2 * pixel(x,y+1);
                    const int shadow = pixel(x-1,y-1) + 2 * pixel(x-1,y);
                    if (mask) mau(out, x, frame);
                    else if (shadow) out << "\033[38;2;65;47;112m";
                    static const char* const blocks[] = {" ", u8"▀", u8"▄", u8"█"};
                    out << blocks[mask ? mask : shadow];
                }
                out << "\033[0m\n";
            }
            rows += 3;
        }
    }
    if (trangChu) {
        canGiua(out, rong, 9);
        out <<"-DOWNCODER-\n";
        ++rows;
    }
    vien();
    return rows;
}

int veMayBay(std::ostream& out, int rong, int soDong, int frame) {
    // Side-to-side symmetry in plan view, nose pointing right.
    static constexpr std::string_view canhVaThan[] = {
        "         ##",
        "          ###",
        "           ####",
        "            #####  ###",
        "  ##         #########",
        "   ###        #######",
        "    ####       ########",
        "     #####################",
        "#############################"
    };
    constexpr int nuaCao = static_cast<int>(std::size(canhVaThan));
    constexpr int daiGoc = static_cast<int>(canhVaThan[nuaCao - 1].size());
    if (rong < 2 || soDong < 1) return 0;
    const int khung = std::min(111, rong);
    const int cao = std::min({nuaCao, soDong, khung * nuaCao / daiGoc});
    const bool dayDu = cao >= 3;
    const int dai = dayDu ? daiGoc * cao / nuaCao : 2;
    const int x = (rong - khung) / 2 + frame % (khung - dai + 1);
    const std::string le(x, ' ');
    auto diem = [&](int y, int cot) {
        const int hang = std::min(y, 2 * cao - 1 - y) * (nuaCao - 1) / (cao - 1);
        const auto dong = canhVaThan[hang];
        const int pixel = cot * (daiGoc - 1) / (dai - 1);
        return pixel < static_cast<int>(dong.size()) && dong[pixel] == '#';
    };
    constexpr const char* khoi[] = {" ", u8"\u2580", u8"\u2584", u8"\u2588"};
    const int soDongVe = dayDu ? cao : 1;
    for (int hang = 0; hang < soDongVe; ++hang) {
        out << le;
        if (ansi) out << "\033[1;38;2;177;214;255m";
        if (dayDu) {
            for (int cot = 0; cot < dai; ++cot)
                out << khoi[diem(2 * hang, cot) + 2 * diem(2 * hang + 1, cot)];
        } else {
            out << u8"\u2708";
        }
        out << (ansi ? "\033[0m\033[K\n" : "\n");
    }
    return soDongVe;
}

enum class Phim { cho, len, xuong, nhan, thoat };
#ifndef _WIN32
struct CheDoPhim {
    termios cu{};
    bool doi = false;
    CheDoPhim() {
        if (tcgetattr(STDIN_FILENO, &cu) == 0) {
            termios moi = cu;
            moi.c_lflag &= ~(ICANON | ECHO);
            doi = tcsetattr(STDIN_FILENO, TCSANOW, &moi) == 0;
        }
    }
    ~CheDoPhim() { if (doi) tcsetattr(STDIN_FILENO, TCSANOW, &cu); }
};
int docByte(int ms) {
    pollfd input{STDIN_FILENO, POLLIN, 0};
    if (poll(&input, 1, ms) <= 0) return -2;
    unsigned char c;
    return read(STDIN_FILENO, &c, 1) == 1 ? c : -1;
}
#endif
Phim docPhim() {
#ifdef _WIN32
    if (!_kbhit()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
        return Phim::cho;
    }
    const int c = _getch();
    if (c == 0 || c == 224) {
        const int arrow = _getch();
        if (arrow == 72) return Phim::len;
        if (arrow == 80) return Phim::xuong;
    }
#else
    const int c = docByte(30);
    if (c == -1) return Phim::thoat;
    if (c == 27) {
        const int prefix = docByte(70);
        if (prefix != '[' && prefix != 'O') return Phim::thoat;
        const int arrow = docByte(70);
        if (arrow == 'A') return Phim::len;
        if (arrow == 'B') return Phim::xuong;
        return Phim::cho;
    }
#endif
    if (c == 27) return Phim::thoat;
    if (c == '\r' || c == '\n') return Phim::nhan;
    return Phim::cho;
}
} // namespace

void khoiTao() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    DWORD mode{};
    const auto handle = GetStdHandle(STD_OUTPUT_HANDLE);
    const bool console = GetConsoleMode(handle, &mode);
    ansi = console && SetConsoleMode(handle, mode | ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    banPhim = console && GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &mode);
#else
    ansi = isatty(STDOUT_FILENO);
    banPhim = ansi && isatty(STDIN_FILENO);
#endif
}
bool coMau() { return ansi; }

void xoaManHinh() {
    if (ansi) { std::cout << "\033[2J\033[3J\033[H" << std::flush; return; }
#ifdef _WIN32
    const HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info{};
    if (!GetConsoleScreenBufferInfo(handle, &info)) return;
    std::cout.flush();
    const COORD start{0, 0};
    const DWORD cells = static_cast<DWORD>(info.dwSize.X) * info.dwSize.Y;
    DWORD written;
    FillConsoleOutputCharacterW(handle, L' ', cells, start, &written);
    FillConsoleOutputAttribute(handle, info.wAttributes, cells, start, &written);
    SetConsoleCursorPosition(handle, start);
#endif
}

void tieuDe(const std::string& ten) {
    const auto size = kichThuoc();
    const int frames = ansi && banPhim && size.rong >= 40 && size.cao >= 18 ? 5 : 1;
    for (int f = 0; f < frames; ++f) {
        std::ostringstream out;
        const int rows = veTieuDe(out, ten, size, f * 5, false);
        std::cout << out.str() << std::flush;
        if (f + 1 < frames) {
            std::this_thread::sleep_for(std::chrono::milliseconds(35));
            std::cout << "\033[" << rows << "F";
        }
    }
}

namespace {
int rongOChua() { return std::clamp(kichThuoc().rong - 3, 4, 58); }
}

void moOChua(const std::string& nhan) {
    const int width = rongOChua();
    const std::string border = "+" + std::string(width, '-') + "+";
    std::cout << (ansi ? "\033[1;36m" : "") << nhan << '\n' << border << '\n';
    if (ansi && banPhim) {
        std::cout << '|' << std::string(width, ' ') << "|\n" << border
                  << "\033[1F\033[3G";
    } else std::cout << "| ";
    if (ansi) std::cout << "\033[0m";
    std::cout.flush();
}

void dongOChua() {
    std::cout << (ansi ? "\033[1;36m" : "") << '+' << std::string(rongOChua(), '-')
              << '+' << (ansi ? "\033[0m" : "") << '\n';
}

int chon(const std::string& ten, const std::vector<std::string>& muc,
         const std::string& quayLai, const std::string& ghiChu, bool trangChu) {
    const int count = static_cast<int>(muc.size());
    if (!trangChu || !banPhim) {
        xoaManHinh();
        if (trangChu) {
            const auto size = kichThuoc();
            const int rows = veTieuDe(std::cout, ten, size, 0, true);
            veMayBay(std::cout, size.rong - 1, size.cao - rows - count - 5, 0);
        } else tieuDe(ten);
        if (!ghiChu.empty()) std::cout << (ansi ? "\033[1;36m" : "") << ghiChu
                                       << (ansi ? "\033[0m" : "") << "\n\n";
        for (int i = 0; i < count; ++i) {
            std::cout << (ansi ? "\033[1;36m" : "") << "  [" << i + 1 << "]  "
                      << (ansi ? "\033[0m" : "") << muc[i] << '\n';
        }
        std::cout << "\n  [0]  " << quayLai << '\n';
        for (;;) {
            std::cout << "\nNhap so (0-" << count << "): ";
            std::string input;
            if (!std::getline(std::cin, input)) return -1;
            std::istringstream parse(input);
            int value;
            if (parse >> value && (parse >> std::ws).eof() && value >= 0 && value <= count) {
                xoaManHinh();
                return value - 1;
            }
            std::cout << (ansi ? "\033[1;33m" : "") << "Lua chon khong hop le."
                      << (ansi ? "\033[0m" : "") << '\n';
        }
    }
#ifndef _WIN32
    CheDoPhim phim;
#endif
    int selected = 0;
    int frame = 0;
    bool redraw = true;
    auto tick = std::chrono::steady_clock::now();
    xoaManHinh();
    if (ansi) std::cout << "\033[?25l";
    for (;;) {
        auto now = std::chrono::steady_clock::now();
        if (redraw || (ansi && now >= tick)) {
            const auto size = kichThuoc();
            std::ostringstream out;
            if (ansi) out << "\033[H";
            else xoaManHinh();
            const int rows = veTieuDe(out, ten, size, frame, trangChu);
            const int width = std::max(1, size.rong - 1);
            const int planeRows = trangChu
                ? veMayBay(out, width, size.cao - rows - count - 5, frame) : 0;
            const int indent = std::max(0, (width - 60) / 2);
            const std::string pad(indent, ' ');
            const int visible = std::max(1, size.cao - rows - 4 - planeRows);
            const int first = std::max(0, selected - visible + 1);
            for (int i = first; i <= count && i < first + visible; ++i) {
                if (ansi && i == selected) out << "\033[1;38;2;130;221;255;48;2;43;38;92m";
                out << pad << (i == selected ? " >> " : "    ") << (i == count ? quayLai : muc[i]);
                out << (ansi ? "\033[0m\033[K\n" : "\n");
            }
            const int shown = std::min(visible, count + 1 - first);
            const int gap = std::max(0, size.cao - rows - planeRows - shown - 3);
            if (ansi) out << "\033[J";
            out << std::string(gap, '\n') << (ansi ? "\033[1;33m" : "")
                << pad << "Len / Xuong: Di chuyen\n"
                << pad << "Enter: Chon muc\n"
                << pad << "Esc: Thoat" << (ansi ? "\033[0m" : "");
            std::cout << out.str() << std::flush;
            if (now >= tick) { frame = (frame + 1) % 100000; tick = now + std::chrono::milliseconds(100); }
            redraw = false;
        }
        const Phim key = docPhim();
        if (key == Phim::thoat || key == Phim::nhan) {
            if (ansi) std::cout << "\033[?25h";
            xoaManHinh();
            return key == Phim::thoat || selected == count ? -1 : selected;
        }
        if (key == Phim::len || key == Phim::xuong) {
            selected = (selected + (key == Phim::len ? count : 1)) % (count + 1);
            redraw = true;
        }
    }
}
} // namespace ConsoleUI
