#pragma once 

#include <math.h>

class Vector2D {
public:
	Vector2D(float x, float y) : m_x(x), m_y(y) {}
	float getX() { return m_x; }
	float getY() { return m_y; }
	void setX(float x) { m_x = x; }
	void setY(float y) { m_y = y; }

	void limit(float max)
	{
		float l = length();
		if (l > 0)
		{
			if (l > max)
			{
				(*this) *= 1 / l;
				(*this) *= max;
			}
		}
	}

	float length() { return sqrt(m_x * m_x + m_y * m_y); }
	float lengthSqaured() { return m_x * m_x + m_y * m_y; }

	Vector2D operator+(const Vector2D& v2) const
	{
		return Vector2D(m_x + v2.m_x, m_y + v2.m_y);
	}

	friend Vector2D& operator+=(Vector2D& v1, const Vector2D& v2)
	{
		v1.m_x += v2.m_x;
		v1.m_y += v2.m_y;
		return v1;
	}

	Vector2D operator-(const Vector2D& v2) const
	{
		return Vector2D(m_x - v2.m_x, m_y - v2.m_y);
	}

	friend Vector2D& operator-=(Vector2D& v1, const Vector2D& v2)
	{
		v1.m_x -= v2.m_x;
		v1.m_y -= v2.m_y;
		return v1;
	}

	Vector2D operator*(float scalar)
	{
		return Vector2D(m_x * scalar, m_y * scalar);
	}

	Vector2D& operator*=(float scalar)
	{
		m_x *= scalar;
		m_y *= scalar;
		return *this;
	}

	Vector2D operator/(float scalar)
	{
		return Vector2D(m_x / scalar, m_y / scalar);
	}

	Vector2D& operator/=(float scalar)
	{
		m_x /= scalar;
		m_y /= scalar;
		return *this;
	}
	void normalize()
	{
		float l = length();
		if (l > 0)   // we never want to attempt to divide by 0
		{
			(*this) *= 1 / l;
		}
	}

	Vector2D Normalized()
	{
		float len = length();
		if (len > 0)
		{
			return Vector2D((*this).m_x / len, (*this).m_y / len);
		}

		return Vector2D(0, 0);
	}

	Vector2D RotTranslate(float radian, float tx, float ty)
	{
		return Vector2D((*this).m_x * cos(radian) - (*this).m_y * sin(radian) + tx, (*this).m_x * sin(radian) + (*this).m_y * cos(radian) + ty);
	}

	Vector2D RotTranslate(float radian, Vector2D transform)
	{
		return Vector2D((*this).m_x * cos(radian) - (*this).m_y * sin(radian) + transform.m_x, (*this).m_x * sin(radian) + (*this).m_y * cos(radian) + transform.m_y);
	}

	Vector2D RotateAngle(float radian)
	{
		return Vector2D((*this).m_x * cos(radian) - (*this).m_y * sin(radian), (*this).m_x * sin(radian) + (*this).m_y * cos(radian));
	}

	float constrainSquare(float min, float max) const
	{
		float distSquare = m_x * m_x + m_y * m_y;
		if (distSquare > 0)
		{
			if (distSquare > max)
			{
				return max;
			}
			else if (distSquare < min)
			{
				return min;
			}
		}

		return distSquare;
	}

private:
	float m_x;
	float m_y;
};