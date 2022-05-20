/*
 * codlili v0.1.0
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
            constexpr ListNode(ListNode* prev, T value, ListNode* next) : next(next), prev(prev), value(value) {}
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
            // debug();
            for (auto element : elements) {
                push_back(element);
            }
            // debug();
        }
        // initialises list with the specified number of this element value-copied
        constexpr List(std::size_t size, const_reference value) {
            // debug();
            for (std::size_t i = 0; i < size; i++) {
                push_back(value);
            }
            // debug();
        }
        /* rule of three: */
        // copy constructor
        constexpr List(const List& other) {
            // debug();
            for (auto element : other) {
                push_back(element);
            }
            // debug();
        }
        // destructor, needed because there is manual memory management
        constexpr ~List() {
            // debug();
            // delete the entire chain of ListNode pointers
            auto cursor = _back;
            while (cursor != nullptr) {
                auto next = cursor->prev;
                delete cursor;
                cursor = next;
            }
            _front = nullptr;
            _back = nullptr;
            // debug();
        }
        // copy assignment operator
        constexpr List& operator=(const List& other) noexcept {
            // debug();
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
        constexpr reference back() { return this->_back->prev->value; }
        // get read-only reference to last element
        constexpr const_reference back() const { return this->_back->prev->value; }
        /* iterators */
        constexpr iterator begin() { return iterator(_front); }
        constexpr iterator end() { return iterator(_back); } // 1 past the end, out of bounds
        constexpr iterator begin() const { return iterator(_front); }
        constexpr iterator end() const { return iterator(_back); } // 1 past the end, out of bounds
        /* capacity */
        // is list empty?
        constexpr bool empty() const noexcept { return _front == _back; }
        // get size of list in number of elements
        constexpr std::size_t size() const noexcept {
            debug();
            // count the number of elements
            std::size_t count = 0;
            for (auto it : *this) {
                count++;
            }
            // debug();
            // std::cout << "size()" << std::endl;
            return count;
        }
        /* modifiers */
        // erases all elements from the list, .size() = 0 after this call
        constexpr void clear() noexcept {
            // delete all except back, and reset front to back after
            auto cursor = _back->prev;
            while (cursor != nullptr) {
                auto next = cursor->prev;
                delete cursor;
                cursor = next;
            }
            // reset back's links and set front to back
            _back->prev = nullptr;
            _front = _back;
            // debug();
        }
        // prepends the given element value to the front of the list
        constexpr void push_front(const_reference value) {
            debug();
            auto old_front = _front;
            _front = new ListNode(value, old_front);
            // create the back-link from old front to new front
            old_front->prev = _front;
            debug();
        }
        // appends the given element value to the end of the list
        constexpr void push_back(const_reference value) {
            // debug();
            // when empty it's the same as push_front()
            if (empty()) { return push_front(value); }
            // insert between back and back-1
            auto behind = _back->prev;
            ListNode* added = new ListNode(behind, value, _back);
            // create back-references
            behind->next = added;
            _back->prev = added;
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
            // debug();
            auto old_front = _front; // keep reference to old front
            _front = old_front->next; // set the 2nd element to be the new front
            if (_front == _back) { // if front is now back marker, list is now empty, so set back->prev
                _back->prev = nullptr;
            } else { // otherwise, just clear prev pointer of new front node
                _front->prev = nullptr;
            }
            delete old_front;
            // debug();
        }
        // removes the last element from the list
        constexpr void pop_back() {
            // debug();
            // remove back-1
            auto behind = _back->prev;
            // create the backlink from back to whatever was behind behind
            _back->prev = behind->prev;
            // if there is something behind behind, link it forward to back
            if (behind->prev != nullptr) { behind->prev->next = _back; }
            // now set behind's backlink to null so we can delete it without destroying everything else
            behind->prev = nullptr;
            delete behind;
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
            for (auto cursor = _front; cursor != nullptr; cursor = cursor->next) {
                std::cout << cursor << "<->";
            }
            std::cout << " [" << _back << "]";
            // std::cin.get();
            std::cout << std::endl;
        }
        // front and back pointers
        ListNode* _front = new ListNode();
        ListNode* _back = _front;
    };
}

#endif
