#pragma once

namespace GomiEngine::Math
{
    struct Vector4
    {
        union { float x; float r; };
        union { float y; float g; };
        union { float z; float b; };
        union { float w; float a; };

        constexpr Vector4() noexcept : Vector4(0.0f) {}
        explicit constexpr Vector4(float f) noexcept : Vector4(f, f, f, f) {}
        constexpr Vector4(float x, float y, float z, float w) noexcept : x{ x }, y{ y }, z{ z }, w{ w } {}

        // commonly used vectors
        const static Vector4 Zero;
        const static Vector4 One;
        const static Vector4 XAxis;
        const static Vector4 YAxis;
        const static Vector4 ZAxis;

        constexpr Vector4 operator-() const { return { -x, -y, -z, -w }; }
        constexpr Vector4 operator+(Vector4 rhs) const { return { x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w }; }
        constexpr Vector4 operator-(Vector4 rhs) const { return { x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w }; }
        constexpr Vector4 operator*(float s) const { return { x * s, y * s, z * s, w * s }; }
        constexpr Vector4 operator/(float s) const { return { x / s, y / s, z / s, w / s }; }

        constexpr Vector4& operator+=(const Vector4& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w; return *this; }
        constexpr Vector4& operator-=(const Vector4& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w; return *this; }
        constexpr Vector4& operator*=(float s) { x *= s; y *= s; z *= s; w *= s; return *this; }
        constexpr Vector4& operator/=(float s) { x /= s; y /= s; z /= s; w /= s; return *this; }
    };
}
