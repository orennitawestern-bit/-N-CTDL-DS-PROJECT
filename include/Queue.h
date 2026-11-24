#ifndef QUEUE_H
#define QUEUE_H

#include "LinkedList.h"

#include <cstddef>

/** Hang doi FIFO duoc cai bang danh sach lien ket don. */
template <typename Datatype>
class Queue {
private:
    LinkedList<Datatype> data_;

public:
    bool rong() const { return data_.rong(); }
    std::size_t kichThuoc() const { return data_.kichThuoc(); }
    void vaoHang(const Datatype& value) { data_.themCuoi(value); }
    bool raHang() { return data_.xoaDau(); }
    Datatype* dauHang() { return data_.dauDanhSach(); }
    const Datatype* dauHang() const { return data_.dauDanhSach(); }

    template <typename Func>
    void duyet(Func func) const { data_.duyet(func); }
};

#endif
