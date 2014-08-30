#pragma once
#include <algorithm>
template <typename T>
class Vec3D
{
public:
	Vec3D() {};
	Vec3D(Vec3D&& other) : Vec3D() { swap(*this, other); };
	Vec3D(T x, T y, T z) : x(x), y(y), z(z) {};
	~Vec3D() {};
	T x, y, z;
	inline bool operator==(const Vec3D<T>& rhs) {
		if ((*this).x == rhs.x && (*this).y == rhs.y && (*this).z == rhs.z) {
			return true;
		}
		return false;
	};
	inline bool operator!=(const Vec3D<T>& rhs) {
		return !((*this) == rhs);
	};
	Vec3D& operator=(Vec3D temp) {
		std::swap((*this).x, temp.x);
		std::swap((*this).y, temp.y);
		std::swap((*this).z, temp.z);
		return *this;
	};
};