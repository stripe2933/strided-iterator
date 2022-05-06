/**
 * @file strided_iterator.hpp
 * @author LEE KYOUNGHEON (stripe2933@outlook.com)
 * @version 0.1
 * @date 2022-05-06
 * 
 * @copyright Copyright (c) 2022
 * This code is licensed under MIT license (see LICENSE.txt for details)
 */

#pragma once

#include <iterator>
#include <stdexcept>

template <typename Tp, std::ptrdiff_t...> struct strided_iterator;

/**
 * @brief An iterator with stride.
 * 
 * @tparam Tp Type of iterator referencing
 * @tparam Stride Iterator stride
 * 
 * @note If std > c++17 and Stride is either 1 or -1, it is std::contiguous_iterator, otherwise, it is std::random_access_iterator.
 * If \p Stride is negative, it goes backward.
 * If \p Stride is 0, it will reference same object forever while increment/decrement/arithemetic operations for the iterator.
 */
template <typename Tp, std::ptrdiff_t Stride>
struct strided_iterator<Tp, Stride>{
public:
#if __cplusplus > 201703L
    using iterator_category = std::conditional_t<(Stride == 1) || (Stride == -1), std::contiguous_iterator_tag, std::random_access_iterator_tag>;
    using element_type = Tp; // for std::contiguous_iterator_tag
#else
    using iterator_category = std::random_access_iterator_tag;
#endif

    using value_type = Tp;
    using pointer = Tp*;
    using reference = Tp&;
    using difference_type = std::ptrdiff_t;

    using self_type = strided_iterator<Tp, Stride>;

private:
    Tp* _ptr;

public:
    // Constructors
    strided_iterator() noexcept : _ptr { } { }
    strided_iterator(pointer ptr) noexcept : _ptr { ptr } { }
    strided_iterator(std::__wrap_iter<pointer> wrap_iter) noexcept : _ptr { wrap_iter.base() } { }
    strided_iterator(const self_type& source) noexcept : _ptr { source._ptr } { }

    self_type& operator=(const self_type& iterator) noexcept { _ptr = iterator._ptr; return *this; }
    self_type& operator=(pointer ptr) noexcept { _ptr = ptr; return *this; }

    // Tp* like operators
    reference operator*() const noexcept { return *_ptr; }
    pointer operator->() const noexcept { return _ptr; }
    reference operator[](difference_type n) const noexcept { return _ptr[n * Stride]; }

    // Increment / Decrement
    self_type& operator++() noexcept { _ptr += Stride; return *this; }
    self_type operator++(int) noexcept { self_type temp { *this }; ++(*this); return temp; }
    self_type& operator--() noexcept { _ptr -= Stride; return *this; }
    self_type operator--(int) noexcept { self_type temp { *this }; --(*this); return temp; }

    // Arithmetic
    self_type& operator+=(difference_type n) noexcept { _ptr += n * Stride; return *this; }
    self_type& operator-=(difference_type n) noexcept { _ptr -= n * Stride; return *this; }
    friend self_type operator+(const self_type& iter, difference_type n) noexcept { self_type temp { iter }; temp += n; return temp; }
    friend self_type operator+(difference_type n, self_type right) noexcept { right += n; return right; }
    friend self_type operator-(self_type left, difference_type n) noexcept { left -= n; return left; }
    
    // Difference
    friend difference_type operator-(const self_type& left, const self_type& right) noexcept { return (right._ptr - left._ptr) / Stride; }

    // Comparison operators
    friend bool operator==(const self_type& left, const self_type& right) noexcept { return left._ptr == right._ptr; }
    friend bool operator!=(const self_type& left, const self_type& right) noexcept { return left._ptr != right._ptr; }
    friend bool operator<(const self_type& left, const self_type& right) noexcept { return left._ptr < right._ptr; }
    friend bool operator<=(const self_type& left, const self_type& right) noexcept { return left._ptr <= right._ptr; }
    friend bool operator>(const self_type& left, const self_type& right) noexcept { return left._ptr > right._ptr; }
    friend bool operator>=(const self_type& left, const self_type& right) noexcept { return left._ptr >= right._ptr; }
};

/**
 * @brief An iterator with stride.
 * 
 * @tparam Tp Type of iterator referencing
 * 
 * @note If the stride is known at the compile time, it is recomended to use strided_iterator<Tp, Stride> because it has less overhead
 * and optimization for std::contiguous_iterator for \p Stride is 1 or -1.
 */
template <typename Tp>
struct strided_iterator<Tp>{
public:
    using iterator_category = std::random_access_iterator_tag;

    using value_type = Tp;
    using pointer = Tp*;
    using reference = Tp&;
    using difference_type = std::ptrdiff_t;

    using self_type = strided_iterator<Tp>;

private:
    Tp* _ptr;
    difference_type _stride;

public:
    // Constructors
    strided_iterator(difference_type stride = 1) noexcept : _ptr { }, _stride { stride } { }
    strided_iterator(Tp* ptr, difference_type stride = 1) noexcept : _ptr { ptr }, _stride { stride } { }
    strided_iterator(std::__wrap_iter<Tp*> wrap_iter, difference_type stride = 1) noexcept : _ptr { wrap_iter.base() }, _stride { stride } { }
    strided_iterator(const self_type& source) noexcept : _ptr { source._ptr }, _stride { source._stride } { }

    self_type& operator=(const self_type& iterator) noexcept { _ptr = iterator._ptr; return *this; }
    self_type& operator=(pointer ptr) noexcept { _ptr = ptr; return *this; }

    // Tp* like operators
    reference operator*() const noexcept { return *_ptr; }
    pointer operator->() const noexcept { return _ptr; }
    reference operator[](difference_type n) const noexcept { return _ptr[n * _stride]; }

    // Increment / Decrement
    self_type& operator++() noexcept { _ptr += _stride; return *this; }
    self_type operator++(int) noexcept { self_type temp { *this }; ++(*this); return temp; }
    self_type& operator--() noexcept { _ptr -= _stride; return *this; }
    self_type operator--(int) noexcept { self_type temp { *this }; --(*this); return temp; }

    // Arithmetic
    self_type& operator+=(difference_type n) noexcept { _ptr += n * _stride; return *this; }
    self_type& operator-=(difference_type n) noexcept { _ptr -= n * _stride; return *this; }
    friend self_type operator+(const self_type& iter, difference_type n) noexcept { self_type temp { iter }; temp += n; return temp; }
    friend self_type operator+(difference_type n, self_type right) noexcept { right += n; return right; }
    friend self_type operator-(self_type left, difference_type n) noexcept { left -= n; return left; }
    
    // Difference
    friend difference_type operator-(const self_type& left, const self_type& right) { 
        if (left._stride != right._stride){
            throw std::runtime_error { "strided_iterator<Tp> subtract operation with different strides." };
        }
        return (right._ptr - left._ptr) / left._stride;
    }

    // Comparison operators
    friend bool operator==(const self_type& left, const self_type& right) noexcept { return left._ptr == right._ptr; }
    friend bool operator!=(const self_type& left, const self_type& right) noexcept { return left._ptr != right._ptr; }
    friend bool operator<(const self_type& left, const self_type& right) noexcept { return left._ptr < right._ptr; }
    friend bool operator<=(const self_type& left, const self_type& right) noexcept { return left._ptr <= right._ptr; }
    friend bool operator>(const self_type& left, const self_type& right) noexcept { return left._ptr > right._ptr; }
    friend bool operator>=(const self_type& left, const self_type& right) noexcept { return left._ptr >= right._ptr; }
};