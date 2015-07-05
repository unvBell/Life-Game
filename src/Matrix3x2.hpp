#pragma once

#include <ostream>
#include <boost/optional.hpp>

struct Matrix3x2 {
	double a11, a12;
	double a21, a22;
	double a31, a32;

	Matrix3x2()
		: a11(0), a12(0)
		, a21(0), a22(0)
		, a31(0), a32(0)
	{}

	Matrix3x2(
		double _a11, double _a12,
		double _a21, double _a22,
		double _a31, double _a32
	)
		: a11(_a11), a12(_a12)
		, a21(_a21), a22(_a22)
		, a31(_a31), a32(_a32)
	{}


	explicit operator bool() const {
		return !isZero();
	}

	Matrix3x2 operator+(const Matrix3x2& m) const {
		return {
			a11+m.a11, a12+m.a12,
			a21+m.a21, a22+m.a22,
			a31+m.a31, a32+m.a32,
		};
	}

	Matrix3x2 operator-(const Matrix3x2& m) const {
		return {
			a11-m.a11, a12-m.a12,
			a21-m.a21, a22-m.a22,
			a31-m.a31, a32-m.a32,
		};
	}

	Matrix3x2 operator*(double a) const {
		return {
			a11*a, a12*a,
			a21*a, a22*a,
			a31*a, a32*a,
		};
	}

	Matrix3x2 operator/(double a) const {
		return {
			a11/a, a12/a,
			a21/a, a22/a,
			a31/a, a32/a,
		};
	}

	friend Matrix3x2 operator*(double a, const Matrix3x2& m) {
		return {
			a*m.a11, a*m.a12,
			a*m.a21, a*m.a22,
			a*m.a31, a*m.a32,
		};
	}

	Matrix3x2 operator*(const Matrix3x2& m) const {
		return multiply(m);
	}

	Matrix3x2& operator+=(const Matrix3x2& m) {
		return *this = *this+m;
	}

	Matrix3x2& operator-=(const Matrix3x2& m) {
		return *this = *this-m;
	}

	Matrix3x2& operator*=(double a) {
		return *this = *this*a;
	}

	Matrix3x2& operator/=(double a) {
		return *this = *this/a;
	}

	Matrix3x2& operator*=(const Matrix3x2& m) {
		return *this = *this*m;
	}

	bool operator==(const Matrix3x2& m) const {
		return equals(m);
	}

	bool operator!=(const Matrix3x2& m) const {
		return !equals(m);
	}


	static Matrix3x2 zero() {
		return {};
	}


	static Matrix3x2 identity() {
		return {
			1, 0,
			0, 1,
			0, 0,
		};
	}


	static Matrix3x2 translation(double x, double y) {
		return {
			1, 0,
			0, 1,
			x, y,
		};
	}


	static Matrix3x2 scaling(double x, double y) {
		return {
			x, 0,
			0, y,
			0, 0,
		};
	}


	static Matrix3x2 scaling(double sx, double sy, double cx, double cy) {
		return {
			sx        , 0         ,
			0         , sy        ,
			cx - sx*cx, cy - sy*cy,
		};
	}


	static Matrix3x2 rotation(double rot_rad) {
		double c = std::cos(rot_rad);
		double s = std::sin(rot_rad);

		return {
			 c, s,
			-s, c,
			 0, 0,
		};
	}


	static Matrix3x2 rotation(double rot_rad, double cx, double cy) {
		double c = std::cos(rot_rad);
		double s = std::sin(rot_rad);

		return {
			 c          , s           ,
			-s          , c           ,
			cx-c*cx+s*cy, cy-s*cx-c*cy,
		};
	}


	Matrix3x2& set(
		double _a11, double _a12,
		double _a21, double _a22,
		double _a31, double _a32
	) {
		a11 = _a11; a12 = _a12;
		a21 = _a21; a22 = _a22;
		a31 = _a31; a32 = _a32;
		return *this;
	}


	bool isZero() const {
		return
			a11 == 0 && a12 == 0 &&
			a21 == 0 && a22 == 0 &&
			a31 == 0 && a32 == 0;
	}


	bool isIdentity() const {
		return
			a11 == 1 && a12 == 0 &&
			a21 == 0 && a22 == 1 &&
			a31 == 0 && a32 == 0;
	}


	bool isInvertible() const {
		return determinant() != 0;
	}


	bool equals(const Matrix3x2& m) const {
		return
			a11 == m.a11 && a12 == m.a12 &&
			a21 == m.a21 && a22 == m.a22 &&
			a31 == m.a31 && a32 == m.a32;
	}


	double determinant() const {
		return a11*a22 - a12*a21;
	}


	boost::optional<Matrix3x2> invert() const {
		if(double d = determinant()) {
			return Matrix3x2 {
				 a22             /d, -a12             /d,
				-a21             /d,  a11             /d,
				(a21*a32-a22*a31)/d, (a12*a31-a11*a32)/d,
			};
		}

		return boost::none;
	}


	Matrix3x2 multiply(const Matrix3x2& m) const {
		return {
			a11*m.a11+a12*m.a21      , a11*m.a12+a12*m.a22      ,
			a21*m.a11+a22*m.a21      , a21*m.a12+a22*m.a22      ,
			a31*m.a11+a32*m.a21+m.a31, a31*m.a12+a32*m.a22+m.a32,
		};
	}
};
