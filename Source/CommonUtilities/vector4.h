#pragma once

#include "vector3.h"

#define VECTOR_TEMPLATE_DECL template<typename TYPE>

namespace CU
{
	// Forward declarations
	//
	template<typename TYPE>
	class Vector3;


	template<typename TYPE>
	class Vector2;

	template<typename TYPE>
	class Vector4
	{
	public:
		// Construction
		Vector4(void);
		Vector4(const Vector4 &anOther);

		explicit Vector4(const Vector3<TYPE> &anOther);
		explicit Vector4(const Vector2<TYPE> &anOther);
		explicit Vector4(const Vector2<TYPE> &aXY, const TYPE aZ, const TYPE aW);
		explicit Vector4(const Vector2<TYPE> &aXY, const Vector2<TYPE> &aZW);

		Vector4(const TYPE anX, const TYPE anY, const TYPE aZ, const TYPE aW);


		template<typename U> explicit Vector4(const Vector4<U> &anOther);


		// Arithmetic
		friend Vector4 operator +(Vector4 aLeft, const Vector4 &aRight)
		{
			return aLeft += aRight;
		}

		friend Vector4 operator +(Vector4 aLeft, const Vector3<TYPE> &aRight)
		{
			return aLeft += aRight;
		}



		friend Vector4 operator -(Vector4 aLeft, const Vector4 &aRight)
		{
			return aLeft -= aRight;
		}


		friend Vector4 operator *(Vector4 aLeft, const TYPE aRight)
		{
			return aLeft *= aRight;
		}


		friend Vector4 operator /(Vector4 aLeft, const TYPE aRight)
		{
			return aLeft /= aRight;
		}


		friend Vector4 operator -(Vector4 aRight)
		{
			aRight *= -1;
			return aRight;
		}


		// Comparison
		friend bool operator ==(const Vector4 &aLeft, const Vector4 &aRight)
		{
			if ((aLeft.x == aRight.x) && (aLeft.y == aRight.y) && (aLeft.z == aRight.z) && (aLeft.w == aRight.w))
			{
				return true;
			}
			else
			{
				return false;
			}
		}


		friend bool operator !=(const Vector4 &aLeft, const Vector4 &aRight)
		{
			if ((aLeft.x == aRight.x) && (aLeft.y == aRight.y) && (aLeft.z == aRight.z) && (aLeft.w == aRight.w))
			{
				return false;
			}
			else
			{
				return true;
			}
		}


		// Assignment
		Vector4 &operator =(const Vector4 &aRight);
		Vector4 &operator =(const Vector3<TYPE> &aRight);
		Vector4 &operator =(const Vector2<TYPE> &aRight);

		void Set(const TYPE aX, const TYPE aY, const TYPE aZ, const TYPE aW);

		// Combined
		Vector4 &operator +=(const Vector3<TYPE> &aRight);

		Vector4 &operator +=(const Vector4 &aRight);
		Vector4 &operator -=(const Vector4 &aRight);
		Vector4 &operator *=(const TYPE aRight);
		Vector4 &operator /=(const TYPE aRight);

		// Info
		TYPE Length(void) const;
		TYPE Length2(void) const;
		TYPE Dot(const Vector4 &aRight) const;
		Vector4 GetNormalized(void) const;

		// Manipulation
		Vector4 &Normalize(void);

		union
		{
			struct  
			{
				union
				{
					TYPE	myX;
					TYPE    x;
					TYPE	r;
					TYPE	u;
				};
				union
				{
					TYPE	myY;
					TYPE    y;
					TYPE	g;
					TYPE	v;
				};
				union
				{
					TYPE	myZ;
					TYPE    z;
					TYPE	b;
				};
				union
				{
					TYPE	myW;
					TYPE    w;
					TYPE	a;
				};
			};

			struct
			{
				CU::Vector3<TYPE> myVector3;
				TYPE myDontAsk;
			};
		};

		static Vector4 Normalize(Vector4 aVector);
		static TYPE Dot(const Vector4 &aFirstVector, const Vector4 &aSecondVector);

		void Print() const;

		static const Vector4	Zero,
			UnitX,
			UnitY,
			UnitZ,
			UnitW,
			One;
	};

	using Vector4c = Vector4<char>;
	using Vector4i = Vector4<int>;
	using Vector4ui = Vector4<unsigned int>;
	using Vector4f = Vector4<float>;
	using Vector4d = Vector4<double>;

	using Point4c = Vector4<char>;
	using Point4i = Vector4<int>;
	using Point4ui = Vector4<unsigned int>;
	using Point4f = Vector4<float>;
	using Point4d = Vector4<double>;

	template<typename TYPE> const Vector4<TYPE> Vector4<TYPE>::Zero(0, 0, 0, 0);
	template<typename TYPE> const Vector4<TYPE> Vector4<TYPE>::UnitX(1, 0, 0, 0);
	template<typename TYPE> const Vector4<TYPE> Vector4<TYPE>::UnitY(0, 1, 0, 0);
	template<typename TYPE> const Vector4<TYPE> Vector4<TYPE>::UnitZ(0, 0, 1, 0);
	template<typename TYPE> const Vector4<TYPE> Vector4<TYPE>::UnitW(0, 0, 0, 1);
	template<typename TYPE> const Vector4<TYPE> Vector4<TYPE>::One(1, 1, 1, 1);


	// Construction
	VECTOR_TEMPLATE_DECL
		Vector4<TYPE>::Vector4(void)
	{
		myX = static_cast<TYPE>(0);
		myY = static_cast<TYPE>(0);
		myZ = static_cast<TYPE>(0);
		myW = static_cast<TYPE>(0);
	}
	VECTOR_TEMPLATE_DECL
		Vector4<TYPE>::Vector4(const Vector4 &aVector4)
	{
		myX = aVector4.myX;
		myY = aVector4.myY;
		myZ = aVector4.myZ;
		myW = aVector4.myW;
	}

	VECTOR_TEMPLATE_DECL
		Vector4<TYPE>::Vector4(const Vector3<TYPE> &anOther)
	{
		myX = anOther.myX;
		myY = anOther.myY;
		myZ = anOther.myZ;
		myW = static_cast<TYPE>(1);
	}

	template<typename TYPE>
	template<typename U>
	inline Vector4<TYPE>::Vector4(const Vector4<U> &aVector) : x(static_cast<TYPE>(aVector.x)), y(static_cast<TYPE>(aVector.y)), z(static_cast<TYPE>(aVector.z)), w(static_cast<TYPE>(aVector.w))
	{
	}

	VECTOR_TEMPLATE_DECL
		Vector4<TYPE>::Vector4(const TYPE aX, const TYPE aY, const TYPE aZ, const TYPE aW)
	{
		myX = aX;
		myY = aY;
		myZ = aZ;
		myW = aW;
	}



	VECTOR_TEMPLATE_DECL
		Vector4<TYPE>::Vector4(const Vector2<TYPE> &aXY)
	{
		myX = aXY.x;
		myY = aXY.y;
		myZ = static_cast<TYPE>(0);
		myW = static_cast<TYPE>(1); // VENNE?! CARL?
	}





	VECTOR_TEMPLATE_DECL
		Vector4<TYPE>::Vector4(const Vector2<TYPE> &aXY, const TYPE aZ, const TYPE aW)
	{
		myX = aXY.x;
		myY = aXY.y;
		myZ = aZ;
		myW = aZ;
	}




	VECTOR_TEMPLATE_DECL
		Vector4<TYPE>::Vector4(const Vector2<TYPE> &aXY, const Vector2<TYPE> &aZW)
	{
		myX = aXY.x;
		myY = aXY.y;
		myZ = aZW.x;
		myW = aZW.y;
	}



	// Assignment
	VECTOR_TEMPLATE_DECL
		Vector4<TYPE> &Vector4<TYPE>::operator =(const Vector4 &aRight)
	{
		myX = aRight.myX;
		myY = aRight.myY;
		myZ = aRight.myZ;
		myW = aRight.w;
		return *this;
	}
	VECTOR_TEMPLATE_DECL
		Vector4 <TYPE> &Vector4<TYPE>::operator =(const Vector3<TYPE> &aRight)
	{
		myX = aRight.myX;
		myY = aRight.myY;
		myZ = aRight.myZ;
		myW = static_cast<TYPE>(0);
		return *this;
	}

	VECTOR_TEMPLATE_DECL
		Vector4 <TYPE> &Vector4<TYPE>::operator =(const Vector2<TYPE> &aRight)
	{
		myX = aRight.myX;
		myY = aRight.myY;
		myZ = static_cast<TYPE>(0);
		myW = static_cast<TYPE>(0);
		return *this;
	}

	VECTOR_TEMPLATE_DECL
	inline void Vector4<TYPE>::Set(const TYPE aX, const TYPE aY, const TYPE aZ, const TYPE aW)
	{
		myX = aX;
		myY = aY;
		myZ = aZ;
		myW = aW;
	}

	// Combined
	VECTOR_TEMPLATE_DECL
	Vector4<TYPE> &Vector4<TYPE>::operator +=(const Vector4 &aRight)
	{
		myX += aRight.x;
		myY += aRight.y;
		myZ += aRight.z;
		myW += aRight.w;
		return *this;
	}


	VECTOR_TEMPLATE_DECL
		Vector4<TYPE> &Vector4<TYPE>::operator +=(const Vector3<TYPE> &aRight)
	{
		myX += aRight.x;
		myY += aRight.y;
		myZ += aRight.z;
		return *this;
	}

	VECTOR_TEMPLATE_DECL
		Vector4<TYPE> &Vector4<TYPE>::operator -=(const Vector4 &aRight)
	{
		myX -= aRight.x;
		myY -= aRight.y;
		myZ -= aRight.z;
		myW -= aRight.w;
		return *this;
	}
	VECTOR_TEMPLATE_DECL
		Vector4<TYPE> &Vector4<TYPE>::operator *=(const TYPE aRight)
	{
		myX *= aRight;
		myY *= aRight;
		myZ *= aRight;
		myW *= aRight;
		return *this;
	}
	VECTOR_TEMPLATE_DECL
		Vector4<TYPE> &Vector4<TYPE>::operator /=(const TYPE aRight)
	{
		myX /= aRight;
		myY /= aRight;
		myZ /= aRight;
		myW /= aRight;
		return *this;
	}

	// Info
	VECTOR_TEMPLATE_DECL
		TYPE Vector4<TYPE>::Length(void) const
	{
		return sqrt(Length2());
	}
	VECTOR_TEMPLATE_DECL
		TYPE Vector4<TYPE>::Length2(void) const
	{
		return ((myX * myX) + (myY * myY) + (myZ * myZ) + (myW * myW));
	}
	VECTOR_TEMPLATE_DECL
		TYPE Vector4<TYPE>::Dot(const Vector4 &aRight) const
	{
		return ((myX * aRight.x) + (myY * aRight.y) + (myZ * aRight.z) + (myW * aRight.w));
	}
	VECTOR_TEMPLATE_DECL
		Vector4<TYPE> Vector4<TYPE>::GetNormalized(void) const
	{
		Vector4<TYPE> normalized = *this;
		return normalized.Normalize();
	}


	// Manipulation
	VECTOR_TEMPLATE_DECL
		Vector4<TYPE> &Vector4<TYPE>::Normalize(void)
	{
		TYPE length = Length();
		if (length > 0)
		{
			myX /= length;
			myY /= length;
			myZ /= length;
			myW /= length;
		}

		return *this;
	}

	//STATICS
	VECTOR_TEMPLATE_DECL
		Vector4<TYPE> Vector4<TYPE>::Normalize(Vector4 aVector)
	{
		TYPE length = aVector.Length();
		return Vector4<TYPE>(aVector.x / length, aVector.y / length, aVector.z / length, aVector.w / length);
	}
	VECTOR_TEMPLATE_DECL
		TYPE Vector4<TYPE>::Dot(const Vector4 &aFirstVector, const Vector4 &aSecondVector)
	{
		return ((aFirstVector.x * aSecondVector.x) +
			(aFirstVector.y * aSecondVector.y) +
			(aFirstVector.z * aSecondVector.z) +
			(aFirstVector.w * aSecondVector.w));
	}

	template<typename TYPE>
	inline void Vector4<TYPE>::Print() const
	{
#ifdef DL_PRINT
		DL_PRINT("(%f, %f, %f, %f)", x, y, z, w);
#endif // DL_PRINT
	}
}
