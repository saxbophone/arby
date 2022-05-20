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


namespace com::saxbophone::codlili {
    template <typename T>  // the type of elements to store
    class List {
    public:
        // simple record type for the doubly-linked-list nodes
        struct ListNode {
            constexpr ListNode() {}
            constexpr ListNode(T value, ListNode* next) : next(next), value(value) {}
            constexpr ListNode(ListNode* prev, T value, ListNode* next) : next(next), prev(prev), value(value) {}
            ListNode* next = nullptr;
            ListNode* prev = nullptr;
            T value = {};
        };
        struct iterator {
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
            // comparison
            constexpr friend bool operator==(const iterator& a, const iterator& b) {
                return a._cursor == b._cursor;
            };
            constexpr friend bool operator!=(const iterator& a, const iterator& b) {
                return a._cursor != b._cursor;
            };
        private:
            ListNode* _cursor;
        };
        using reverse_iterator = std::reverse_iterator<iterator>;
        using reference = T&;
        using const_reference = const T&;
        // initialises size to zero, an empty list
        constexpr List() noexcept {}
        // initialises list with the specified number of default-constructed elements
        constexpr List(std::size_t size) : List(size, T{}) {} // reuse (size,value) ctor
        // initialises list with the given elements
        constexpr List(std::initializer_list<T> elements) {
            for (auto element : elements) {
                push_back(element);
            }
        }
        // initialises list with the specified number of this element value-copied
        constexpr List(std::size_t size, const_reference value) {
            for (std::size_t i = 0; i < size; i++) {
                push_back(value);
            }
        }
        /* rule of three: */
        // copy constructor
        constexpr List(const List& other) {
            for (auto element : other) {
                push_back(element);
            }
        }
        // destructor, needed because there is manual memory management
        constexpr ~List() {
            // delete the entire chain of ListNode pointers
            auto cursor = _back;
            while (cursor != nullptr) {
                auto next = cursor->prev;
                delete cursor;
                cursor = next;
            }
            _front = nullptr;
            _back = nullptr;
        }
        // copy assignment operator
        constexpr List& operator=(const List& other) noexcept {
            clear();
            for (auto element : other) {
                push_back(element);
            }
            return *this;
        }
        /* element access */
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
        constexpr reverse_iterator rbegin() { return reverse_iterator(end()); }
        constexpr reverse_iterator rend() { return reverse_iterator(begin()); }
        constexpr reverse_iterator rbegin() const { return reverse_iterator(end()); }
        constexpr reverse_iterator rend() const { return reverse_iterator(begin()); }
        /* capacity */
        // is list empty?
        constexpr bool empty() const noexcept { return _front == _back; }
        // get size of list in number of elements
        constexpr std::size_t size() const noexcept {
            // count the number of elements
            std::size_t count = 0;
            for (auto it[[maybe_unused]] : *this) {
                count++;
            }
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
        }
        // prepends the given element value to the front of the list
        constexpr void push_front(const_reference value) {
            auto old_front = _front;
            _front = new ListNode(value, old_front);
            // create the back-link from old front to new front
            old_front->prev = _front;
        }
        // appends the given element value to the end of the list
        constexpr void push_back(const_reference value) {
            // when empty it's the same as push_front()
            if (empty()) { return push_front(value); }
            // insert between back and back-1
            auto behind = _back->prev;
            ListNode* added = new ListNode(behind, value, _back);
            // create back-references
            behind->next = added;
            _back->prev = added;
        }
        // prepends size copies of the given element value to the front of the list
        constexpr void push_front(std::size_t size, const_reference value) {
            for (std::size_t i = 0; i < size; i++) {
                push_front(value);
            }
        }
        // appends count copies of the given element value to the end of the list
        constexpr void push_back(std::size_t size, const_reference value) {
            for (std::size_t i = 0; i < size; i++) {
                push_back(value);
            }
        }
        // removes the first element from the list
        constexpr void pop_front() {
            auto old_front = _front; // keep reference to old front
            _front = old_front->next; // set the 2nd element to be the new front
            if (_front == _back) { // if front is now back marker, list is now empty, so set back->prev
                _back->prev = nullptr;
            } else { // otherwise, just clear prev pointer of new front node
                _front->prev = nullptr;
            }
            delete old_front;
        }
        // removes the last element from the list
        constexpr void pop_back() {
            // remove back-1
            auto behind = _back->prev;
            // create the backlink from back to whatever was behind behind
            _back->prev = behind->prev;
            // if there is something behind behind, link it forward to back
            if (behind->prev != nullptr) { behind->prev->next = _back; }
            delete behind;
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
        }
        // exchanges this list's contents with that of the other
        constexpr void swap(List& other) noexcept {
            // swapping the front and back pointers should be enough to exchange contents
            std::swap(_front, other._front);
            std::swap(_back, other._back);
        }
        /* comparison */
        constexpr bool operator==(const List& other) const {
            // if (size() != other.size()) { return false; }
            // for (std::size_t i = 0; i < size(); i++) {
            //     if ((*this)[i] != other[i]) {
            //         return false;
            //     }
            // }
            return std::equal(begin(), end(), other.begin(), other.end());
        }
        // constexpr friend auto operator<=>(const List<T>& lhs, const List<T>& rhs) {}
    private:
        // front and back pointers
        ListNode* _front = new ListNode();
        ListNode* _back = _front;
    };
}

#endif
