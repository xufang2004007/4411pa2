/*
The given vec.h and mat.h is FAR TOO WEAK!
We need some extra helpers
*/

#ifndef __VECTOR_MATRIX_EXTENDED_HEADER__
#define __VECTOR_MATRIX_EXTENDED_HEADER__

#include "vec.h"
#include "mat.h"

template <class T>
inline Vec3<T> cross_product(Vec3<T> s1, Vec3<T> s2) {
	return Vec3<T>(s1[1] * s2[2] - s1[2] * s2[1], -s1[0] * s2[2] + s1[2] * s2[0], s1[0] * s2[1] - s1[1] * s2[0]);
}

template <class T>
Mat4<T> Mat4fromGLMatrix(T* n) {
	return Mat4<T>(n[0], n[4], n[8], n[12], n[1], n[5], n[9], n[13], n[2], n[6], n[10], n[14], n[3], n[7], n[11], n[15]);
}

#endif