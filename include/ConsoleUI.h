#pragma once

#include <string>
#include <vector>

namespace ConsoleUI {
void khoiTao();
bool coMau();
void xoaManHinh();
void tieuDe(const std::string& ten);
void moOChua(const std::string& nhan);
void dongOChua();

// Vi tri bat dau tu 0. Tra ve -1 khi chon muc cuoi (quay lai/thoat).
int chon(const std::string& ten, const std::vector<std::string>& muc,
         const std::string& quayLai = "Quay lai", const std::string& ghiChu = "",
         bool trangChu = false);
}
