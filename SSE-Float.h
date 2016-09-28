#ifndef __SSE_FLOAT_H__
#define __SSE_FLOAT_H__

#include <emmintrin.h>

/*
 * \deprecated This has been replaced by cksimd<T>.
 */

class SSEFloat {
public:
	__m128 val;

	SSEFloat() {}
	SSEFloat(float f) { val= _mm_set1_ps(f);}
	SSEFloat(float f0, float f1,float f2, float f3) {val = _mm_setr_ps(f0,f1,f2,f3);}

	friend inline SSEFloat operator -(const SSEFloat &a) {SSEFloat c;c.val=_mm_sub_ps(_mm_setzero_ps(),a.val);return c;}
	friend inline SSEFloat operator +(const SSEFloat &a, const SSEFloat &b) {SSEFloat c;c.val= _mm_add_ps(a.val,b.val);return c;}
	friend inline SSEFloat operator -(const SSEFloat &a, const SSEFloat &b) {SSEFloat c;c.val= _mm_sub_ps(a.val,b.val);return c;}
	friend inline SSEFloat operator *(const SSEFloat &a, const SSEFloat &b) {SSEFloat c;c.val= _mm_mul_ps(a.val,b.val);return c;}
	friend inline SSEFloat operator /(const SSEFloat &a, const SSEFloat &b) {SSEFloat c;c.val= _mm_div_ps(a.val,b.val);return c;}
	friend inline SSEFloat sqrt      (const SSEFloat &a)                  { SSEFloat c;c.val= _mm_sqrt_ps(a.val);return c;}
	friend inline SSEFloat operator +(float a, const SSEFloat &b) {SSEFloat c;c.val= _mm_add_ps(_mm_set1_ps(a),b.val);return c;}
	friend inline SSEFloat operator -(float a, const SSEFloat &b) {SSEFloat c;c.val= _mm_sub_ps(_mm_set1_ps(a),b.val);return c;}
	friend inline SSEFloat operator *(float a, const SSEFloat &b) {SSEFloat c;c.val= _mm_mul_ps(_mm_set1_ps(a),b.val);return c;}
	friend inline SSEFloat operator /(float a, const SSEFloat &b) {SSEFloat c;c.val= _mm_div_ps(_mm_set1_ps(a),b.val);return c;}
	inline SSEFloat& operator +=(const SSEFloat &a) {val= _mm_add_ps(val,a.val);return *this;}
	inline SSEFloat& operator -=(const SSEFloat &a) {val= _mm_sub_ps(val,a.val);return *this;}
	inline SSEFloat& operator *=(const SSEFloat &a) {val= _mm_mul_ps(val,a.val);return *this;}
	inline SSEFloat& operator /=(const SSEFloat &a) {val= _mm_div_ps(val,a.val);return *this;}
	friend inline SSEFloat operator &(const SSEFloat &a, const SSEFloat &b) {SSEFloat c;c.val= _mm_and_ps(a.val,b.val);return c;}
	friend inline SSEFloat operator |(const SSEFloat &a, const SSEFloat &b) {SSEFloat c;c.val= _mm_or_ps(a.val,b.val);return c;}
	friend inline SSEFloat operator ^(const SSEFloat &a, const SSEFloat &b) {SSEFloat c;c.val= _mm_xor_ps(a.val,b.val);return c;}
	friend inline SSEFloat andnot (const SSEFloat &a, const SSEFloat &b) {SSEFloat c;c.val= _mm_andnot_ps(a.val,b.val);return c;}
	friend inline SSEFloat operator <(const SSEFloat &a, const SSEFloat &b) {SSEFloat c;c.val= _mm_cmplt_ps(a.val,b.val);return c;}
	friend inline SSEFloat operator >(const SSEFloat &a, const SSEFloat &b) {SSEFloat c;c.val= _mm_cmpgt_ps(a.val,b.val);return c;}
	friend inline SSEFloat operator ==(const SSEFloat &a, const SSEFloat &b) {SSEFloat c;c.val= _mm_cmpeq_ps(a.val,b.val);return c;}
	friend inline SSEFloat operator <(const SSEFloat &a, float b) {SSEFloat c;c.val= _mm_cmplt_ps(a.val,_mm_set1_ps(b));return c;}
	friend inline SSEFloat operator >(const SSEFloat &a, float b) {SSEFloat c;c.val= _mm_cmpgt_ps(a.val,_mm_set1_ps(b));return c;}
	friend inline SSEFloat max (const SSEFloat &a, SSEFloat &b) { SSEFloat c; c.val= _mm_max_ps(a.val,b.val);return c;}
	friend inline int movemask( const SSEFloat &a) {return _mm_movemask_ps(a.val);}
	friend inline void storeu(float *p, const SSEFloat &a) { _mm_storeu_ps(p,a.val);}
};

#endif
