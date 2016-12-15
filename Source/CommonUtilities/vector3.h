#pragma once
#define VECTOR_TEMPLATE_DECL template<typename TYPE>
#include <math.h>

namespace CU
{
	template<typename TYPE>
	class Vector4;

	template<typename TYPE>
	class Vector2;

	template<typename TYPE>
	class Vector3
	{
	public:
		// Construction
		Vector3(void);
		Vector3(const Vector3 &aVector3);
		explicit Vector3(const Vector2<TYPE> &aVector2, const TYPE aZ = 0);
		explicit Vector3(const Vector4<TYPE> &aVector4);
		template<typename U> explicit Vector3(const Vector3<U> &aVector);
		Vector3(const TYPE anX, const TYPE aY, const TYPE aZ);

		void Set(const TYPE aX, const TYPE aY, const TYPE aZ)
		{
			myX = aX; myY = aY; myZ = aZ;
		}

		// Arithmetic
		friend Vector3 operator +(Vector3 aLeft, const Vector3 &aRight)
		{
			return aLeft += aRight;
		}
		friend Vector3 operator -(Vector3 aLeft, const Vector3 &aRight)
		{
			return aLeft -= aRight;
		}
		friend Vector3 operator *(Vector3 aLeft, const TYPE aRight)
		{
			return aLeft *= aRight;
		}

		friend Vector3 operator *(const TYPE aRight, Vector3 aLeft)
		{
			return aLeft *= aRight;
		}
		friend Vector3 operator *(Vector3 aLeft, const Vector3 aRight)
		{
			aLeft.x *= aRight.x;
			aLeft.y *= aRight.y;
			aLeft.z *= aRight.z;
			return aLeft;
		}
		friend Vector3 operator /(Vector3 aLeft, const TYPE aRight)
		{
			return aLeft /= aRight;
		}
		friend Vector3 operator -(Vector3 aRight)
		{
			aRight *= -1;
			return aRight;
		}
		// Comparison
		friend bool operator ==(const Vector3 &aLeft, const Vector3 &aRight)
		{
			if ((aLeft.x == aRight.x) && (aLeft.y == aRight.y) && (aLeft.z == aRight.z))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		friend bool operator !=(const Vector3 &aLeft, const Vector3 &aRight)
		{
			if ((aLeft.x == aRight.x) && (aLeft.y == aRight.y) && (aLeft.z == aRight.z))
			{
				return false;
			}
			else
			{
				return true;
			}
		}


		// Assignment
		Vector3 &operator =(const Vector3 &aRight);
		Vector3 &operator =(const Vector4<TYPE> &aRight);

		// Combined
		Vector3 &operator +=(const Vector3 &aRight);
		Vector3 &operator -=(const Vector3 &aRight);
		Vector3 &operator *=(const TYPE aRight);
		Vector3 &operator /=(const TYPE aRight);

		// Info
		TYPE Length(void) const;
		TYPE Length2(void) const;
		TYPE Dot(const Vector3 &aRight) const;
		Vector3 Cross(const Vector3 &aRight) const;
		Vector3 GetNormalized(void) const;

		// Manipulation
		//
		Vector3 &Normalize(void);

		union
		{
			TYPE	myX;
			TYPE    x;
			TYPE    X;
			TYPE	u;
			TYPE	r;
		};
		union
		{
			TYPE	myY;
			TYPE    y;
			TYPE    Y;
			TYPE	v;
			TYPE	g;
		};
		union
		{
			TYPE	myZ;
			TYPE    z;
			TYPE    Z;
			TYPE	w;
			TYPE	b;
		};

		static TYPE Dot(const Vector3 &aLeft, const Vector3 &aRight);
		static Vector3 Cross(const Vector3 &aLeft, const Vector3 &aRight);
		static Vector3 Normalize(Vector3 aVector);
		Vector3 InterPolateTowards(Vector3 aVectorToInterPolateTowards, float aInterpolatingSpeed);

		static const Vector3	Zero,
			UnitX,
			UnitY,
			UnitZ,
			One;
	};

	using Vector3c = Vector3<char>;
	using Vector3uc = Vector3<unsigned char>;
	using Vector3i = Vector3<int>;
	using Vector3ui = Vector3<unsigned int>;
	using Vector3f = Vector3<float>;
	using Vector3d = Vector3<double>;

	using Point3c = Vector3<char>;
	using Point3i = Vector3<int>;
	using Point3ui = Vector3<unsigned int>;
	using Point3f = Vector3<float>;
	using Point3d = Vector3<double>;
	VECTOR_TEMPLATE_DECL
		using Point3 = Vector3 < TYPE >;

	template<typename TYPE> const Vector3<TYPE> Vector3<TYPE>::Zero(0, 0, 0);
	template<typename TYPE> const Vector3<TYPE> Vector3<TYPE>::UnitX(1, 0, 0);
	template<typename TYPE> const Vector3<TYPE> Vector3<TYPE>::UnitY(0, 1, 0);
	template<typename TYPE> const Vector3<TYPE> Vector3<TYPE>::UnitZ(0, 0, 1);
	template<typename TYPE> const Vector3<TYPE> Vector3<TYPE>::One(1, 1, 1);

	//#include "vector3_template.inl"

	// Construction
	VECTOR_TEMPLATE_DECL
		Vector3<TYPE>::Vector3(void)
	{
		myX = static_cast<TYPE>(0);
		myY = static_cast<TYPE>(0);
		myZ = static_cast<TYPE>(0);
	}
	VECTOR_TEMPLATE_DECL
	Vector3<TYPE>::Vector3(const Vector2<TYPE> &aVector2, const TYPE aZ)
	{
		myX = aVector2.x;
		myY = aVector2.x;
		myZ = aZ;
	}


	VECTOR_TEMPLATE_DECL
		Vector3<TYPE>::Vector3(const Vector3 &aVector3)
	{
		myX = aVector3.myX;
		myY = aVector3.myY;
		myZ = aVector3.myZ;
	}

	VECTOR_TEMPLATE_DECL
		Vector3<TYPE>::Vector3(const Vector4<TYPE> &aVector4)
	{
		myX = aVector4.myX;
		myY = aVector4.myY;
		myZ = aVector4.myZ;
	}


	template<typename TYPE>
	template<typename U>
	inline Vector3<TYPE>::Vector3(const Vector3<U> &aVector) : x(static_cast<TYPE>(aVector.x)), y(static_cast<TYPE>(aVector.y)), z(static_cast<TYPE>(aVector.z))
	{
	}
	VECTOR_TEMPLATE_DECL
		Vector3<TYPE>::Vector3(const TYPE aX, const TYPE aY, const TYPE aZ)
	{
		myX = aX;
		myY = aY;
		myZ = aZ;
	}


	// Assignment
	VECTOR_TEMPLATE_DECL
		Vector3<TYPE> &Vector3<TYPE>::operator =(const Vector3 &aRight)
	{
		myX = aRight.myX;
		myY = aRight.myY;
		myZ = aRight.myZ;
		return *this;
	}
	VECTOR_TEMPLATE_DECL
		Vector3<TYPE> &Vector3<TYPE>::operator =(const Vector4<TYPE> &aRight)
	{
		myX = aRight.myX;
		myY = aRight.myY;
		myZ = aRight.myZ;
		return *this;
	}

	// Combined
	VECTOR_TEMPLATE_DECL
		Vector3<TYPE> &Vector3<TYPE>::operator +=(const Vector3 &aRight)
	{
		myX += aRight.x;
		myY += aRight.y;
		myZ += aRight.z;
		return *this;
	}
	VECTOR_TEMPLATE_DECL
		Vector3<TYPE> &Vector3<TYPE>::operator -=(const Vector3 &aRight)
	{
		myX -= aRight.x;
		myY -= aRight.y;
		myZ -= aRight.z;
		return *this;
	}
	VECTOR_TEMPLATE_DECL
		Vector3<TYPE> &Vector3<TYPE>::operator *=(const TYPE aRight)
	{
		myX *= aRight;
		myY *= aRight;
		myZ *= aRight;
		return *this;
	}
	VECTOR_TEMPLATE_DECL
		Vector3<TYPE> &Vector3<TYPE>::operator /=(const TYPE aRight)
	{
		myX /= aRight;
		myY /= aRight;
		myZ /= aRight;
		return *this;
	}

	// Info
	VECTOR_TEMPLATE_DECL
		TYPE Vector3<TYPE>::Length(void) const
	{
		return sqrt((myX * myX) + (myY * myY) + (myZ * myZ));
	}
	VECTOR_TEMPLATE_DECL
		TYPE Vector3<TYPE>::Length2(void) const
	{
		return ((myX * myX) + (myY * myY) + (myZ * myZ));

	}
	VECTOR_TEMPLATE_DECL
		TYPE Vector3<TYPE>::Dot(const Vector3 &aRight) const
	{
		return ((myX * aRight.x) + (myY * aRight.y) + (myZ * aRight.z));
	}
	VECTOR_TEMPLATE_DECL
		Vector3<TYPE> Vector3<TYPE>::Cross(const Vector3 &aRight) const
	{
		return Vector3((myY * aRight.z) - (myZ * aRight.y),
			(myZ * aRight.x) - (myX * aRight.z),
			(myX * aRight.y) - (myY * aRight.x));
	}
	VECTOR_TEMPLATE_DECL
		Vector3<TYPE> Vector3<TYPE>::GetNormalized(void) const
	{
		return Vector3<TYPE>(myX / Length(), myY / Length(), myZ / Length());
	}

	// Manipulation
	VECTOR_TEMPLATE_DECL
		Vector3<TYPE> &Vector3<TYPE>::Normalize(void)
	{
		TYPE length = Length();

		myX /= length;
		myY /= length;
		myZ /= length;
		return *this;
	}

	//STATICS
	VECTOR_TEMPLATE_DECL
		TYPE Vector3<TYPE>::Dot(const Vector3 &aLeft, const Vector3 &aRight)
	{
		return ((aLeft.x * aRight.x) + (aLeft.y * aRight.y) + (aLeft.z * aRight.z));
	}
	VECTOR_TEMPLATE_DECL
		Vector3<TYPE> Vector3<TYPE>::Cross(const Vector3 &aLeft, const Vector3 &aRight)
	{
		return Vector3((aLeft.y * aRight.z) - (aLeft.z * aRight.y),
			(aLeft.z * aRight.x) - (aLeft.x * aRight.z),
			(aLeft.x * aRight.y) - (aLeft.y * aRight.x));
	}
	VECTOR_TEMPLATE_DECL
		Vector3<TYPE> Vector3<TYPE>::Normalize(Vector3 aVector)
	{
		TYPE length = aVector.Length();
		return Vector3<TYPE>(aVector.x / length, aVector.y / length, aVector.z / length);
	}
	VECTOR_TEMPLATE_DECL
		Vector3<TYPE> Vector3<TYPE>::InterPolateTowards(Vector3 aVectorToInterPolateTowards, float aInterpolatingSpeed)
	{
		myX = myX + aInterpolatingSpeed * (aVectorToInterPolateTowards.myX - myX);
		myY = myY + aInterpolatingSpeed * (aVectorToInterPolateTowards.myY - myY);
		myZ = myZ + aInterpolatingSpeed * (aVectorToInterPolateTowards.myZ - myZ);
		return *this;
	}
} 