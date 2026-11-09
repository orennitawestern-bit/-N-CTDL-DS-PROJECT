#include "MayBay.h"

MayBay::MayBay() : soCho_(0) {}
MayBay::MayBay(const std::string& soHieu, int soCho)
    : soHieu_(soHieu), soCho_(soCho) {}

const std::string& MayBay::laySoHieu() const { return soHieu_; }
int MayBay::laySoCho() const { return soCho_; }
