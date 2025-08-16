#pragma once
#include <iostream>
#include <cmath>

#define VEC2_INLINE [[nodiscard]] constexpr inline

using uint = unsigned int;
using uchar = unsigned char;

template <typename T = float>
struct Vec2 {
    T x, y;

    //Constructors
    constexpr Vec2() : x(0), y(0) {}
    constexpr Vec2(T x) : x(x), y(x) {}
    constexpr Vec2(T x, T y) : x(x), y(y) {}

    //Conversion constructor
    template<typename U>
    explicit constexpr Vec2(const Vec2<U>& other) : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)) {}

    //Negation
    constexpr Vec2 operator-() const { return { -x, -y }; }

    //Addition
    VEC2_INLINE Vec2 operator+(const Vec2& other) const { return Vec2(x + other.x, y + other.y); }
    VEC2_INLINE Vec2& operator+=(const Vec2& other) {
        x += other.x; y += other.y;
        return *this;
    }
    template<typename U>
    Vec2& operator+=(const Vec2<U>& rhs) {
        x += static_cast<T>(rhs.x);
        y += static_cast<T>(rhs.y);
        return *this;
    }

    //Subtraction
    VEC2_INLINE Vec2 operator-(const Vec2& other) const { return Vec2(x - other.x, y - other.y); }
    VEC2_INLINE Vec2& operator-=(const Vec2& other) {
        x -= other.x; y -= other.y;
        return *this;
    }
    template<typename U>
    Vec2& operator-=(const Vec2<U>& rhs) {
        x -= static_cast<T>(rhs.x);
        y -= static_cast<T>(rhs.y);
        return *this;
    }

    //Multiplication
    VEC2_INLINE Vec2 operator*(T scalar) const { return { x * scalar, y * scalar }; }
    VEC2_INLINE Vec2& operator*=(T scalar) {
        x *= scalar; y *= scalar;
        return *this;
    }
    template<typename U>
    VEC2_INLINE Vec2<std::common_type_t<T, U>> operator*(const Vec2<U>& rhs) const {
        return { x * rhs.x, y * rhs.y };
    }

    //Division
    VEC2_INLINE Vec2 operator/(T scalar) const { return { x / scalar, y / scalar }; }
    VEC2_INLINE Vec2& operator/=(T scalar) {
        x /= scalar; y /= scalar;
        return *this;
    }
    template<typename U>
    VEC2_INLINE Vec2<std::common_type_t<T, U>> operator/(const Vec2<U>& rhs) const {
        return { x / rhs.x, y / rhs.y };
    }

    //(In)Equality
    VEC2_INLINE bool operator==(const Vec2& other) const { return x == other.x and y == other.y; }
    VEC2_INLINE bool operator!=(const Vec2& other) const { return x != other.x or y != other.y; }

    //Magnitude (Length)
    [[nodiscard]] T Length() const { return std::sqrt(x * x + y * y); }
    //Normalize
    [[nodiscard]] constexpr Vec2 Normalized() const {
        T len = Length();
        return (len != 0) ? *this / len : *this;
    }
    //Dot product
    [[nodiscard]] constexpr T Dot(const Vec2& other) const { return x * other.x + y * other.y; }
};

//ostream operator
template <typename T>
std::ostream& operator<<(std::ostream& os, const Vec2<T>& v) { return os << v.x << ", " << v.y; }

//Rounding
template<typename T>
[[nodiscard]] constexpr Vec2<int> Round(const Vec2<T>& v) {
    return { static_cast<int>(std::round(v.x)), static_cast<int>(std::round(v.y)) };
}
template<typename T>
[[nodiscard]] constexpr Vec2<int> Round(const T x, const T y) {
    return { static_cast<int>(std::round(x)), static_cast<int>(std::round(y)) };
}
template<typename T, typename U>
[[nodiscard]] constexpr Vec2<int> Round(const T x, const U y) {
    return { static_cast<int>(std::round(x)), static_cast<int>(std::round(y)) };
}

//Distance
template<typename T>
[[nodiscard]] constexpr float Distance(const Vec2<T>& v1, const Vec2<T>& v2) {
    return sqrt((v2.x - v1.x) * (v2.x - v1.x) + (v2.y - v1.y) * (v2.y - v1.y));
}
template<typename T, typename U>
[[nodiscard]] constexpr float Distance(const Vec2<T>& v1, const Vec2<U>& v2) {
    return sqrt((v2.x - v1.x) * (v2.x - v1.x) + (v2.y - v1.y) * (v2.y - v1.y));
}

//Universal scalar multiplication & division
template<typename T>
constexpr Vec2<T> operator*(T scalar, const Vec2<T>& v) { return v * scalar; }
template<typename T, typename U>
constexpr auto operator*(const Vec2<T>& vec, U scalar)
-> Vec2<std::common_type_t<T, U>> {
    using R = std::common_type_t<T, U>;
    return { static_cast<R>(vec.x) * scalar, static_cast<R>(vec.y) * scalar };
}

template<typename T>
constexpr Vec2<T> operator/(T scalar, const Vec2<T>& v) { return v / scalar; }
template<typename T, typename U>
constexpr auto operator/(const Vec2<T>& vec, U scalar)
-> Vec2<std::common_type_t<T, U>> {
    using R = std::common_type_t<T, U>;
    return { static_cast<R>(vec.x) / scalar, static_cast<R>(vec.y) / scalar };
}

using Vec2i = Vec2<int>;
using Vec2u = Vec2<uint>;
using Vec2f = Vec2<float>;
using Vec2c = Vec2<char>;
using Vec2uc = Vec2<uchar>;