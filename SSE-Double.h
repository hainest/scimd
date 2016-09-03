#ifndef __SSE_DOUBLE_H__
#define __SSE_DOUBLE_H__

#if CMK_USE_AVX && defined(__AVX__)

#include <x86intrin.h>

#include<iostream>


class SSEDouble
{

   public: __m256d val;


   public:

           SSEDouble() {}

           SSEDouble(double d) { val = _mm256_set1_pd(d); }

           SSEDouble(double d0, double d1, double d2, double d3) {  val = _mm256_setr_pd(d0,d1,d2,d3); }

           /* Arithmetic Operators*/
           friend inline SSEDouble operator -(const SSEDouble &a) {SSEDouble c;c.val=_mm256_sub_pd(_mm256_setzero_pd(),a.val);return c;}

           friend inline SSEDouble operator +(const SSEDouble &a, const SSEDouble &b) {SSEDouble c;c.val= _mm256_add_pd(a.val,b.val);return c;}

           friend inline SSEDouble operator -(const SSEDouble &a, const SSEDouble &b) {SSEDouble c;c.val= _mm256_sub_pd(a.val,b.val);return c;}

           friend inline SSEDouble operator *(const SSEDouble &a, const SSEDouble &b) {SSEDouble c;c.val= _mm256_mul_pd(a.val,b.val);return c;}

           friend inline SSEDouble operator /(const SSEDouble &a, const SSEDouble &b) {SSEDouble c;c.val= _mm256_div_pd(a.val,b.val);return c;}

           friend inline SSEDouble sqrt      (const SSEDouble &a)                  { SSEDouble c;c.val= _mm256_sqrt_pd(a.val);return c;}


          friend inline SSEDouble operator +(double a, const SSEDouble &b) {SSEDouble c;c.val= _mm256_add_pd(_mm256_set1_pd(a),b.val);return c;}


          friend inline SSEDouble operator -(double a, const SSEDouble &b) {SSEDouble c;c.val= _mm256_sub_pd(_mm256_set1_pd(a),b.val);return c;}

          friend inline SSEDouble operator *(double a, const SSEDouble &b) {SSEDouble c;c.val= _mm256_mul_pd(_mm256_set1_pd(a),b.val);return c;}

          friend inline SSEDouble operator /(double a, const SSEDouble &b) {SSEDouble c;c.val= _mm256_div_pd(_mm256_set1_pd(a),b.val);return c;}

           inline SSEDouble& operator +=(const SSEDouble &a) {val= _mm256_add_pd(val,a.val);return *this;}

           inline SSEDouble& operator -=(const SSEDouble &a) {val= _mm256_sub_pd(val,a.val);return *this;}

           inline SSEDouble& operator *=(const SSEDouble &a) {val= _mm256_mul_pd(val,a.val);return *this;}

           inline SSEDouble& operator /=(const SSEDouble &a) {val= _mm256_div_pd(val,a.val);return *this;}

           /*Logical Operators*/

           friend inline SSEDouble operator &(const SSEDouble &a, const SSEDouble &b) {SSEDouble c;c.val= _mm256_and_pd(a.val,b.val);return c;}

           friend inline SSEDouble operator |(const SSEDouble &a, const SSEDouble &b) {SSEDouble c;c.val= _mm256_or_pd(a.val,b.val);return c;}

           friend inline SSEDouble operator ^(const SSEDouble &a, const SSEDouble &b) {SSEDouble c;c.val= _mm256_xor_pd(a.val,b.val);return c;}

           friend inline SSEDouble andnot (const SSEDouble &a, const SSEDouble &b) {SSEDouble c;c.val= _mm256_andnot_pd(a.val,b.val);return c;}

         /*Comparison Operators*/

            //friend inline SSEDouble operator <(const SSEDouble &a, const SSEDouble &b) {SSEDouble c;c.val= _mm256_cmplt_pd(a.val,b.val);return c;}
            friend inline SSEDouble operator <(const SSEDouble &a, const SSEDouble &b) {SSEDouble c;c.val= _mm256_cmp_pd(a.val,b.val,_CMP_LT_OS);return c;}

            //friend inline SSEDouble operator >(const SSEDouble &a, const SSEDouble &b) {SSEDouble c;c.val= _mm256_cmpgt_pd(a.val,b.val);return c;}
            friend inline SSEDouble operator >(const SSEDouble &a, const SSEDouble &b) {SSEDouble c;c.val= _mm256_cmp_pd(a.val,b.val,_CMP_GT_OS);return c;}

            //friend inline SSEDouble operator ==(const SSEDouble &a, const SSEDouble &b) {SSEDouble c;c.val= _mm256_cmpeq_pd(a.val,b.val);return c;}
            friend inline SSEDouble operator ==(const SSEDouble &a, const SSEDouble &b) {SSEDouble c;c.val= _mm256_cmp_pd(a.val,b.val,_CMP_EQ_OQ);return c;}

            //friend inline SSEDouble operator <(const SSEDouble &a, double b) {SSEDouble c;c.val= _mm256_cmplt_pd(a.val,_mm256_set1_pd(b));return c;}
            friend inline SSEDouble operator <(const SSEDouble &a, double b) {SSEDouble c;c.val= _mm256_cmp_pd(a.val,_mm256_set1_pd(b),_CMP_LT_OS);return c;}

            //friend inline SSEDouble operator >(const SSEDouble &a, double b) {SSEDouble c;c.val= _mm256_cmpgt_pd(a.val,_mm256_set1_pd(b));return c;}
            friend inline SSEDouble operator >(const SSEDouble &a, double b) {SSEDouble c;c.val= _mm256_cmp_pd(a.val,_mm256_set1_pd(b),_CMP_GT_OS);return c;}

            friend inline SSEDouble max (const SSEDouble &a, SSEDouble &b) { SSEDouble c; c.val= _mm256_max_pd(a.val,b.val);return c;}


        /*Masking Operations */

           friend inline int movemask( const SSEDouble &a) {return _mm256_movemask_pd(a.val);}


        /*Store Operations*/

          friend inline void storeu(double *p, const SSEDouble &a) { _mm256_storeu_pd(p,a.val);}


       //   void display();


	/**
	* 	This routine is adapted from
	* 	https://github.com/stgatilov/recip_rsqrt_benchmark
	*
	* 	It uses a 5th-order polynomial to bring the error to 51.5 bits (~3e-16).
	*
	* 	The speedup compared to using sqrt+div varies by CPU architecture, but
	* 	is between 10% (Skylake) and 3x (Sandybridge) faster.
	*/
	friend inline SSEDouble rsqrt(SSEDouble a) {
		__m256d one = _mm256_set1_pd(1.0), 		c1 = _mm256_set1_pd(1.0/2.0),
				c2 = _mm256_set1_pd(3.0/8.0), 	c3 = _mm256_set1_pd(15.0/48.0),
				c4 = _mm256_set1_pd(105.0/384.0);
		__m256d x = _mm256_cvtps_pd(_mm_rsqrt_ps(_mm256_cvtpd_ps(a.val)));
		__m256d r = _mm256_sub_pd(one, _mm256_mul_pd(_mm256_mul_pd(a.val, x), x));
		__m256d r2 = _mm256_mul_pd(r, r);
		__m256d t1 = _mm256_add_pd(_mm256_mul_pd(c2, r), c1);
		__m256d t3 = _mm256_add_pd(_mm256_mul_pd(c4, r), c3);
		__m256d poly = _mm256_add_pd(_mm256_mul_pd(r2, t3), t1);
		__m256d res = _mm256_add_pd(_mm256_mul_pd(_mm256_mul_pd(x, r), poly), x);
		SSEDouble c;
		c.val = res;
		return c;
	}
};


#else


#include<emmintrin.h>

#include<iostream>



class SSEDouble
{

   public: __m128d val;


   public:

           SSEDouble() {}

           SSEDouble(double d) { val= _mm_set1_pd(d);}

           SSEDouble(double d0, double d1) {val = _mm_setr_pd(d0,d1);}

           /* Arithmetic Operators*/
           friend inline SSEDouble operator -(const SSEDouble &a) {SSEDouble c;c.val=_mm_sub_pd(_mm_setzero_pd(),a.val);return c;}

           friend inline SSEDouble operator +(const SSEDouble &a, const SSEDouble &b) {SSEDouble c;c.val= _mm_add_pd(a.val,b.val);return c;}

           friend inline SSEDouble operator -(const SSEDouble &a, const SSEDouble &b) {SSEDouble c;c.val= _mm_sub_pd(a.val,b.val);return c;}

           friend inline SSEDouble operator *(const SSEDouble &a, const SSEDouble &b) {SSEDouble c;c.val= _mm_mul_pd(a.val,b.val);return c;}

           friend inline SSEDouble operator /(const SSEDouble &a, const SSEDouble &b) {SSEDouble c;c.val= _mm_div_pd(a.val,b.val);return c;}

           friend inline SSEDouble sqrt      (const SSEDouble &a)                  { SSEDouble c;c.val= _mm_sqrt_pd(a.val);return c;}


          friend inline SSEDouble operator +(double a, const SSEDouble &b) {SSEDouble c;c.val= _mm_add_pd(_mm_set1_pd(a),b.val);return c;}


          friend inline SSEDouble operator -(double a, const SSEDouble &b) {SSEDouble c;c.val= _mm_sub_pd(_mm_set1_pd(a),b.val);return c;}

          friend inline SSEDouble operator *(double a, const SSEDouble &b) {SSEDouble c;c.val= _mm_mul_pd(_mm_set1_pd(a),b.val);return c;}

          friend inline SSEDouble operator /(double a, const SSEDouble &b) {SSEDouble c;c.val= _mm_div_pd(_mm_set1_pd(a),b.val);return c;}

           inline SSEDouble& operator +=(const SSEDouble &a) {val= _mm_add_pd(val,a.val);return *this;}

           inline SSEDouble& operator -=(const SSEDouble &a) {val= _mm_sub_pd(val,a.val);return *this;}

           inline SSEDouble& operator *=(const SSEDouble &a) {val= _mm_mul_pd(val,a.val);return *this;}

           inline SSEDouble& operator /=(const SSEDouble &a) {val= _mm_div_pd(val,a.val);return *this;}

           /*Logical Operators*/

           friend inline SSEDouble operator &(const SSEDouble &a, const SSEDouble &b) {SSEDouble c;c.val= _mm_and_pd(a.val,b.val);return c;}

           friend inline SSEDouble operator |(const SSEDouble &a, const SSEDouble &b) {SSEDouble c;c.val= _mm_or_pd(a.val,b.val);return c;}

           friend inline SSEDouble operator ^(const SSEDouble &a, const SSEDouble &b) {SSEDouble c;c.val= _mm_xor_pd(a.val,b.val);return c;}

           friend inline SSEDouble andnot (const SSEDouble &a, const SSEDouble &b) {SSEDouble c;c.val= _mm_andnot_pd(a.val,b.val);return c;}

         /*Comparison Operators*/


            friend inline SSEDouble operator <(const SSEDouble &a, const SSEDouble &b) {SSEDouble c;c.val= _mm_cmplt_pd(a.val,b.val);return c;}

            friend inline SSEDouble operator >(const SSEDouble &a, const SSEDouble &b) {SSEDouble c;c.val= _mm_cmpgt_pd(a.val,b.val);return c;}

            friend inline SSEDouble operator ==(const SSEDouble &a, const SSEDouble &b) {SSEDouble c;c.val= _mm_cmpeq_pd(a.val,b.val);return c;}

            friend inline SSEDouble operator <(const SSEDouble &a, double b) {SSEDouble c;c.val= _mm_cmplt_pd(a.val,_mm_set1_pd(b));return c;}

            friend inline SSEDouble operator >(const SSEDouble &a, double b) {SSEDouble c;c.val= _mm_cmpgt_pd(a.val,_mm_set1_pd(b));return c;}

            friend inline SSEDouble max (const SSEDouble &a, SSEDouble &b) { SSEDouble c; c.val= _mm_max_pd(a.val,b.val);return c;}


        /*Masking Operations */

           friend inline int movemask( const SSEDouble &a) {return _mm_movemask_pd(a.val);}


        /*Store Operations*/

          friend inline void storel(double *p, const SSEDouble &a) { _mm_storel_pd(p,a.val);}

          friend inline void storeh(double *p, const SSEDouble &a) { _mm_storeh_pd(p,a.val);}


       //   void display();

          /**
           * 	This routine is adapted from
           * 	https://github.com/stgatilov/recip_rsqrt_benchmark
           *
           * 	It uses a 5th-order polynomial to bring the error to 51.5 bits (~3e-16).
           *
           * 	The speedup compared to using sqrt+div varies by CPU architecture, but
           * 	is between 10% (Skylake) and 3x (Sandybridge) faster.
           */
          friend inline SSEDouble rsqrt(SSEDouble a) {
			__m128d one = _mm_set1_pd(1.0), 		c1 = _mm_set1_pd(1.0/2.0),
					c2 = _mm_set1_pd(3.0/8.0), 	c3 = _mm_set1_pd(15.0/48.0),
					c4 = _mm_set1_pd(105.0/384.0);
			__m128d x = _mm_cvtps_pd(_mm_rsqrt_ps(_mm_cvtpd_ps(a.val)));
			__m128d r = _mm_sub_pd(one, _mm_mul_pd(_mm_mul_pd(a.val, x), x));
			__m128d r2 = _mm_mul_pd(r, r);
			__m128d t1 = _mm_add_pd(_mm_mul_pd(c2, r), c1);
			__m128d t3 = _mm_add_pd(_mm_mul_pd(c4, r), c3);
			__m128d poly = _mm_add_pd(_mm_mul_pd(r2, t3), t1);
			__m128d res = _mm_add_pd(_mm_mul_pd(_mm_mul_pd(x, r), poly), x);
			SSEDouble c;
			c.val = res;
			return c;
          }


};


/*
void SSEDouble::display()
{

storel(z,val);
//_mm_storeh_pd(z,val);
cout<<*z;
}

int main()
{

  double i=1.0;
  double *p=&i;
// __m128d t1=_mm_setr_pd(3.0,0.0); __m128d t2 = _mm_setr_pd(5.0,0.0);

  SSEDouble d1(2.0),d2(4.0),d4(25.0);

  SSEDouble d3 = (25.0/5.0) + (d1 * d2) + d4 ;


  storel(p,d3);

  cout<<*p;
//      d3 = d1 ^ d2;

 // d4 = sqrt(d2);

 // __m128d t =  _mm_and_pd(t1,t2);

//  cout << movemask(d3);
   d3.display();

  //int i = movemask(d4);

  //cout<<i;

}

*/

#endif

#endif //__SSE_DOUBLE_H__
