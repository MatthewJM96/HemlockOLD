#pragma once
#include <algorithm>
template <typename T>
class Vec2D
{
public:
	Vec2D() {};
	Vec2D(Vec2D&& other) : Vec2D() { swap(*this, other); };
	Vec2D(T x, T y) : x(x), y(y) {};
	~Vec2D() {};
	T x, y;
	inline bool operator==(const Vec2D<T>& rhs) {
		if ((*this).x == rhs.x && (*this).y == rhs.y) {
			return true;
		}
		return false;
	};
	inline bool operator!=(const Vec2D<T>& rhs) {
		return !((*this) == rhs);
	};
	Vec2D& operator=(Vec2D temp) {
		std::swap((*this).x, temp.x);
		std::swap((*this).y, temp.y);
		return *this;
	};
};