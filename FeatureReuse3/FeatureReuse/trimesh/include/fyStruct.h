/********************************************************************
	Copyright (C) Fanyou Technology Co.,Ltd.  2001 - All Rights Reserved.
*********************************************************************/

/********************************************************************
	created:	2005/02/02
	created:	2:2:2005   2:13
	filename: 	inc\FyStruct.h
	file path:	inc
	file base:	FyStruct
	file ext:	h
	author:		Lin Jinming	
	
	purpose:	Fanyou 3D Software math types and functions
*********************************************************************/

#ifndef __FY_STRUCT_H__
#define __FY_STRUCT_H__

//#include "inc/api/fyApi_defs.h"
#include <math.h>
#include <limits>
#pragma warning (disable : 4305)
#pragma warning (disable : 4244)

#define FY_FLT_MAX	(1.0e+20f)//FLT_MAX
#define FY_INT_MAX	2147483647 //INT_MAX

#ifdef __cplusplus

#ifndef __FYVEC2I_H__
#define __FYVEC2I_H__
#include <assert.h>

typedef struct fyVec2i 
{	
	int x;
	int y;
	fyVec2i():x(0),y(0){};
	fyVec2i(int X):x(X),y(X){};
	fyVec2i(long X):x(X),y(X){};
	fyVec2i(int X, int Y):x(X),y(Y){};
	fyVec2i(long X, long Y):x(X),y(Y){};
	fyVec2i(float X, float Y)				{ x = (int)(X); y = (int)(Y);}
	fyVec2i(const fyVec2i &v)				{ x = v.x; y = v.y; };
	
    // binary operators
	fyVec2i  operator+ (const fyVec2i& v)const	{ return fyVec2i(x+v.x, y+v.y); }
	fyVec2i  operator- (const fyVec2i& v)const	{ return fyVec2i(x-v.x, y-v.y); }
	fyVec2i  operator* (int s)	const			{ return fyVec2i(x*s, y*s);	}
	fyVec2i  operator/ (int s)	const			{ assert(s!=0); return fyVec2i(x/s, y/s); }
	fyVec2i  operator* (float s)const			{ return fyVec2i((int)(x*s), (int)(y*s)); }

    // assignment operators
	fyVec2i& operator*= (int s)					{ x *= s; y *= s; return *this;}
	fyVec2i& operator/= (int s)					{ assert(s!=0); x/=s; y/=s; return *this; }
	fyVec2i& operator+= (const fyVec2i &v)		{ x+=v.x; y+=v.y; return *this; }
	fyVec2i& operator-= (const fyVec2i &v)		{ x-=v.x; y-=v.y; return *this; }

	bool operator == (const fyVec2i& v)const	{ return x==v.x && y== v.y; }
	bool operator != (const fyVec2i& v)const	{ return x!=v.x || y!= v.y; }
	
	// unary operators
    fyVec2i operator + () const					{ return *this; };
    fyVec2i operator - () const					{ return fyVec2i(-x,-y); }		
	
	void   Zero()								{ x = y = 0; }
	void   AllOne()								{ x = y = 1; };
//	int  Length()								{ return (sqrt(x*x+y*y)); }
	int    LengthSq()							{ return x*x+y*y; }
	int    Dot (const fyVec2i & v1) const 		{ return x*v1.x + y*v1.y; }	

	bool operator < (const fyVec2i& v) const
	{
		if(x<v.x)	return true;
		if(x>v.x)	return false;
		return y<v.y;
	}
	bool operator > (const fyVec2i& v) const
	{
		if(x>v.x)	return true;
		if(x<v.x)	return false;
		return y>v.y;
	}

	friend void Swap(fyVec2i& a, fyVec2i& b)			{ fyVec2i c(a); a = b; b = c; }
	friend fyVec2i operator* (int s, const fyVec2i& v)	{ return v*s; }

	friend fyVec2i Min(const fyVec2i& a,const fyVec2i& b)
	{
		return fyVec2i(
			a.x < b.x ? a.x : b.x,
			a.y < b.y ? a.y : b.y);
	}
	friend fyVec2i Max(const fyVec2i& a, const fyVec2i& b)
	{
		return fyVec2i(
			a.x > b.x ? a.x : b.x,
			a.y > b.y ? a.y : b.y);
	}		
}fyVec2i, *LPfyVec2i;

#endif /*__FYVEC2I_H__*/


#ifndef __FYVEC2F_H__
#define __FYVEC2F_H__

typedef struct fyVec2f 
{
	float x;
	float y;	
/*	
	union
	{
		float x;
		float u;
	};
	union
	{
		float y;
		float v;
	};
//*/
	fyVec2f():x(0),y(0){};
	fyVec2f(float fx, float fy):x(fx),y(fy){};
	fyVec2f(const fyVec2f &v)				{ x = v.x; y = v.y; };
	fyVec2f(const float * pf)				{ assert(pf!=0); x = pf[0]; y = pf[1]; }
	
	// casting
    operator float* ()						{ return (float*) &x;}
    operator const float* () const			{ return (const float*) &x;}
	
    // binary operators
	fyVec2f  operator+ (const fyVec2f& v)const	{ return fyVec2f(x+v.x, y+v.y); }
	fyVec2f  operator- (const fyVec2f& v)const	{ return fyVec2f(x-v.x, y-v.y); }
	fyVec2f  operator* (float f)const		{ return fyVec2f(x*f, y*f);	}
	fyVec2f  operator/ (float f)const		{ float fInv = 1/f; return fyVec2f(x*fInv, y*fInv);}
	
    // assignment operators
	fyVec2f& operator+= (const fyVec2f &v)	{ x+=v.x; y+=v.y; return *this; }
	fyVec2f& operator-= (const fyVec2f &v)	{ x-=v.x; y-=v.y; return *this; }
	fyVec2f& operator*= (float s)					{ x *= s; y *= s; return *this;}
	fyVec2f& operator/= (float s)					{ assert(s!=0); x/=s; y/=s; return *this; }
	
	bool operator == (const fyVec2f& v)const{ return x==v.x && y== v.y; }
	bool operator != (const fyVec2f& v)const{ return x!=v.x || y!= v.y; }
	
	// unary operators
    fyVec2f operator + () const				{ return *this; };
    fyVec2f operator - () const				{ return fyVec2f(-x,-y); }	
	
	float& operator() (int i)				{ assert( i>=0 && i<2 ); return (float&) *(&x + i);}
	const float& operator() (int i) const	{ assert( i>=0 && i<2 ); return (float&) *(&x + i);}
	
	void   Zero()							{ x = y = 0; }
	void   AllOne()							{ x = y = 1; };
	float  Length()							{ return sqrtf(x*x+y*y); }
	float  LengthSq()						{ return x*x+y*y; };
	float  Dot( const fyVec2f & v1 ) const 	{ return x*v1.x + y*v1.y; }	
	fyVec2f& Normalize()
	{		
		float d = x*x + y*y;
		
		if(d < 0.00000001f)
		{
			d = 100000000.0f;
		}
		else{
			d = float(1.0/sqrt(d));
		}
		x *= d;
		y *= d;
		return *this;
	}	
	bool operator < (const fyVec2f& v) const
	{
#if 1		
		if((int)(x*1000)<(int)(v.x*1000)) return true;
		if((int)(x*1000)>(int)(v.x*1000)) return false;
		if((int)(y*1000)<(int)(v.y*1000)) return true;
		if((int)(y*1000)>(int)(v.y*1000)) return false;
		return false;		
#else
		if(y<v.y) return true;
		if(y>v.y) return false;
		if(x<v.x) return true;
		if(x>v.x) return false;
		return false;
#endif
		if(x<v.x)	return true;
		if(x>v.x)	return false;
		return y<v.y;
	}
	bool operator > (const fyVec2f& v) const
	{
#if 1		
		if((int)(x*1000)>(int)(v.x*1000)) return true;
		if((int)(x*1000)<(int)(v.x*1000)) return false;
		if((int)(y*1000)>(int)(v.y*1000)) return true;
		if((int)(y*1000)<(int)(v.y*1000)) return false;
		return false;		
#else
		if(x>v.x) return true;
		if(x<v.x) return false;
		return y>v.y;
#endif
		if(x>v.x)	return true;
		if(x<v.x)	return false;
		return y>v.y;
	}
	
	friend void Swap(fyVec2f& a, fyVec2f& b)		{ fyVec2f c(a); a = b; b = c; }
	friend fyVec2f operator* (float s, fyVec2f& v)	{ return v*s; };
	friend fyVec2f Min(const fyVec2f& a, const fyVec2f& b)
	{
		return fyVec2f(
			a.x < b.x ? a.x : b.x,
			a.y < b.y ? a.y : b.y);
	}
	friend fyVec2f Max(const fyVec2f& a, const fyVec2f& b)
	{
		return fyVec2f(
			a.x > b.x ? a.x : b.x,
			a.y > b.y ? a.y : b.y);
	}

	friend fyVec2f Lerp	( const fyVec2f &v1, const fyVec2f &v2,	float s )
	{
		return fyVec2f(
			v1.x + s * (v2.x - v1.x),
			v1.y + s * (v2.y - v1.y)
			);
	}
}fyVec2f, *LPfyVec2f;

#endif /*__FYVEC2F_H__*/


#ifndef __FYVEC3I_H__
#define __FYVEC3I_H__

typedef struct fyVec3i
{
	int x;
	int y;
	int z;
	
	fyVec3i():x(0),y(0),z(0){};
	fyVec3i(int fx, int fy, int fz):x(fx),y(fy),z(fz){};
	fyVec3i(const fyVec3i &v) { x = v.x; y = v.y; z = v.z; }
	
    // binary operators
	fyVec3i operator + (const fyVec3i& v) const	{ return fyVec3i(x + v.x, y + v.y, z + v.z); }
	fyVec3i operator - (const fyVec3i& v) const { return fyVec3i(x - v.x, y - v.y, z - v.z); }
	fyVec3i operator * (float K) const			{ return fyVec3i(x*K, y*K, z*K); }	
	fyVec3i operator / (float K) const			{ return fyVec3i(x/K, y/K, z/K); }	
	
	// assignment operators
	fyVec3i& operator+=(const fyVec3i& v)		{ x+=v.x;y+=v.y;z+=v.z; return *this;	}
	fyVec3i& operator-=(const fyVec3i& v)		{ x-=v.x;y-=v.y;z-=v.z; return *this;	}
	fyVec3i& operator*=(float f)				{ x*=f; y*=f; z*=f;return *this;		}
	fyVec3i& operator/=(float f)				{ assert(0!=f); float s=1.0f/f; return operator*=(s); }
	
	int& operator() (int i)						{ assert( i>=0 && i<3 ); return (int&) *(&x + i);}
	const int& operator() (int i) const			{ assert( i>=0 && i<3 ); return (int&) *(&x + i);}
	
	// unary operators
    fyVec3i operator + () const					{ return *this; }
    fyVec3i operator - () const					{ return fyVec3i(-x,-y,-z); }
	
	bool operator == (const fyVec3i& v) const	{ return ((x==v.x)&&(y==v.y)&&(z==v.z)); }
	bool operator != (const fyVec3i& v) const	{ return ((x!=v.x)||(y!=v.y)||(z!=v.z)); }
	fyVec3i operator * (const fyVec3i& v) const	{ return fyVec3i(y*v.z - v.y*z, z*v.x - v.z*x, x*v.y - v.x*y); }
	bool operator < (const fyVec3i v) const
	{
		if(z<v.z) return true;
		if(z>v.z) return false;
		if(x<v.x) return true;
		if(x>v.x) return false;
		if(y<v.y) return true;
		return false;
	}
	bool operator > (const fyVec3i v) const
	{
		if(z>v.z) return true;
		if(z<v.z) return false;
		if(x>v.x) return true;
		if(x<v.x) return false;
		return y>v.y;
	}

	float Length()								{ return (float)sqrt(float(x*x+y*y+z*z));}
	int   LengthSq()							{ return x*x+y*y+z*z;}
	void  Zero()								{ x = y = z = 0;  }
	void  AllOne()								{ x = y = z = 1;  }
	void  infinity()							{ x = y = z = FY_INT_MAX; }
	
	fyVec3i Cross(const fyVec3i& v)	const		{ return fyVec3i(y*v.z - v.y*z, z*v.x - v.z*x, x*v.y - v.x*y); }
	float   Dot  (const fyVec3i & v)	 const	{ return x*v.x + y*v.y + z*v.z; }	

	friend void    Swap ( fyVec3i& a, fyVec3i& b ) { fyVec3i c(a); a = b; b = c; }
	friend fyVec3i Cross( const fyVec3i& v1, const fyVec3i& v2 ) 
	{
		return fyVec3i(v1.y*v2.z - v2.y*v1.z, v1.z*v2.x - v2.z*v1.x, v1.x*v2.y - v2.x*v1.y); 
	}
	friend float   Dot  ( const fyVec3i& v1, const fyVec3i& v2 ) 
	{
		return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z; 
	}
	friend fyVec3i Min(const fyVec3i& a,const fyVec3i& b)
	{
		return fyVec3i(
			a.x < b.x ? a.x : b.x,
			a.y < b.y ? a.y : b.y,
			a.z < b.z ? a.z : b.z);
	}
	friend fyVec3i Max(const fyVec3i& a, const fyVec3i& b)
	{
		return fyVec3i(
			a.x > b.x ? a.x : b.x,
			a.y > b.y ? a.y : b.y,
			a.z > b.z ? a.z : b.z);
	}		
}fyVec3i, *LPfyVec3i;

#endif /*__FYVEC3L_H__*/

#ifndef __FYVEC3L_H__
#define __FYVEC3L_H__

typedef struct fyVec3l
{
	long x;
	long y;
	long z;
	
	fyVec3l():x(0),y(0),z(0){};
	fyVec3l(long fx, long fy, long fz):x(fx),y(fy),z(fz){};
	fyVec3l(const fyVec3l &v) { x = v.x; y = v.y; z = v.z; }
	
    // binary operators
	fyVec3l operator + (const fyVec3l& v) const	{ return fyVec3l(x + v.x, y + v.y, z + v.z); }
	fyVec3l operator - (const fyVec3l& v) const { return fyVec3l(x - v.x, y - v.y, z - v.z); }
//	fyVec3l operator * (float K) const			{ return fyVec3l(x*K, y*K, z*K); }	
//	fyVec3l operator / (float K) const			{ return fyVec3l(x/K, y/K, z/K); }	
	
	// assignment operators
	fyVec3l& operator+=(const fyVec3l& v)		{ x+=v.x;y+=v.y;z+=v.z; return *this;	}
	fyVec3l& operator-=(const fyVec3l& v)		{ x-=v.x;y-=v.y;z-=v.z; return *this;	}
//	fyVec3l& operator*=(float f)				{ x*=f; y*=f; z*=f;return *this;		}
//	fyVec3l& operator/=(float f)				{ assert(0!=f); float s=1.0f/f; return operator*=(s); }
	
	long& operator() (int i)						{ assert( i>=0 && i<3 ); return (long&) *(&x + i);}
	const long& operator() (int i) const			{ assert( i>=0 && i<3 ); return (long&) *(&x + i);}
	
	// unary operators
    fyVec3l operator + () const					{ return *this; }
    fyVec3l operator - () const					{ return fyVec3l(-x,-y,-z); }
	
	bool operator == (const fyVec3l& v) const	{ return ((x==v.x)&&(y==v.y)&&(z==v.z)); }
	bool operator != (const fyVec3l& v) const	{ return ((x!=v.x)||(y!=v.y)||(z!=v.z)); }
	fyVec3l operator * (const fyVec3l& v) const	{ return fyVec3l(y*v.z - v.y*z, z*v.x - v.z*x, x*v.y - v.x*y); }
	bool operator < (const fyVec3l v) const
	{
		if(z<v.z) return true;
		if(z>v.z) return false;
		if(x<v.x) return true;
		if(x>v.x) return false;
		if(y<v.y) return true;
		return false;
	}
	bool operator > (const fyVec3l v) const
	{
		if(z>v.z) return true;
		if(z<v.z) return false;
		if(x>v.x) return true;
		if(x<v.x) return false;
		return y>v.y;
	}
	
	float Length()								{ return (float)sqrt(float(x*x+y*y+z*z));}
	int   LengthSq()							{ return x*x+y*y+z*z;}
	void  Zero()								{ x = y = z = 0;  }
	void  AllOne()								{ x = y = z = 1;  }
	void  infinity()							{ x = y = z = FY_INT_MAX; }
	
	fyVec3l Cross(const fyVec3l& v)	const		{ return fyVec3l(y*v.z - v.y*z, z*v.x - v.z*x, x*v.y - v.x*y); }
	float   Dot  (const fyVec3l & v)	 const	{ return x*v.x + y*v.y + z*v.z; }	
	
	friend void    Swap ( fyVec3l& a, fyVec3l& b ) { fyVec3l c(a); a = b; b = c; }
	friend fyVec3l Cross( const fyVec3l& v1, const fyVec3l& v2 ) 
	{
		return fyVec3l(v1.y*v2.z - v2.y*v1.z, v1.z*v2.x - v2.z*v1.x, v1.x*v2.y - v2.x*v1.y); 
	}
	friend float   Dot  ( const fyVec3l& v1, const fyVec3l& v2 ) 
	{
		return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z; 
	}
	friend fyVec3l Min(const fyVec3l& a,const fyVec3l& b)
	{
		return fyVec3l(
			a.x < b.x ? a.x : b.x,
			a.y < b.y ? a.y : b.y,
			a.z < b.z ? a.z : b.z);
	}
	friend fyVec3l Max(const fyVec3l& a, const fyVec3l& b)
	{
		return fyVec3l(
			a.x > b.x ? a.x : b.x,
			a.y > b.y ? a.y : b.y,
			a.z > b.z ? a.z : b.z);
	}		
}fyVec3l;

#endif /*__FYVEC3L_H__*/

#ifndef __FYVEC3F_H__
#define __FYVEC3F_H__

typedef struct fyVec3f
{
	float x;
	float y;
	float z;

	fyVec3f():x(0),y(0),z(0){};
	fyVec3f(float fx, float fy, float fz):x(fx),y(fy),z(fz){};
	fyVec3f(const fyVec3f &v) { x = v.x; y = v.y; z = v.z; }
	fyVec3f(float *v)	{ x = v[0]; y = v[1]; z = v[2]; }
	
    // binary operators
	fyVec3f operator + (const fyVec3f& v) const	{ return fyVec3f(x + v.x, y + v.y, z + v.z); }
	fyVec3f operator - (const fyVec3f& v) const { return fyVec3f(x - v.x, y - v.y, z - v.z); }
	fyVec3f operator * (float K) const			{ return fyVec3f(x*K, y*K, z*K); }	
	fyVec3f operator / (float K) const			{ return fyVec3f(x/K, y/K, z/K); }	
	fyVec3f operator + (float K) const			{ return fyVec3f(x+K, y+K, z+K); }	
	fyVec3f operator - (float K) const			{ return fyVec3f(x-K, y-K, z-K); }	

	// assignment operators
	fyVec3f& operator+=(const fyVec3f& v)		{ x+=v.x;y+=v.y;z+=v.z; return *this;	}
	fyVec3f& operator-=(const fyVec3f& v)		{ x-=v.x;y-=v.y;z-=v.z; return *this;	}
	fyVec3f& operator+=(const float v)			{ x+=v;y+=v;z+=v; return *this;	}
	fyVec3f& operator-=(const float v)			{ x-=v;y-=v;z-=v; return *this;	}

	fyVec3f& operator*=(float f)				{ x*=f; y*=f; z*=f;		return *this;	}
	fyVec3f& operator/=(float f)				{ assert(0!=f); float s=1.0f/f; return operator*=(s); }

	float& operator() (int i)					{ assert( i>=0 && i<3 ); return (float&) *(&x + i);}
	const float& operator() (int i) const		{ assert( i>=0 && i<3 ); return (float&) *(&x + i);}
	float& operator[] (int i)					{ assert( i>=0 && i<3 ); return (float&) *(&x + i);}
	const float& operator[] (int i) const		{ assert( i>=0 && i<3 ); return (float&) *(&x + i);}

	// unary operators
    fyVec3f operator + () const					{ return *this; }
    fyVec3f operator - () const					{ return fyVec3f(-x,-y,-z); }

	bool operator == (const fyVec3f& v) const	{ return ((x==v.x)&&(y==v.y)&&(z==v.z)); }
	bool operator != (const fyVec3f& v) const	{ return ((x!=v.x)||(y!=v.y)||(z!=v.z)); }

	float Length()		{ return (float)sqrtf(x*x+y*y+z*z);}
	float LengthSq()	{ return x*x+y*y+z*z;}
	void  Zero()		{ x = y = z = 0;  }
	void  AllOne()		{ x = y = z = 1;  }
	void  Infinity()	{ x = y = z = FY_FLT_MAX; }
	bool  PerpendicularTo(fyVec3f &v)
	{
		bool rc = false;
		int i, j, k; 
		float a, b;
		k = 2;
		if ( fabs(v.y) > fabs(v.x) ) {
			if ( fabs(v.z) > fabs(v.y) ) {
				// |v.z| > |v.y| > |v.x|
				i = 2;
				j = 1;
				k = 0;
				a = v.z;
				b = -v.y;
			}
			else if ( fabs(v.z) >= fabs(v.x) ){
				// |v.y| >= |v.z| >= |v.x|
				i = 1;
				j = 2;
				k = 0;
				a = v.y;
				b = -v.z;
			}
			else {
				// |v.y| > |v.x| > |v.z|
				i = 1;
				j = 0;
				k = 2;
				a = v.y;
				b = -v.x;
			}
		}
		else if ( fabs(v.z) > fabs(v.x) ) {
			// |v.z| > |v.x| >= |v.y|
			i = 2;
			j = 0;
			k = 1;
			a = v.z;
			b = -v.x;
		}
		else if ( fabs(v.z) > fabs(v.y) ) {
			// |v.x| >= |v.z| > |v.y|
			i = 0;
			j = 2;
			k = 1;
			a = v.x;
			b = -v.z;
		}
		else {
			// |v.x| >= |v.y| >= |v.z|
			i = 0;
			j = 1;
			k = 2;
			a = v.x;
			b = -v.y;
		}
		float* this_v = &x;
		this_v[i] = b;
		this_v[j] = a;
		this_v[k] = 0.0;
		return (a != 0.0) ? true : false;
	}
	fyVec3f operator * (const fyVec3f& v) const	{ return fyVec3f(y*v.z - v.y*z, z*v.x - v.z*x, x*v.y - v.x*y); }//²æ³Ë
	fyVec3f Cross(const fyVec3f& v)		  const { return fyVec3f(y*v.z - v.y*z, z*v.x - v.z*x, x*v.y - v.x*y); }//²æ³Ë
	float   Dot  (const fyVec3f& v)	  const { return x*v.x + y*v.y + z*v.z; }//µã³Ë	
	void	 set_min(const fyVec3f& rhs)			{ if(rhs.x<x) x=rhs.x; if(rhs.y<y) y=rhs.y; if(rhs.z<z) z=rhs.z; }
	void	 set_max(const fyVec3f& rhs)			{ if(rhs.x>x) x=rhs.x; if(rhs.y>y) y=rhs.y; if(rhs.z>z) z=rhs.z; }

	fyVec3f& NormalVector(const fyVec3f& v1, const fyVec3f& v2, const fyVec3f& v3)
	{
		*this = (v2 - v1)*(v3 - v1);
		Normalize();
		return *this;
	}	
	
	fyVec3f& Normalize()
	{		
		float d = x*x + y*y + z*z;
		
		if(d < 0.00000001f)
		{
			d = 100000000.0f;
		}
		else{
			d = float(1.0/sqrt(d));
		}
		x *= d;
		y *= d;
		z *= d;
		return *this;
//		
//		float d = 0;
//		d = LengthSq() > 0.00000001f ? float(1.0/sqrt(d)) : 100000000.0f;		
//		return fyVec3f(x*d,y*d,z*d);

	}
	
	bool operator < (const fyVec3f v) const
	{
#if 1		
		if((int)(z*1000)<(int)(v.z*1000)) return true;
		if((int)(z*1000)>(int)(v.z*1000)) return false;
		if((int)(x*1000)<(int)(v.x*1000)) return true;
		if((int)(x*1000)>(int)(v.x*1000)) return false;
		if((int)(y*1000)<(int)(v.y*1000)) return true;
		if((int)(y*1000)>(int)(v.y*1000)) return false;
		return false;		
#else
		if(z<v.z) return true;
		if(z>v.z) return false;
		if(y<v.y) return true;
		if(y>v.y) return false;
		if(x<v.x) return true;
		if(x>v.x) return false;
		return false;
#endif
	}
	bool operator > (const fyVec3f v) const
	{
#if 1		
		if((int)(z*1000)>(int)(v.z*1000)) return true;
		if((int)(z*1000)<(int)(v.z*1000)) return false;
		if((int)(x*1000)>(int)(v.x*1000)) return true;
		if((int)(x*1000)<(int)(v.x*1000)) return false;
		if((int)(y*1000)>(int)(v.y*1000)) return true;
		if((int)(y*1000)<(int)(v.y*1000)) return false;
		return false;		
#else
		if(z>v.z) return true;
		if(z<v.z) return false;
		if(x>v.x) return true;
		if(x<v.x) return false;
		return y>v.y;
#endif
	}

	friend void    Swap ( fyVec3f& a, fyVec3f& b )				 { fyVec3f c(a); a = b; b = c; }
	friend fyVec3f Cross( const fyVec3f& v1, const fyVec3f& v2 ) 
	{ 
		return fyVec3f(v1.y*v2.z - v2.y*v1.z, v1.z*v2.x - v2.z*v1.x, v1.x*v2.y - v2.x*v1.y); 
	}
	friend float   Dot  ( const fyVec3f& v1, const fyVec3f& v2 ) 
	{ 
		return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z; 
	}

	friend fyVec3f Min(const fyVec3f& a,const fyVec3f& b)
	{
		return fyVec3f(
			a.x < b.x ? a.x : b.x,
			a.y < b.y ? a.y : b.y,
			a.z < b.z ? a.z : b.z);
	}
	friend fyVec3f Max(const fyVec3f& a, const fyVec3f& b)
	{
		return fyVec3f(
			a.x > b.x ? a.x : b.x,
			a.y > b.y ? a.y : b.y,
			a.z > b.z ? a.z : b.z);
	}
	
	friend fyVec3f Lerp	( const fyVec3f &v1, const fyVec3f &v2,	float s )
	{
		return fyVec3f(
			v1.x + s * (v2.x - v1.x),
			v1.y + s * (v2.y - v1.y),
			v1.z + s * (v2.z - v1.z)
			);
	}

}fyVec3f, *LPFyVec3f, fyVertex3f;

#endif /*__FYVEC3F_H__*/

#ifndef __FYTRI3D_H__
#define __FYTRI3D_H__

typedef struct fyTri3d
{
	int v[3];
	fyTri3d(){};
	fyTri3d(const fyTri3d& tT)
	{
		v[0] = tT.v[0];
		v[1] = tT.v[1];
		v[2] = tT.v[2];	
	}
	fyTri3d(int v0, int v1, int v2)
	{
		v[0] = v0;
		v[1] = v1;
		v[2] = v2;
	}
	
	void Set(int v0, int v1, int v2)
	{
		v[0] = v0;
		v[1] = v1;
		v[2] = v2;
	}

	fyTri3d operator + (int n) const			{ return fyTri3d(v[0]+n, v[1]+n, v[2]+n); }
	fyTri3d operator - (int n) const			{ return fyTri3d(v[0]-n, v[1]-n, v[2]-n); }
	fyTri3d& operator+=(int n)					{ v[0]+=n;v[1]+=n;v[2]+=n; return *this;  }
	fyTri3d& operator-=(int n)					{ v[0]-=n;v[1]-=n;v[2]-=n; return *this;  }
	bool operator == (const fyTri3d& tT) const	{ return ((v[0]==tT.v[0])&&(v[1]==tT.v[1])&&(v[2]==tT.v[2])); }
	bool operator != (const fyTri3d& tT) const	{ return ((v[0]!=tT.v[0])||(v[1]!=tT.v[1])||(v[2]!=tT.v[2])); }
	bool operator < (const fyTri3d& tT) const		
	{	
		if(v[0] < tT.v[0]) return true;
		if(v[0] > tT.v[0]) return false;
		if(v[1] < tT.v[1]) return true;
		if(v[1] > tT.v[1]) return false;
		if(v[2] < tT.v[2]) return true;
		if(v[2] > tT.v[2]) return false;
		return false;
	}

	
	int& operator() (int i)						{ assert( i>=0 && i<3 ); return (int&) v[i]; }
	const int& operator() (int i) const			{ assert( i>=0 && i<3 ); return (int&) v[i]; }

	int GetAt(const int i) 						{ return v[i]; }
	const int GetAt(const int i) const			{ return (const int)v[i]; }

	bool GetThirdVer(int nA, int nB, int &nC)
	{
		int i,j; 
		for (i=0; i<3; i++) 
			if (v[i]==nA) break; 
			if (i==3) return false; 
			for (j=0; j<3; j++) 
				if (v[j]==nB) break; 
				if (j==3) return false; 
				nC = v[3-i-j]; 
				return true; 
	}
	int* GetTriIdx()
	{
		return v;
	}

	void RevertNormal()
	{
		int tmp = v[0];
		v[0] = v[1];
		v[1] = tmp;
	}

	bool IsValid()
	{
		return ( v[0] != v[1] && v[1] != v[2] && v[2] != v[0]
				 && v[0] != -1 && v[1] != -1 && v[2] != -1);
	}

}fyTri3d, *LPfyTri3d;

#endif /*__FYTRI3D_H__*/

#ifndef __FYTRI4D_H__
#define __FYTRI4D_H__

typedef struct fyTri4d
{
	int v[4];
	fyTri4d(){};
	fyTri4d(const fyTri4d& tT)
	{
		v[0] = tT.v[0];
		v[1] = tT.v[1];
		v[2] = tT.v[2];	
		v[3] = tT.v[3];
	}
	
	fyTri4d(int v0, int v1, int v2, int v3)
	{
		v[0] = v0;
		v[1] = v1;
		v[2] = v2;
		v[3] = v3;
	}

	fyTri4d operator + (int n) const			{ return fyTri4d(v[0]+n, v[1]+n, v[2]+n, v[3]+n); }
	fyTri4d operator - (int n) const			{ return fyTri4d(v[0]-n, v[1]-n, v[2]-n, v[3]-n); }
	fyTri4d& operator+=(int n)					{ v[0]+=n;v[1]+=n;v[2]+=n;v[3]+=n; return *this;  }
	fyTri4d& operator-=(int n)					{ v[0]-=n;v[1]-=n;v[2]-=n;v[3]-=n; return *this;  }
	bool operator == (const fyTri4d& tT) const	{ return ((v[0]==tT.v[0])&&(v[1]==tT.v[1])&&(v[2]==tT.v[2])&&(v[3]==tT.v[3])); }
	bool operator != (const fyTri4d& tT) const	{ return ((v[0]!=tT.v[0])||(v[1]!=tT.v[1])||(v[2]!=tT.v[2])||(v[3]!=tT.v[3])); }

}fyTri4d, *LPfyTri4d;

#endif /*__FYTRI4D_H__*/

#ifndef __FYBOX2F_H__
#define __FYBOX2F_H__

typedef struct fyBox2f
{
	fyVec2f Min;
	fyVec2f Max;
	
	fyBox2f()									{ Clear();};
	fyBox2f(const fyVec2f& a, const fyVec2f& b)	{ Min = a; Max = b; }
	fyBox2f(const fyBox2f& b)					{ Min = b.Min; Max = b.Max; }
	
	bool operator == (const fyBox2f& b) const	{ return (Min==b.Min)&&(Max==b.Max); }
	bool operator != (const fyBox2f& b) const	{ return (Min!=b.Min)||(Max!=b.Max); }	
	fyBox2f operator + (const fyVec2f& v) 		{ return fyBox2f(Min+v, Max+v); }
	fyBox2f operator - (const fyVec2f& v)		{ return fyBox2f(Min-v, Max-v); }
	fyBox2f& operator+=(const fyVec2f& v)		{ Min+=v; Max+=v; return *this;  }
	fyBox2f& operator-=(const fyVec2f& v)		{ Min-=v; Max-=v; return *this;  }

#if 0
	fyBox2f operator + (const fyBox2f& b) 		
	{
		return fyBox2f(min_num(Min,b.Min), max_num(Max,b.Max)); 
	}
#endif
	fyBox2f& operator= (const fyBox2f& b)
	{
		if( this != &b )
		{
			Min = b.Min;
			Max = b.Max;
		}
		return *this;
	}
	
	void Clear()
	{
		Min.x = Min.y =  FY_FLT_MAX;
		Max.x = Max.y = -FY_FLT_MAX;
	}

	bool IsEmpty()			const	{ return Min > Max; }
	void Empty()					{ Clear(); }	
	void Zero()						{ Min.Zero(); Max.Zero(); }
	fyVec2f& GetMin()				{ return Min; }
	fyVec2f& GetMax()				{ return Max; }
	fyVec2f  GetMid()		const	{ return (Min+Max)*0.5f; }
	fyVec2f  corner(int i)	const
	{
		assert(i>=0 && i<4);
		return fyVec2f(
			(i&2) ? Max(0) : Min(0),
			(i&1) ? Max(1) : Min(1));
	}
	void Add(float x, float y)
	{
		if( x < Min.x )	Min.x = x;
		if( x > Max.x ) Max.x = x;
		if( y < Min.y ) Min.y = y;
		if( y > Max.y ) Max.y = y;
	}
	void Add(const fyVec2f& p)
	{
		Add(p.x, p.y);
	}

}fyBox2f, *LPBox2f;

#endif /*__FYBOX2F_H__*/

#ifndef __FYBOX3F_H__
#define __FYBOX3F_H__

typedef struct fyBox3f 
{
private:
	fyVec3f Min, Max;
	fyVec3f calcCenter(void) const
    { 
		return (Min+Max)*0.5f;
    }

public:
	fyBox3f()									{ Clear(); }
	fyBox3f(const fyVec3f& b_min, const fyVec3f& b_max)	{ Min = b_min; Max = b_max; }
	fyBox3f(const fyBox3f& b)					{ Min = b.Min; Max = b.Max; }	
	fyBox3f(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax)
	{
		Min.x = xmin;	Min.y = ymin;	Min.z = zmin;
		Max.x = xmax;	Max.y = ymax;	Max.z = zmax;
	}
	
	bool operator == (const fyBox3f& b) const	{ return (Min==b.Min)&&(Max==b.Max); }
	bool operator != (const fyBox3f& b) const	{ return (Min!=b.Min)||(Max!=b.Max); }	

	fyBox3f operator + (const fyVec3f& v) 		{ return fyBox3f(Min+v, Max+v); }
	fyBox3f operator - (const fyVec3f& v)		{ return fyBox3f(Min-v, Max-v); }
	fyBox3f& operator+=(const fyVec3f& v)		{ Min+=v; Max+=v; return *this;  }
	fyBox3f& operator-=(const fyVec3f& v)		{ Min-=v; Max-=v; return *this;  }
	
	fyBox3f& operator=(const fyBox3f& b)		{ Min = b.Min;	Max = b.Max; return *this; }

	void Zero()		{ Min.Zero(); Max.Zero(); }
	
	void Clear()
	{
		Min.x = Min.y = Min.z =  FY_FLT_MAX;
		Max.x = Max.y = Max.z = -FY_FLT_MAX;
	}

	bool IsEmpty() const
	{
		return Min.x > Max.x;
	}

	void SetMin(const fyVec3f& b)
	{
		Min = b;
	}

	void SetMax(const fyVec3f& b)
	{
		Max = b;
	}

	const fyVec3f& GetMin() const
	{
		return Min; 
	}

	const fyVec3f& GetMax() const
	{
		return Max;
	}

	fyVec3f GetMid() const
	{
		return calcCenter();
	}

	double GetDiagLen() const
	{
		return (Max-Min).Length(); 
	}
	fyVec3f GetSize() const
	{
		return (Max - Min);
	}

	void Add(float x, float y, float z)
	{
		if( x < Min.x )	Min.x = x;
		if( x > Max.x ) Max.x = x;
		if( y < Min.y ) Min.y = y;
		if( y > Max.y ) Max.y = y;
		if( z < Min.z ) Min.z = z;
		if( z > Max.z ) Max.z = z;
	}

	void Add(const float* a) 
	{
		Add(a[0], a[1], a[2]);
	}

	void Add(const double* a) 
	{
		Add(a[0], a[1], a[2]);
	}

	void Add(const fyVec3f& p)
	{
		Add(p.x, p.y, p.z);
	}

	void Add(const fyBox3f& b)
	{
		if (!b.IsEmpty()) 
		{
			Add(b.Min);
			Add(b.Max); 
		}
	}

	void Scale(float scale)
	{
		if (!IsEmpty()) 
		{
			fyVec3f mid = GetMid();
			Min = mid + (Min-mid)*scale;
			Max = mid + (Max-mid)*scale;
		}
	}
	
	fyVec3f corner(int i) const
	{
		return fyVec3f(
			(i&4) ? Max.x : Min.x,
			(i&2) ? Max.y : Min.y,
			(i&1) ? Max.z : Min.z);
	}
	
	bool Intersect(const fyBox3f &b) const
    {
		// find whether any of the faces separate the boxes
		if (Max.x < b.Min.x) return false;
		if (Max.y < b.Min.y) return false;
		if (Max.z < b.Min.z) return false;
		if (b.Max.x < Min.x) return false;
		if (b.Max.y < Min.y) return false;
		if (b.Max.z < Min.z) return false;
		return true;      
    }

	bool Contains (const fyVec3f& p) const
	{
		return ( Min.x <= p.x && Min.y <= p.y && Min.z <= p.z 
			     && Max.x >= p.x && Max.y >= p.y && Max.z >= p.z
				);

	}
	bool Encloses (const fyBox3f& b) const
	{
		return ( Min.x <= b.Min.x && Min.y <= b.Min.y &&	Min.z <= b.Min.z 
				 && Max.x >= b.Max.x && Max.y >= b.Max.y &&	Max.z >= b.Max.z
				);

	}
}fyBox3f, *LPfyBox3f;

#endif /*__FYBOX3F_H__*/

#else  /*__cplusplus*/

// #ifndef __FYVEC2I_H__
// #define __FYVEC2I_H__
// struct fyVec2i 
// {
// 	int x;
// 	int y;
// };		
// #endif /*__FYVEC2I_H__*/
// 
// #ifndef __FYVEC2F_H__
// #define __FYVEC2F_H__
// struct fyVec2f
// {
// 	float x;
// 	float y;
// };
// #endif /*__FYVEC2F_H__*/
// 
// 
// #ifndef __FYVEC3I_H__
// #define __FYVEC3I_H__
// struct fyVec3i
// {
// 	int x;
// 	int y;
// 	int z;
// };
// #endif /*__FYVEC3I_H__*/
// 
// #ifndef __FYVEC3F_H__
// #define __FYVEC3F_H__
// 
// struct fyVec3f
// {
// 	float x;
// 	float y;
// 	float z;
// };
// #endif /*__FYVEC3F_H__*/
// 
// #ifndef __FYTRI3D_H__
// #define __FYTRI3D_H__
// struct fyTri3d
// {
// 	int v[3];
// };
// 
// #endif /*__FYTRI3D_H__*/
// 
// #ifndef __FYTRI4D_H__
// #define __FYTRI4D_H__
// 
// struct fyTri4d
// {
// 	int v[4];
// };
// 
// #endif /*__FYTRI4D_H__*/
// 
// #ifndef __FYBOX2F_H__
// #define __FYBOX2F_H__
// struct fyBox2f 
// {
// 	fyVec2f Min;
// 	fyVec2f Max;
// }
// #endif /*__FYBOX2F_H__*/
// 
// #ifndef __FYBOX3F_H__
// #define __FYBOX3F_H__
// struct fyBox3f 
// {
// 	fyVec3f Min;
// 	fyVec3f Max;
// }
// #endif /*__FYBOX3F_H__*/


#endif /*__cplusplus*/


#endif /*__FY_STRUCT_H__*/
