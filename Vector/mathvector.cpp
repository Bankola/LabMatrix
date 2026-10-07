#include "mathvector.h"

template <typename T>
TMathVector<T>::TMathVector() : Vector<T>() {}

template <typename T>
TMathVector<T>::TMathVector(size_t n) : Vector<T>(n) {}

template <typename T>
TMathVector<T>::TMathVector(size_t n, const T& value) : Vector<T>(n) {
    for (size_t i = 0; i < n; i++) (*this)[i] = value;
}

template <typename T>
TMathVector<T>::TMathVector(std::initializer_list<T> list) : Vector<T>() {
    for (const auto& v : list) this->push_back(v);
}

template <typename T>
TMathVector<T>& TMathVector<T>::operator+=(const TMathVector& rhs) {
    if (this->get_size() != rhs.get_size())
        throw std::invalid_argument("ERROR: TMathVector sizes differ (+=)");
    for (size_t i = 0; i < this->get_size(); i++) (*this)[i] += rhs[i];
    return *this;
}

template <typename T>
TMathVector<T>& TMathVector<T>::operator-=(const TMathVector& rhs) {
    if (this->get_size() != rhs.get_size())
        throw std::invalid_argument("ERROR: TMathVector sizes differ (-=)");
    for (size_t i = 0; i < this->get_size(); i++) (*this)[i] -= rhs[i];
    return *this;
}

template <typename T>
TMathVector<T>& TMathVector<T>::operator*=(const T& scalar) {
    for (size_t i = 0; i < this->get_size(); i++) (*this)[i] *= scalar;
    return *this;
}

template <typename T>
TMathVector<T>& TMathVector<T>::operator/=(const T& scalar) {
    if (scalar == T{})
        throw std::domain_error("ERROR: TMathVector division by zero");
    for (size_t i = 0; i < this->get_size(); i++) (*this)[i] /= scalar;
    return *this;
}

template <typename T>
TMathVector<T> TMathVector<T>::operator+(const TMathVector& rhs) const {
    TMathVector r(*this); r += rhs; return r;
}

template <typename T>
TMathVector<T> TMathVector<T>::operator-(const TMathVector& rhs) const {
    TMathVector r(*this); r -= rhs; return r;
}

template <typename T>
TMathVector<T> TMathVector<T>::operator-() const {
    TMathVector r(this->get_size());
    for (size_t i = 0; i < this->get_size(); i++) r[i] = -(*this)[i];
    return r;
}

template <typename T>
TMathVector<T> TMathVector<T>::operator*(const T& scalar) const {
    TMathVector r(*this); r *= scalar; return r;
}

template <typename T>
TMathVector<T> TMathVector<T>::operator/(const T& scalar) const {
    TMathVector r(*this); r /= scalar; return r;
}

template <typename T>
T TMathVector<T>::dot(const TMathVector& rhs) const {
    if (this->get_size() != rhs.get_size())
        throw std::invalid_argument("ERROR: TMathVector sizes differ (dot)");
    T s = T{};
    for (size_t i = 0; i < this->get_size(); i++) s += (*this)[i] * rhs[i];
    return s;
}

template <typename T>
T TMathVector<T>::norm_squared() const { return dot(*this); }

template <typename T>
double TMathVector<T>::norm() const {
    return std::sqrt(static_cast<double>(norm_squared()));
}

template <typename T>
TMathVector<double> TMathVector<T>::normalized() const {
    double n = norm();
    if (n == 0.0)
        throw std::domain_error("ERROR: Cannot normalize zero vector");
    TMathVector<double> r;
    for (size_t i = 0; i < this->get_size(); i++)
        r.push_back(static_cast<double>((*this)[i]) / n);
    return r;
}

template <typename T>
TMathVector<T> TMathVector<T>::cross(const TMathVector& rhs) const {
    if (this->get_size() != 3 || rhs.get_size() != 3)
        throw std::invalid_argument("ERROR: cross is only defined for size == 3");
    TMathVector r;
    r.push_back((*this)[1] * rhs[2] - (*this)[2] * rhs[1]);
    r.push_back((*this)[2] * rhs[0] - (*this)[0] * rhs[2]);
    r.push_back((*this)[0] * rhs[1] - (*this)[1] * rhs[0]);
    return r;
}

template <typename T>
TMathVector<T> TMathVector<T>::basis(size_t n, size_t axis) {
    if (axis >= n)
        throw std::out_of_range("ERROR: TMathVector::basis axis out of range");
    TMathVector r;
    for (size_t i = 0; i < n; i++)
        r.push_back(i == axis ? static_cast<T>(1) : T{});
    return r;
}

template <typename T>
double TMathVector<T>::angle(const TMathVector& rhs) const {
    double n1 = norm(), n2 = rhs.norm();
    if (n1 == 0.0 || n2 == 0.0)
        throw std::domain_error("ERROR: Cannot compute angle with zero vector");
    double c = static_cast<double>(dot(rhs)) / (n1 * n2);
    if (c > 1.0) c = 1.0;
    if (c < -1.0) c = -1.0;
    return std::acos(c);
}

template <typename T>
bool TMathVector<T>::is_zero() const {
    for (size_t i = 0; i < this->get_size(); i++)
        if ((*this)[i] != T{}) return false;
    return true;
}
template <typename T>
TMathVector<T> operator*(const T& scalar, const TMathVector<T>& v) {
    return v * scalar;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const TMathVector<T>& v) {
    out << "(";
    for (size_t i = 0; i < v.get_size(); i++) {
        if (i) out << ", ";
        out << v[i];
    }
    out << ")";
    return out;
}

template class TMathVector<double>;
template class TMathVector<int>;

template TMathVector<double> operator*(const double&, const TMathVector<double>&);
template TMathVector<int>    operator*(const int&, const TMathVector<int>&);

template std::ostream& operator<<(std::ostream&, const TMathVector<double>&);
template std::ostream& operator<<(std::ostream&, const TMathVector<int>&);