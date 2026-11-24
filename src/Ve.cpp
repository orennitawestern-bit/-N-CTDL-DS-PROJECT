#include "Ve.h"

Ve::Ve() : soGhe_(0) {}
Ve::Ve(const std::string& maChuyenBay, const std::string& cmnd,
       const std::string& hoTen, int soGhe)
    : maVe_(maChuyenBay + "_" + std::to_string(soGhe)),
      maChuyenBay_(maChuyenBay), cmnd_(cmnd), hoTen_(hoTen), soGhe_(soGhe) {}

const std::string& Ve::layMaVe() const { return maVe_; }
const std::string& Ve::layMaChuyenBay() const { return maChuyenBay_; }
const std::string& Ve::layCmnd() const { return cmnd_; }
const std::string& Ve::layHoTen() const { return hoTen_; }
int Ve::laySoGhe() const { return soGhe_; }
