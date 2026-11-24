#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <cstddef>
#include <utility>

/**
 * Danh sach lien ket don tong quat (class template Datatype).
 * Tim kiem tuyen tinh, chen dau/cuoi, xoa va Merge Sort.
 */
template <typename Datatype>
class LinkedList {
private:
    struct Node {
        Datatype data;
        Node* next;

        explicit Node(const Datatype& value) : data(value), next(nullptr) {}
    };

    Node* head_;
    Node* tail_;
    std::size_t size_;

    template <typename Compare>
    static Node* merge(Node* left, Node* right, Compare compare);

    template <typename Compare>
    static Node* mergeSort(Node* first, Compare compare);

public:
    LinkedList();

    LinkedList(const LinkedList& other);

    LinkedList(LinkedList&& other);

    LinkedList& operator=(LinkedList other);

    ~LinkedList();

    void hoanDoi(LinkedList& other) noexcept;

    bool rong() const;
    std::size_t kichThuoc() const;

    void themDau(const Datatype& value);

    void themCuoi(const Datatype& value);

    Datatype* dauDanhSach();
    const Datatype* dauDanhSach() const;

    bool xoaDau();

    template <typename Predicate>
    Datatype* timNeu(Predicate predicate);

    template <typename Predicate>
    const Datatype* timNeu(Predicate predicate) const;

    template <typename Predicate>
    bool xoaDauTienNeu(Predicate predicate);

    template <typename Func>
    void duyet(Func func);

    template <typename Func>
    void duyet(Func func) const;

    template <typename Compare>
    void sapXep(Compare compare);

    void xoaTatCa();
};

#endif
