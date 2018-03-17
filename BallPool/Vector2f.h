#pragma once
#include <cmath>
#include <numeric>
// —труктура, моделирующа€ двумерный вектор
struct Vector2f;

Vector2f operator*(const Vector2f& v, float scale);
Vector2f operator*(float scale, const Vector2f& v);
Vector2f operator/(const Vector2f& v, float scale);

float dot(const Vector2f& a, const Vector2f& b);
Vector2f norm(const Vector2f& a);
Vector2f perp(const Vector2f& a);
float dist(const Vector2f& a, const Vector2f& b);

struct Vector2f
{
	float x = 0;
	float y = 0;

	Vector2f() = default;

	Vector2f(float x, float y)
		: x(x), y(y)
	{
	}

	inline float length() const
	{
		return std::hypot(x, y);
	}

	inline void normalize()
	{
		float len = length();
		if (len != 0) 
		{
			x /= len;
			y /= len;
		}
	}

	inline float projectOn(const Vector2f& other) const
	{
		return dot(*this, other) / other.length();
	}

	inline Vector2f operator+(const Vector2f& other) const
	{
		return { x + other.x, y + other.y };
	}

	inline Vector2f& operator+=(const Vector2f& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	inline Vector2f operator-(const Vector2f& other) const
	{
		return { x - other.x, y - other.y };
	}

	inline Vector2f& operator-=(const Vector2f& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	inline Vector2f operator-() const
	{
		return { -x, -y };
	}

	inline bool operator==(const Vector2f &a) const
	{
		return ((a.x == this->x) && (a.y == this->y));
	}

	inline bool operator!=(const Vector2f &a) const
	{
		return !(a == *this);
	}

	inline Vector2f& operator*=(float scale)
	{
		this->x *= scale;
		this->y *= scale;
		return *this;
	}

	inline Vector2f& operator/=(float scale)
	{
		this->x /= scale;
		this->y /= scale;
		return *this;
	}
};

inline Vector2f operator*(const Vector2f& v, float scale)
{
	return { scale * v.x, scale * v.y };
}

inline Vector2f operator*(float scale, const Vector2f& v)
{
	return v * scale;
}

inline Vector2f operator/(const Vector2f& v, float scale)
{
	return { v.x / scale, v.y / scale };
}

inline float dot(const Vector2f& a, const Vector2f& b)
{
	return a.x * b.x + a.y * b.y;
}

inline Vector2f norm(const Vector2f& a)
{
	float len = a.length();
	if (len == 0)
		return Vector2f{};
	return a / len;
}

inline Vector2f perp(const Vector2f& a)
{
	return Vector2f{ -a.y, a.x };
}

inline float dist(const Vector2f& a, const Vector2f& b)
{
	const Vector2f c = b - a;
	return c.length();
}