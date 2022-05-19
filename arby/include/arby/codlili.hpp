/*
 * codlili
 *
 * /kɒdliːliː/
 *
 * - COnstexpr
 * - Doubly
 * - LInked
 * - LIst
 */

#ifndef COM_SAXBOPHONE_CODLILI_LIST_HPP
#define COM_SAXBOPHONE_CODLILI_LIST_HPP

#include <cstddef>           // size_t
#include <algorithm>         // swap
#include <initializer_list>  // initializer_list
#include <iterator>          // iterator traits

#include <iostream> // DEBUG

namespace com::saxbophone::codlili {
    template <typename T>  // the type of elements to store
    class List {
    public:
        // simple record type for the doubly-linked-list nodes
        struct ListNode {
            constexpr ListNode() {}
            constexpr ListNode(T value) : value(value) {}
            constexpr ListNode(T value, ListNode* next) : next(next), value(value) {}
            constexpr ListNode(ListNode* prev, T value) : prev(prev), value(value) {}
            constexpr ~ListNode() {
                if (next != nullptr) {
                    // delete forwards only
                    delete next;
                    next = nullptr;
                }
            }
            // ListNode(ListNode* prev, T value, ListNode* next) {}
            ListNode* next = nullptr;
            ListNode* prev = nullptr;
            T value = {};
        };
        struct iterator {
            // TODO: convert to std::random_access_iterator_tag in future
            using iterator_category = std::bidirectional_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T*;
            using reference = T&;

            constexpr iterator(ListNode* node) : _cursor(node) {}
            constexpr reference operator*() const { return _cursor->value; }
            constexpr pointer operator->() { return &_cursor->value; }
            constexpr iterator& operator++() {
                _cursor = _cursor->next;
                return *this;
            }
            constexpr iterator operator++(int) {
                iterator tmp = *this;
                operator++();
                return tmp;
            }
            constexpr iterator& operator--() {
                _cursor = _cursor->prev;
                return *this;
            }
            constexpr iterator operator--(int) {
                iterator tmp = *this;
                operator--();
                return tmp;
            }
            // std::random_access_iterator_tag
            constexpr iterator& operator+=(difference_type offset) {
                if (offset < 0) { // walk backwards
                    for (difference_type i = offset; i < 0; i++) { operator--(); }
                } else { // walk forwards
                    for (difference_type i = 0; i < offset; i++) { operator++(); }
                }
                return *this;
            }
            // std::random_access_iterator_tag
            constexpr iterator operator+(difference_type offset) const {
                // reuse compound assignment
                iterator jump = *this;
                jump += offset;
                return jump;
            }
            // std::random_access_iterator_tag
            constexpr friend iterator operator+(difference_type offset, const iterator& rhs) {
                // reuse operator+
                return rhs + offset;
            }
            // std::random_access_iterator_tag
            // TODO: this is almost identical to operator+= --couldn't we just multiply offset by -1 and reuse that operator?
            constexpr iterator& operator-=(difference_type offset) {
                if (offset > 0) { // walk backwards
                    for (difference_type i = offset; i --> 0; ) { operator--(); }
                } else { // walk forwards
                    for (difference_type i = 0; i < offset; i++) { operator++(); }
                }
                return *this;
            }
            // std::random_access_iterator_tag
            constexpr iterator operator-(difference_type offset) const {
                // reuse compound assignment
                iterator jump = *this;
                jump -= offset;
                return jump;
            }
            // std::random_access_iterator_tag
            // constexpr difference_type operator-(const iterator& rhs) const {
            //     return 0; // TODO: calculate signed delta between iterators and return that
            // }
            // comparison
            constexpr friend bool operator==(const iterator& a, const iterator& b) {
                return a._cursor == b._cursor;
            };
            constexpr friend bool operator!=(const iterator& a, const iterator& b) {
                return a._cursor != b._cursor;
            };
            // std::random_access_iterator_tag
            // TODO: implement once signed delta between iterators is implemented
            // constexpr friend auto operator<=>(const iterator& lhs, const iterator& rhs) {}
        private:
            ListNode* _cursor;
        };
        using reverse_iterator = std::reverse_iterator<iterator>;
        using reference = T&;
        using const_reference = const T&;
        // initialises size to zero, an empty list
        constexpr List() noexcept {
            // debug();
        }
        // initialises list with the specified number of default-constructed elements
        constexpr List(std::size_t size) : List(size, T{}) {
            // debug();
        } // reuse (size,value) ctor
        // initialises list with the given elements
        constexpr List(std::initializer_list<T> elements) {
            for (auto element : elements) {
                push_back(element);
            }
            // debug();
        }
        // initialises list with the specified number of this element value-copied
        constexpr List(std::size_t size, const_reference value) {
            for (std::size_t i = 0; i < size; i++) {
                push_back(value);
            }
            // debug();
        }
        /* rule of three: */
        // copy constructor
        constexpr List(const List& other) {
            for (auto element : other) {
                push_back(element);
            }
            // debug();
        }
        // destructor, needed because there is manual memory management
        constexpr ~List() {
            if (_front != nullptr) {
                // delete forwards only
                delete _front;
            }
            // debug();
        }
        // copy assignment operator
        constexpr List& operator=(const List& other) noexcept {
            clear();
            for (auto element : other) {
                push_back(element);
            }
            // debug();
            return *this;
        }
        /* element access */
        // get a reference to the element at specified location without bounds checking
        constexpr reference operator[](std::size_t index) {
            auto cursor = _front;
            for (std::size_t i = 0; i < index; i++) {
                cursor = cursor->next;
            }
            // debug();
            return cursor->value;
        }
        // get a read-only reference to the element at specified location without bounds checking
        constexpr const_reference operator[](std::size_t index) const {
            auto cursor = _front;
            for (std::size_t i = 0; i < index; i++) {
                cursor = cursor->next;
            }
            // debug();
            return cursor->value;
        }
        // get reference to first element
        constexpr reference front() { return this->_front->value; }
        // get read-only reference to first element
        constexpr const_reference front() const { return this->_front->value; }
        // get reference to last element
        constexpr reference back() { return this->_back->value; }
        // get read-only reference to last element
        constexpr const_reference back() const { return this->_back->value; }
        /* iterators */
        constexpr iterator begin() { return iterator(_front); }
        // XXX: these end() iterators don't work for reverse iteration because they don't store pointers to the previous
        // element (i.e. the last true element)
        // TODO: change iterator implementation to store one dummy iterator at the end that only has a prev pointer and
        // add a method bool iterator::is_sentinel() for detecting it, use it in iteration code
        constexpr iterator end() { return iterator(nullptr); } // 1 past the end, out of bounds
        constexpr iterator begin() const { return iterator(_front); }
        constexpr iterator end() const { return iterator(nullptr); } // 1 past the end, out of bounds
        constexpr reverse_iterator rbegin() { return reverse_iterator(end()); }
        constexpr reverse_iterator rend() { return reverse_iterator(begin()); }
        constexpr reverse_iterator rbegin() const { return reverse_iterator(end()); }
        constexpr reverse_iterator rend() const { return reverse_iterator(begin()); }
        /* capacity */
        // is list empty?
        constexpr bool empty() const noexcept { return _front == nullptr; }
        // get size of list in number of elements
        constexpr std::size_t size() const noexcept {
            // count the number of elements
            std::size_t count = 0;
            for (auto it : *this) {
                count++;
            }
            // debug();
            return count;
        }
        /* modifiers */
        // erases all elements from the list, .size() = 0 after this call
        constexpr void clear() noexcept {
            if (_front != nullptr) {
                delete _front;
                _front = nullptr;
                _back = nullptr;
            }
            // debug();
        }
        // prepends the given element value to the front of the list
        constexpr void push_front(const_reference value) {
            if (empty()) {
                _front = new ListNode(value);
                _back = _front;
            } else {
                _front = new ListNode(value, _front);
                // create the back-link from old front to new front
                _front->next->prev = _front;
            }
            // debug();
        }
        // appends the given element value to the end of the list
        constexpr void push_back(const_reference value) {
            if (empty()) {
                _back = new ListNode(value);
                _front = _back;
            } else {
                _back = new ListNode(_back, value);
                // create the link from old back to new back
                _back->prev->next = _back;
            }
            // debug();
        }
        // prepends size copies of the given element value to the front of the list
        constexpr void push_front(std::size_t size, const_reference value) {
            for (std::size_t i = 0; i < size; i++) {
                push_front(value);
            }
            // debug();
        }
        // appends count copies of the given element value to the end of the list
        constexpr void push_back(std::size_t size, const_reference value) {
            for (std::size_t i = 0; i < size; i++) {
                push_back(value);
            }
            // debug();
        }
        // removes the first element from the list
        constexpr void pop_front() {
            auto old_front = _front; // keep reference to old front
            _front = old_front->next; // set the 2nd element to be the new front
            // remove the link from old front <-> new front before deleting the former
            old_front->next = nullptr;
            if (_front == nullptr) { // if List is now empty, ensure the back pointer is null as well
                _back = nullptr;
            } else { // otherwise, just clear prev pointer of new front node
                _front->prev = nullptr;
            }
            delete old_front;
            // debug();
        }
        // removes the last element from the list
        constexpr void pop_back() {
            auto old_back = _back; // keep reference to old back
            _back = old_back->prev; // set the penultimate element to be the new back
            // remove the link from old back <-> new back before deleting the former
            old_back->prev = nullptr;
            if (_back == nullptr) { // if List is now empty, ensure the front pointer is null as well
                _front = nullptr;
            } else { // otherwise, just clear next pointer of new back node
                _back->next = nullptr;
            }
            delete old_back;
            // debug();
        }
        // resizes the list to hold count elements, removing excess elements if count less than current size, or adding
        // new default-constructed elements at the end if it is greater
        constexpr void resize(std::size_t count) { return resize(count, T{}); }
        // resizes the list to hold count elements, removing excess elements if count less than current size, or adding
        // new copies of value at the end if it is greater
        constexpr void resize(std::size_t count, const_reference value) {
            std::size_t current_size = size();
            if (count < current_size) { // remove elements
                for (std::size_t i = current_size; i > count; i--) {
                    pop_back();
                }
            } else { // add elements
                for (std::size_t i = current_size; i < count; i++) {
                    push_back(value);
                }
            }
            // debug();
        }
        // exchanges this list's contents with that of the other
        constexpr void swap(List& other) noexcept {
            // swapping the front and back pointers should be enough to exchange contents
            std::swap(_front, other._front);
            std::swap(_back, other._back);
        }
        /* comparison */
        constexpr bool operator==(const List& other) const {
            // debug();
            if (size() != other.size()) { return false; }
            for (std::size_t i = 0; i < size(); i++) {
                if ((*this)[i] != other[i]) {
                    return false;
                }
            }
            return true;
        }
        // constexpr friend auto operator<=>(const List<T>& lhs, const List<T>& rhs) {}
    private:
        void debug() const {
            std::cout << "List @" << this << ": ";
            auto cursor = _front;
            do {
                std::cout << "[" << cursor;
                if (cursor != nullptr) {
                    std::cout << ", " << cursor->prev << ", " << cursor->next;
                    cursor = cursor->next;
                }
                std::cout << "] -> ";
            } while (cursor != nullptr);
            std::cout << "[" << cursor;
            if (cursor != nullptr) {
                std::cout << ", " << cursor->prev << ", " << cursor->next;
            }
            std::cout << "]";
            // std::cin.get();
            std::cout << std::endl;
        }
        // front and back pointers
        ListNode* _front = nullptr;
        ListNode* _back = nullptr;
    };
}

#endif
