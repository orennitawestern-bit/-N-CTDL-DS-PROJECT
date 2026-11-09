#ifndef MAY_BAY_H
#define MAY_BAY_H

#include <string>

class MayBay {
private:
    std::string soHieu_;
    int soCho_;

public:
    MayBay();
    MayBay(const std::string& soHieu, int soCho);

    const std::string& laySoHieu() const;
    int laySoCho() const;
};

#endif
