#pragma once
#include <algorithm>
template <typename T>
class Vec4D
{
public:
	Vec4D() {};
	Vec4D(Vec4D&& other) : Vec4D() { swap(*this, other); };
	Vec4D(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {};
	~Vec4D() {};
	T x, y, z, w;
	inline bool operator==(const Vec4D<T>& rhs) {
		if ((*this).x == rhs.x && (*this).y == rhs.y && (*this).z == rhs.z && (*this).w == rhs.w) {
			return true;
		}
		return false;
	};
	inline bool operator!=(const Vec4D<T>& rhs) {
		return !((*this) == rhs);
	};
	Vec4D& operator=(Vec4D temp) {
		std::swap((*this).x, temp.x);
		std::swap((*this).y, temp.y);
		std::swap((*this).z, temp.z);
		std::swap((*this).w, temp.w);
		return *this;
	};
};