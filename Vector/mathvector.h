#pragma once
#include "vector.h"
#include <cmath>
#include <stdexcept>
#include <iostream>

template <typename T>
class TMathVector : public Vector<T> {
public:
    TMathVector();                                
    explicit TMathVector(size_t n);                 
    TMathVector(size_t n, const T& value);          
    TMathVector(std::initializer_list<T> list);    
    TMathVector(const TMathVector&) = default;
    TMathVector(TMathVector&&) noexcept = default;
    TMathVector& operator=(const TMathVector&) = default;
    TMathVector& operator=(TMathVector&&) noexcept = default;
    ~TMathVector() = default;

    size_t size() const noexcept { return this->get_size(); }

    TMathVector& operator+=(const TMathVector& rhs);
    TMathVector& operator-=(const TMathVector& rhs);
    TMathVector& operator*=(const T& scalar);
    TMathVector& operator/=(const T& scalar);

    TMathVector operator+(const TMathVector& rhs) const;
    TMathVector operator-(const TMathVector& rhs) const;
    TMathVector operator-() const;
    TMathVector operator*(const T& scalar) const;
    TMathVector operator/(const T& scalar) const;

    T dot(const TMathVector& rhs) const;
    T norm_squared() const;
    double norm() const;
    TMathVector<double> normalized() const;        

    double angle(const TMathVector& rhs) const;

    TMathVector cross(const TMathVector& rhs) const;

    bool is_zero() const;
    static TMathVector basis(size_t n, size_t axis); 
};

template <typename T>
TMathVector<T> operator*(const T& scalar, const TMathVector<T>& v);

template <typename T>
std::ostream& operator<<(std::ostream& out, const TMathVector<T>& v);