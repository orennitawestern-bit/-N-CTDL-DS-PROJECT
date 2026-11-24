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
    static Node* merge(Node* left, Node* right, Compare compare) {
        Node* result = nullptr;
        Node** lastLink = &result;
        while (left != nullptr && right != nullptr) {
            if (compare(right->data, left->data)) {
                *lastLink = right;
                right = right->next;
            } else {
                *lastLink = left;
                left = left->next;
            }
            lastLink = &((*lastLink)->next);
        }
        *lastLink = (left != nullptr) ? left : right;
        return result;
    }

    template <typename Compare>
    static Node* mergeSort(Node* first, Compare compare) {
        if (first == nullptr || first->next == nullptr) {
            return first;
        }
        Node* slow = first;
        Node* fast = first->next;
        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
        }
        Node* right = slow->next;
        slow->next = nullptr;
        return merge(mergeSort(first, compare), mergeSort(right, compare), compare);
    }

public:
    LinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}

    LinkedList(const LinkedList& other) : LinkedList() {
        other.duyet([this](const Datatype& value) { themCuoi(value); });
    }

    LinkedList(LinkedList&& other) noexcept
        : head_(other.head_), tail_(other.tail_), size_(other.size_) {
        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = 0;
    }

    LinkedList& operator=(LinkedList other) {
        hoanDoi(other);
        return *this;
    }

    ~LinkedList() { xoaTatCa(); }

    void hoanDoi(LinkedList& other) noexcept {
        std::swap(head_, other.head_);
        std::swap(tail_, other.tail_);
        std::swap(size_, other.size_);
    }

    bool rong() const { return size_ == 0; }
    std::size_t kichThuoc() const { return size_; }

    void themDau(const Datatype& value) {
        Node* node = new Node(value);
        node->next = head_;
        head_ = node;
        if (tail_ == nullptr) tail_ = node;
        ++size_;
    }

    void themCuoi(const Datatype& value) {
        Node* node = new Node(value);
        if (tail_ != nullptr) tail_->next = node;
        else head_ = node;
        tail_ = node;
        ++size_;
    }

    Datatype* dauDanhSach() { return (head_ != nullptr) ? &head_->data : nullptr; }
    const Datatype* dauDanhSach() const { return (head_ != nullptr) ? &head_->data : nullptr; }

    bool xoaDau() {
        if (head_ == nullptr) return false;
        Node* old = head_;
        head_ = head_->next;
        delete old;
        --size_;
        if (head_ == nullptr) tail_ = nullptr;
        return true;
    }

    template <typename Predicate>
    Datatype* timNeu(Predicate predicate) {
        for (Node* current = head_; current != nullptr; current = current->next) {
            if (predicate(current->data)) return &current->data;
        }
        return nullptr;
    }

    template <typename Predicate>
    const Datatype* timNeu(Predicate predicate) const {
        for (const Node* current = head_; current != nullptr; current = current->next) {
            if (predicate(current->data)) return &current->data;
        }
        return nullptr;
    }

    template <typename Predicate>
    bool xoaDauTienNeu(Predicate predicate) {
        Node* previous = nullptr;
        Node* current = head_;
        while (current != nullptr) {
            if (predicate(current->data)) {
                if (previous != nullptr) previous->next = current->next;
                else head_ = current->next;
                if (tail_ == current) tail_ = previous;
                delete current;
                --size_;
                return true;
            }
            previous = current;
            current = current->next;
        }
        return false;
    }

    template <typename Func>
    void duyet(Func func) {
        for (Node* current = head_; current != nullptr; current = current->next) {
            func(current->data);
        }
    }

    template <typename Func>
    void duyet(Func func) const {
        for (const Node* current = head_; current != nullptr; current = current->next) {
            func(current->data);
        }
    }

    template <typename Compare>
    void sapXep(Compare compare) {
        head_ = mergeSort(head_, compare);
        tail_ = head_;
        while (tail_ != nullptr && tail_->next != nullptr) tail_ = tail_->next;
    }

    void xoaTatCa() {
        while (xoaDau()) {}
    }
};

#endif
