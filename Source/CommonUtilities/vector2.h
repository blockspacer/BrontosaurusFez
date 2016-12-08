#pragma once
#define VECTOR_TEMPLATE_DECL template<typename TYPE>

namespace CU
{

	template<typename TYPE>
	class Vector2
	{
	public:
		// Construction
		Vector2(void);
		Vector2(const Vector2 &aVector2);
		template<typename U> explicit Vector2(const Vector2<U> &aVector2);
		Vector2(const TYPE anX, const TYPE aY);
		template<typename U> explicit Vector2(const U aX, const U aY);

		// Arithmetic
		friend Vector2 operator +(Vector2 aLeft, const Vector2 &aRight)
		{
			return aLeft += aRight;
		}
		friend Vector2 operator -(Vector2 aLeft, const Vector2 &aRight)
		{
			return aLeft -= aRight;
		}
		friend Vector2 operator *(Vector2 aLeft, const TYPE aRight)
		{
			return aLeft *= aRight;
		}
		friend Vector2 operator /(Vector2 aLeft, const TYPE aRight)
		{
			return aLeft /= aRight;
		}
		friend Vector2 operator -(Vector2 aRight)
		{
			aRight.x *= -1;
			aRight.y *= -1;
			return aRight;
		}

		// Comparison
		friend bool operator ==(const Vector2 &aLeft, const Vector2 &aRight)
		{
			if ((aLeft.x == aRight.x) && (aLeft.y == aRight.y))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		friend bool operator !=(const Vector2 &aLeft, const Vector2 &aRight)
		{
			if ((aLeft.x == aRight.x) && (aLeft.y == aRight.y))
			{
				return false;
			}
			else
			{
				return true;
			}
		}

		// Assignment
		Vector2 &operator =(const Vector2 &aRight);
		void Set(const TYPE aX, const TYPE aY);

		template<typename U>
		inline void Set(const U aX, const U aY);

		// Compound assignment
		Vector2 &operator +=(const Vector2 &aRight);
		Vector2 &operator -=(const Vector2 &aRight);
		Vector2 &operator *=(const TYPE aRight);
		Vector2 &operator /=(const TYPE aRight);

		// Info
		TYPE Length(void) const;
		TYPE Length2(void) const;
		TYPE Dot(const Vector2<TYPE> &aRight) const;
		//KROSSEN
		TYPE Cross(Vector2<TYPE> aVector) const;
		Vector2 GetNormalized(void) const;

		const char* c_str() const;

		// Manipulation
		Vector2 &Normalize(void);

		union
		{
			TYPE    x;
			TYPE	myX;
			TYPE	u;
		};
		union
		{
			TYPE    y;
			TYPE	myY;
			TYPE	v;
		};

		static TYPE Dot(const Vector2 &aFirstVector, const Vector2 &aSecondVector);
		static Vector2 Normalize(Vector2 aVector);

		static const Vector2	Zero;
		static const Vector2	UnitX;
		static const Vector2	UnitY;
		static const Vector2	One;
	};

	using Vector2c = Vector2<char>;
	using Vector2i = Vector2<int>;
	using Vector2ui = Vector2<unsigned int>;
	using Vector2f = Vector2<float>;
	using Vector2d = Vector2<double>;

	using Point2c = Vector2<char>;
	using Point2i = Vector2<int>;
	using Point2ui = Vector2<unsigned int>;
	using Point2f = Vector2<float>;
	using Point2d = Vector2<double>;

	VECTOR_TEMPLATE_DECL
		using Point2 = Vector2<TYPE>;

	template<typename TYPE> const Vector2<TYPE> Vector2<TYPE>::Zero(0, 0);
	template<typename TYPE> const Vector2<TYPE> Vector2<TYPE>::UnitX(1, 0);
	template<typename TYPE> const Vector2<TYPE> Vector2<TYPE>::UnitY(0, 1);
	template<typename TYPE> const Vector2<TYPE> Vector2<TYPE>::One(1, 1);

	//Constructors
	VECTOR_TEMPLATE_DECL
		Vector2<TYPE>::Vector2(void)
	{
		myX = static_cast<TYPE>(0);
		myY = static_cast<TYPE>(0);
	}
	VECTOR_TEMPLATE_DECL
		Vector2<TYPE>::Vector2(const Vector2 &aVector2)
	{
		myX = aVector2.myX;
		myY = aVector2.myY;
	}

	template<typename TYPE>
	template<typename U>
	inline Vector2<TYPE>::Vector2(const Vector2<U> &aVector) : x(static_cast<TYPE>(aVector.x)), y(static_cast<TYPE>(aVector.y))
	{
	}

	template<typename TYPE>
	template<typename U>
	inline Vector2<TYPE>::Vector2(const U aX, const U aY) : x(static_cast<TYPE>(aX)), y(static_cast<TYPE>(aY))
	{
	}

	VECTOR_TEMPLATE_DECL
		Vector2<TYPE>::Vector2(const TYPE aX, const TYPE aY)
	{
		myX = aX;
		myY = aY;
	}

	// Assignment
	VECTOR_TEMPLATE_DECL
		Vector2<TYPE>& Vector2<TYPE>::operator=(const Vector2 &aRight)
	{
		myX = aRight.myX;
		myY = aRight.myY;
		return *this;
	}

	template<typename TYPE>
	template<typename U>
		inline void Vector2<TYPE>::Set(const U aX, const U aY)
	{
		myX = static_cast<TYPE>(aX);
		myY = static_cast<TYPE>(aY);
	}

	VECTOR_TEMPLATE_DECL
	inline void Vector2<TYPE>::Set(const TYPE aX, const TYPE aY)
	{
		myX = aX;
		myY = aY;
	}

	// Compound assignment
	VECTOR_TEMPLATE_DECL
		Vector2<TYPE>& Vector2<TYPE>::operator +=(const Vector2 &aRight)
	{
		myX += aRight.x;
		myY += aRight.y;
		return *this;
	}
	VECTOR_TEMPLATE_DECL
		Vector2<TYPE>& Vector2<TYPE>::operator -=(const Vector2 &aRight)
	{
		myX -= aRight.myX;
		myY -= aRight.myY;
		return *this;
	}
	VECTOR_TEMPLATE_DECL
		Vector2<TYPE>& Vector2<TYPE>::operator *=(const TYPE aRight)
	{
		myX *= aRight;
		myY *= aRight;
		return *this;
	}
	VECTOR_TEMPLATE_DECL
		Vector2<TYPE>& Vector2<TYPE>::operator /=(const TYPE aRight)
	{
		myX /= aRight;
		myY /= aRight;
		return *this;
	}

	// Info
	//
	VECTOR_TEMPLATE_DECL
		TYPE  Vector2<TYPE>::Length(void) const
	{
		return sqrt(((myX * myX) + (myY * myY)));
	}
	VECTOR_TEMPLATE_DECL
		TYPE  Vector2<TYPE>::Length2(void) const
	{
		return ((myX * myX) + (myY * myY));
	}
	VECTOR_TEMPLATE_DECL
		TYPE  Vector2<TYPE>::Dot(const Vector2<TYPE> &aRight) const
	{
		return ((myX * aRight.x) + (myY * aRight.y));
	}
	VECTOR_TEMPLATE_DECL
		TYPE Vector2<TYPE>::Cross(Vector2<TYPE> aVector) const
	{
		return (x * aVector.y - y * aVector.x);
	}
	VECTOR_TEMPLATE_DECL
		Vector2<TYPE> Vector2<TYPE>::GetNormalized(void) const
	{
		return Vector2<TYPE>(myX / Length(), myY / Length());
	}

	// Manipulation
	VECTOR_TEMPLATE_DECL
		Vector2<TYPE>& Vector2<TYPE>::Normalize(void)
	{
		TYPE length = Length();
		myX /= length;
		myY /= length;
		return *this;
	}

	template<typename TYPE>
	inline const char* Vector2<TYPE>::c_str() const
	{
		static char locName[64];
		memset(locName, 0, 64);
		sprintf_s(locName, 64, "(%f, %f)", myX, myY);

		return locName;
	}

	//STATICS
	VECTOR_TEMPLATE_DECL
		TYPE Vector2<TYPE>::Dot(const Vector2 &aFirstVector, const Vector2 &aSecondVector)
	{
		return ((aFirstVector.myX * aSecondVector.myX) + (aFirstVector.myY * aSecondVector.myY));
	}
	VECTOR_TEMPLATE_DECL
		Vector2<TYPE> Vector2<TYPE>::Normalize(Vector2 aVector)
	{
		TYPE length = aVector.Length();
		return Vector2<TYPE>(aVector.x / length, aVector.y / length);
	}
}

#undef VECTOR_TEMPLATE_DECL