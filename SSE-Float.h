#ifndef __SSE_FLOAT_H__
#define __SSE_FLOAT_H__

#include <emmintrin.h>

class SSEFloat {
public:
	__m128 val;

	SSEFloat() {}
	SSEFloat(float f) { val= _mm_set1_ps(f);}
	SSEFloat(float f0, float f1,float f2, float f3) {val = _mm_setr_ps(f0,f1,f2,f3);}

	/* Arithmetic Operators*/
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

	/*Logical Operators*/
	friend inline SSEFloat operator &(const SSEFloat &a, const SSEFloat &b) {SSEFloat c;c.val= _mm_and_ps(a.val,b.val);return c;}
	friend inline SSEFloat operator |(const SSEFloat &a, const SSEFloat &b) {SSEFloat c;c.val= _mm_or_ps(a.val,b.val);return c;}
	friend inline SSEFloat operator ^(const SSEFloat &a, const SSEFloat &b) {SSEFloat c;c.val= _mm_xor_ps(a.val,b.val);return c;}
	friend inline SSEFloat andnot (const SSEFloat &a, const SSEFloat &b) {SSEFloat c;c.val= _mm_andnot_ps(a.val,b.val);return c;}

	/*Comparison Operators*/
	friend inline SSEFloat operator <(const SSEFloat &a, const SSEFloat &b) {SSEFloat c;c.val= _mm_cmplt_ps(a.val,b.val);return c;}
	friend inline SSEFloat operator >(const SSEFloat &a, const SSEFloat &b) {SSEFloat c;c.val= _mm_cmpgt_ps(a.val,b.val);return c;}
	friend inline SSEFloat operator ==(const SSEFloat &a, const SSEFloat &b) {SSEFloat c;c.val= _mm_cmpeq_ps(a.val,b.val);return c;}
	friend inline SSEFloat operator <(const SSEFloat &a, float b) {SSEFloat c;c.val= _mm_cmplt_ps(a.val,_mm_set1_ps(b));return c;}
	friend inline SSEFloat operator >(const SSEFloat &a, float b) {SSEFloat c;c.val= _mm_cmpgt_ps(a.val,_mm_set1_ps(b));return c;}
	friend inline SSEFloat max (const SSEFloat &a, SSEFloat &b) { SSEFloat c; c.val= _mm_max_ps(a.val,b.val);return c;}

	/*Masking Operations */
	friend inline int movemask( const SSEFloat &a) {return _mm_movemask_ps(a.val);}

	/*Store Operations*/
	friend inline void storeu(float *p, const SSEFloat &a) { _mm_storeu_ps(p,a.val);}

	friend inline SSEFloat rsqrt(const SSEFloat &a) {
		SSEFloat c;
		c.val= _mm_rsqrt_ps(a.val);

		// do one Newton-Raphson iteration to bring the precision to ~23 bits (~2e-7)
		c *= 0.5f * (3.0f - a * c * c);
		return c;
	}
};

#endif // __SSE_FLOAT_H__
