# PRO_CTDL_5 - Quan ly chuyen bay noi dia (C++17)

Du an hoc phan **Cau truc du lieu & Giai thuat - Project 5**.

## 1. Pham vi va nghiep vu

**Menu chinh:**
1. Hien thi danh sach chuyen bay (ma, ngay khoi hanh, san bay den, trang thai va danh sach ve).
2. Dat ve: khach nhap chuyen bay, so ghe, CMND/CCCD, ho ten; yeu cau duoc xep vao **hang doi cho duyet**.
3. Quan ly: dang nhap tai khoan trong `data/Admin.txt`, sai 3 lan thi thoat chuong trinh; co the dung ESC de quay lai.

**Sau khi dang nhap:**
- Duyet yeu cau dat ve theo thu tu **FIFO**. Phat hanh ve, xoa ghe khoi danh sach ghe trong, luu ve vao file rieng.
- Huy yeu cau dau hang (khi khach huy hoac yeu cau khong con hop le).
- Tra ve neu chuyen bay **chua hoan tat**, tra ghe ve danh sach ghe trong, xoa file ve.
- Thong ke danh sach khach/ve theo ma chuyen bay, ghe trong, so chuyen da hoan tat cua may bay.
- Them may bay, them chuyen bay, danh dau huy/hoan tat chuyen bay, xem danh sach may bay va hang doi.

**Trang thai chuyen bay:** `0` huy, `1` con ve, `2` het ve, `3` hoan tat. Trang thai `1/2` duoc tu dong tinh theo so ghe trong. Khong duoc dat ve tren chuyen `0/2/3`. Khong duoc tra ve tren chuyen `3`. Khong duoc huy/hoan tat chuyen khi con yeu cau cho xu ly.

**Luu y:** `datVe()` chi ghi nhan yeu cau vao hang doi; `xuLyDatVe()` (quan ly) moi phat hanh ve. Cach tach nay lam dung quy trinh "khach hang cho mua ve" va "xu ly dat ve" trong de.

## 2. Cau truc du an

```text
PRO_CTDL_5/
|-- include/
|   |-- LinkedList.h       # Class template DSLK don
|   |-- Queue.h            # Class template hang doi FIFO tren DSLK don
|   |-- MayBay.h
|   |-- ChuyenBay.h
|   |-- KhachHang.h
|   |-- Ve.h
|   |-- AirlineManager.h
|   `-- Utils.h
|-- src/
|   |-- MayBay.cpp
|   |-- ChuyenBay.cpp
|   |-- KhachHang.cpp
|   |-- Ve.cpp
|   |-- AirlineManager.cpp
|   |-- Utils.cpp
|   `-- main.cpp             # Menu, nhap/xuat va password an bang *
|-- data/
|   |-- Admin.txt
|   |-- MayBay.txt
|   |-- ChuyenBay.txt
|   |-- KhachHang.txt
|   `-- Ve/                 # Moi ve la mot file maVe.txt
|-- tests/test_airline.cpp
|-- CMakeLists.txt
`-- build_windows.bat
```

## 3. Cau truc du lieu, thuat toan - chi dung khi can

| Thanh phan | Su dung | Ly do |
|---|---|---|
| `LinkedList<Datatype>` | Quan ly danh sach may bay, chuyen bay, ve, ghe trong | Them/xoa linh hoat va su dung kien thuc DSLK don |
| `Queue<Datatype>` | Hang doi khach hang cho duyet | Quy tac vao truoc xu ly truoc (FIFO) |
| Linear Search qua `timNeu()` | Tim ma may bay, chuyen bay, ve; kiem tra trung ghe, trung CMND | Du de ap dung voi du lieu hien tai, khong can cay AVL |
| Merge Sort qua `sapXep()` | Dua danh sach ghe trong ve thu tu tang dan khi tra ve | Hien thi va kiem tra ghe co thu tu de hieu |

**Khong dung AVL. Khong chen Stack khi de bai khong co nghiep vu LIFO.**

Ten ham nghiep vu va phuong thuc duoc dat theo **tieng Viet khong dau, camelCase**: `docDuLieu()`, `themMayBay()`, `themChuyenBay()`, `timChuyenBay()`, `datVe()`, `xuLyDatVe()`, `traVe()`, `thongKeSoChuyenBay()`, `themCuoi()`, `timNeu()`, `vaoHang()`, `raHang()`.

Ma nguon tach `.h`/`.cpp`, dung header guard, khong su dung `<bits/stdc++.h>` hoac `using namespace std;`. Giao dien chi nam trong `main.cpp` va co the thay doi doc lap.

## 4. Bien dich va chay

**Windows (da cai MSYS2 / MinGW-w64 va them `g++` vao PATH):**

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude src/*.cpp -o PRO_CTDL_5.exe
./PRO_CTDL_5.exe
```

Hoac chay file `build_windows.bat` tu thu muc goc du an.

**CMake (Windows/Linux/macOS, VS Code):**

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Chay `build/PRO_CTDL_5` tren Linux/macOS hoac `build/PRO_CTDL_5.exe` tren Windows. CMake sao chep `data/` vao `build/data` nen co the chay tu thu muc `build`. Chuong trinh nhan tham so thu muc du lieu, vi du: `./build/PRO_CTDL_5 ./data`.

**Dang nhap mau:** user `admin`, password `123456`.

## 5. File du lieu

Moi thong tin duoc ghi theo dong, cac truong ngan cach boi `|`:

```text
MayBay.txt:      soHieu|soCho
ChuyenBay.txt:   maCB|soHieuMayBay|YYYY-MM-DD|sanBayDen|trangThai|cacMaVeCachBangDauPhay|cacGheTrongCachBangDauPhay
KhachHang.txt:   STT|CMND|hoTen|maChuyenBay|soGheYeuCau
Ve/CB001_2.txt:  CB001_2|CB001|123456789|Nguyen Van A|2
Admin.txt:       admin|123456
```

Khi tai chuong trinh, danh sach ghe trong duoc tao theo suc chua may bay, sau do loai tru cac ghe co ve hop le (dua vao ma ve trong `ChuyenBay.txt` va file ve). Cac dong du lieu khong hop le duoc bo qua va thong bao so luong; file bat buoc bi thieu thi chuong trinh bao loi.

**Khong nhap ky tu `|` trong ho ten hoac san bay den.** CMND/CCCD gom 9 hoac 12 chu so, ngay theo `YYYY-MM-DD` va phai la ngay hop le. Ma may bay/chuyen bay gom chu, so, dau `_` hoac `-`.

## 6. Kiem thu

Da co bo kiem thu tu dong trong `tests/test_airline.cpp`, bao gom:
- DSLK don: them, tim/xoa, sap xep.
- Dang nhap, phat hien ma may bay/chuyen bay trung, ngay khong hop le.
- Dat ve: kiem tra trong, ghe vuot qua suc chua, trung ghe, trung CMND.
- Duyet yeu cau FIFO, tao file ve, cap nhat trang thai `1 -> 2`.
- Tra ve, xoa file ve, mo lai ghe, cap nhat trang thai `2 -> 1`.
- Doc lai du lieu sau khi thoat/mo lai, bao gom ve da ban va khach hang dang cho.
- Khoa tra ve khi hoan tat, khong cho danh dau hoan tat khi con khach dang cho.
- Huy yeu cau trong hang doi, huy chuyen bay; bao loi khi thieu file bat buoc.

**Gioi han cua project:** day la ung dung hoc tap chay 1 tien trinh, du lieu TXT va tai khoan mau la minh hoa; chua ho tro nhieu nhan vien ghi du lieu dong thoi, khong co co so du lieu giao dich hay ma hoa mat khau o muc san pham thuc te.
