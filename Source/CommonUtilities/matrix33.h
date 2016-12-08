#pragma once
#include "vector3.h"
#include "vector2.h"
#include <array>
#include <math.h>

namespace CU
{

	// Forward declarations
	//
	template<typename TYPE>
	class Matrix44;


	template<typename TYPE>
	class Matrix33
	{
	public:     // Constants to avoid using magic numbers
				//
		static const unsigned int   NumRows = 3,
			NumColumns = 3,
			NumElements = NumRows * NumColumns;

		// Initializes the matrix to the identity matrix
		//
		Matrix33(void)
		{
			m11 = static_cast<TYPE>(1); m12 = static_cast<TYPE>(0); m13 = static_cast<TYPE>(0);
			m21 = static_cast<TYPE>(0); m22 = static_cast<TYPE>(1); m23 = static_cast<TYPE>(0);
			m31 = static_cast<TYPE>(0); m32 = static_cast<TYPE>(0); m33 = static_cast<TYPE>(1);
		}

		// Initializes the matrix with the data from anOther
		//
		Matrix33(const Matrix33 &anOther)
		{
			m11 = anOther.m11; m12 = anOther.m12; m13 = anOther.m13;
			m21 = anOther.m21; m22 = anOther.m22; m23 = anOther.m23;
			m31 = anOther.m31; m32 = anOther.m32; m33 = anOther.m33;
		}

		// Initializes the matrix with the 3x3-part from anOther
		//
		Matrix33(const Matrix44<TYPE> &anOther)
		{
			m11 = anOther.m11; m12 = anOther.m12; m13 = anOther.m13;
			m21 = anOther.m21; m22 = anOther.m22; m23 = anOther.m23;
			m31 = anOther.m31; m32 = anOther.m32; m33 = anOther.m33;
		}

		// Initializes the matrix with the static_cast'ed data from anOther which uses another base type
		//
		template<typename U>
		Matrix33(const Matrix33<U> &anOther)
		{
			m11 = static_cast<TYPE>(anOther.m11); m12 = static_cast<TYPE>(anOther.m12); m13 = static_cast<TYPE>(anOther.m13);
			m21 = static_cast<TYPE>(anOther.m21); m22 = static_cast<TYPE>(anOther.m22); m23 = static_cast<TYPE>(anOther.m23);
			m31 = static_cast<TYPE>(anOther.m31); m32 = static_cast<TYPE>(anOther.m32); m33 = static_cast<TYPE>(anOther.m33);
		}
		// Initializes the matrix with the data from an array
		//
		// Explanation:
		// This allows the syntax: Matrix33 identity({1, 0, 0, 0, 1, 0, 0, 0, 1});
		//
		Matrix33(const TYPE(&anArray)[NumElements])
		{
			m11 = anArray[0]; m12 = anArray[1]; m13 = anArray[2];
			m21 = anArray[3]; m22 = anArray[4]; m23 = anArray[5];
			m31 = anArray[6]; m32 = anArray[7]; m33 = anArray[8];
		}

		// Initializes the matrix with plain values
		//
		Matrix33(
			const TYPE a11, const TYPE a12, const TYPE a13,
			const TYPE a21, const TYPE a22, const TYPE a23,
			const TYPE a31, const TYPE a32, const TYPE a33)
		{
			m11 = a11; m12 = a12; m13 = a13;
			m21 = a21; m22 = a22; m23 = a23;
			m31 = a31; m32 = a32; m33 = a33;
		}

		// Adds and assigns the values of aRight to this matrix
		//
		Matrix33 &operator +=(const Matrix33 &aRight)
		{
			m11 += aRight.m11; m12 += aRight.m12; m13 += aRight.m13;

			m21 += aRight.m21; m22 += aRight.m22; m23 += aRight.m23;

			m31 += aRight.m31; m32 += aRight.m32; m33 += aRight.m33;
			return *this;
		}

		// Subtract and assigns the values of aRight to this matrix
		//
		Matrix33 &operator -=(const Matrix33 &aRight)
		{
			m11 -= aRight.m11; m12 -= aRight.m12; m13 -= aRight.m13;
			m21 -= aRight.m21; m22 -= aRight.m22; m23 -= aRight.m23;
			m31 -= aRight.m31; m32 -= aRight.m32; m33 -= aRight.m33;
			return *this;
		}

		// Multiplies this matrix with aRight
		//
		Matrix33 &operator *=(const Matrix33 &aRight)
		{
			Matrix33 temp = *this;

			m11 = (temp.m11 * aRight.m11) + (temp.m12 * aRight.m21) + (temp.m13 * aRight.m31);
			m12 = (temp.m11 * aRight.m12) + (temp.m12 * aRight.m22) + (temp.m13 * aRight.m32);
			m13 = (temp.m11 * aRight.m13) + (temp.m12 * aRight.m23) + (temp.m13 * aRight.m33);


			m21 = (temp.m21 * aRight.m11) + (temp.m22 * aRight.m21) + (temp.m23 * aRight.m31);
			m22 = (temp.m21 * aRight.m12) + (temp.m22 * aRight.m22) + (temp.m23 * aRight.m32);
			m23 = (temp.m21 * aRight.m13) + (temp.m22 * aRight.m23) + (temp.m23 * aRight.m33);


			m31 = (temp.m31 * aRight.m11) + (temp.m32 * aRight.m21) + (temp.m33 * aRight.m31);
			m32 = (temp.m31 * aRight.m12) + (temp.m32 * aRight.m22) + (temp.m33 * aRight.m32);
			m33 = (temp.m31 * aRight.m13) + (temp.m32 * aRight.m23) + (temp.m33 * aRight.m33);


			return *this;

		}

		// Sets the values of this matrix to those of aRight
		//
		Matrix33 &operator =(const Matrix33 &aRight)
		{
			m11 = aRight.m11; m12 = aRight.m12; m13 = aRight.m13;
			m21 = aRight.m21; m22 = aRight.m22; m23 = aRight.m23;
			m31 = aRight.m31; m32 = aRight.m32; m33 = aRight.m33;
			return *this;
		}

		// Transposes the matrix and returns *this
		//
		Matrix33 &Transpose(void)
		{
			Matrix33 temp = *this;

			m12 = temp.m21;
			m13 = temp.m31;

			m21 = temp.m12;
			m23 = temp.m32;

			m31 = temp.m13;
			m32 = temp.m23;
			return *this;
		}

		// Creates a transformation matrix for rotating anAngle rad around the x-axis
		//
		static Matrix33 CreateRotateAroundX(const TYPE anAngle)
		{
			Matrix33 rotationX;

			rotationX.m11 = static_cast<TYPE>(1);
			rotationX.m12 = static_cast<TYPE>(0);
			rotationX.m13 = static_cast<TYPE>(0);

			rotationX.m21 = static_cast<TYPE>(0);
			rotationX.m22 = static_cast<TYPE>(cos(static_cast<double>(anAngle)));
			rotationX.m23 = static_cast<TYPE>(sin(static_cast<double>(anAngle)));

			rotationX.m31 = static_cast<TYPE>(0);
			rotationX.m32 = static_cast<TYPE>(-sin(static_cast<double>(anAngle)));
			rotationX.m33 = static_cast<TYPE>(cos(static_cast<double>(anAngle)));

			return rotationX;
		}

		// Creates a transformation matrix for rotating anAngle rad around the y-axis
		//
		static Matrix33 CreateRotateAroundY(const TYPE anAngle)
		{
			Matrix33 rotationY;

			rotationY.m11 = static_cast<TYPE>(cos(static_cast<double>(anAngle)));
			rotationY.m12 = static_cast<TYPE>(0);
			rotationY.m13 = static_cast<TYPE>(-sin(static_cast<double>(anAngle)));

			rotationY.m21 = static_cast<TYPE>(0);
			rotationY.m22 = static_cast<TYPE>(1);
			rotationY.m23 = static_cast<TYPE>(0);

			rotationY.m31 = static_cast<TYPE>(sin(static_cast<double>(anAngle)));
			rotationY.m32 = static_cast<TYPE>(0);
			rotationY.m33 = static_cast<TYPE>(cos(static_cast<double>(anAngle)));

			return rotationY;
		}

		// Creates a transformation matrix for rotating anAngle rad around the z-axis
		//
		static Matrix33 CreateRotateAroundZ(const TYPE anAngle)
		{
			Matrix33 rotationZ;

			rotationZ.m11 = static_cast<TYPE>(cos(static_cast<double>(anAngle)));
			rotationZ.m12 = static_cast<TYPE>(sin(static_cast<double>(anAngle)));
			rotationZ.m13 = static_cast<TYPE>(0);

			rotationZ.m21 = static_cast<TYPE>(-sin(static_cast<double>(anAngle)));
			rotationZ.m22 = static_cast<TYPE>(cos(static_cast<double>(anAngle)));
			rotationZ.m23 = static_cast<TYPE>(0);

			rotationZ.m31 = static_cast<TYPE>(0);
			rotationZ.m32 = static_cast<TYPE>(0);
			rotationZ.m33 = static_cast<TYPE>(1);

			return rotationZ;
		}

		Matrix33 Lerp(const Matrix33<TYPE> aInterpolateToMatrix, TYPE aInterpolatingSpeed)
		{
			m21 = m21 + aInterpolatingSpeed * (aInterpolateToMatrix.m21 - m21);
			m22 = m22 + aInterpolatingSpeed * (aInterpolateToMatrix.m22 - m22);
			m23 = m23 + aInterpolatingSpeed * (aInterpolateToMatrix.m23 - m23);

			m31 = m31 + aInterpolatingSpeed * (aInterpolateToMatrix.m31 - m31);
			m32 = m32 + aInterpolatingSpeed * (aInterpolateToMatrix.m32 - m32);
			m33 = m33 + aInterpolatingSpeed * (aInterpolateToMatrix.m33 - m33);

			myForwardVector.Normalize();
			myRightVector = myUpVector.Cross(myForwardVector).GetNormalized();
			myUpVector = myForwardVector.Cross(myRightVector).GetNormalized();

			return *this;
		}
		Matrix33 LookAt(CU::Vector3<TYPE> aPositionToLookFrom, CU::Vector3<TYPE> aPositionToLookAt)
		{
			CU::Vector3f lookAt = aPositionToLookAt;
			CU::Vector3f lookFrom = aPositionToLookFrom;
			CU::Vector3f objectUpVector = CU::Vector3f(0.0f, 1.0f, 0.0f);


			CU::Vector3f zAxis = lookAt - lookFrom;
			zAxis = zAxis.GetNormalized();
			CU::Vector3f xAxis = objectUpVector.Cross(zAxis);
			xAxis = xAxis.GetNormalized();
			CU::Vector3f yAxis = zAxis.Cross(xAxis);


			m11 = xAxis.x;
			m12 = xAxis.y;
			m13 = xAxis.z;

			m21 = yAxis.x;
			m22 = yAxis.y;
			m23 = yAxis.z;

			m31 = zAxis.x;
			m32 = zAxis.y;
			m33 = zAxis.z;

			return *this;
		}
		// Returns a transposed copy of aMatrix
		//
		static Matrix33 Transpose(Matrix33 aMatrix)
		{
			Matrix33 temp = aMatrix;

			temp.m21 = aMatrix.m12;
			temp.m31 = aMatrix.m13;

			temp.m12 = aMatrix.m21;
			temp.m32 = aMatrix.m23;

			temp.m13 = aMatrix.m31;
			temp.m23 = aMatrix.m32;
			return temp;
		}

		// Sets the position part of a 2D transformation matrix 
		// 

		void SetPosition(const Vector2<TYPE> &aPosition)
		{
			m31 = aPosition.x;
			m32 = aPosition.y;
		}

		// Gets the position part of a 2D transformation matrix 
		// 
		Vector2<TYPE> GetPosition(void) const
		{
			return Vector2<TYPE>(m31, m32);
		}

		// Rotates the upper left 2x2 part of the matrix by anAngle rad around the z-axis 
		// 
		Matrix33 &Rotate2D(const TYPE anAngle)
		{
			*(this) = CreateRotateAroundZ(anAngle) * *(this);
			return *this;

			/*Matrix33<TYPE> rotation = CreateRotateAroundZ(anAngle);

			*this = *this * rotation;
			return *this;*/
		}


		// Sets the upper left 2x2 part of the matrix to the rotation of anAngle rad. Will 
		// create the same values for the rotation-part as the CreateRotateAroundZ but 
		// leaves the other values untouched. 

		Matrix33 &Set2DRotation(const TYPE anAngle)
		{
			/*			Matrix33<TYPE> rotation = CreateRotateAroundZ(anAngle);
						m11 = rotation.m11;
						m12 = rotation.m12;

						m21 = rotation.m21;
						m22 = rotation.m22;

						return *this;
		*/
			Matrix33<TYPE> tempMatrix = *(this);
			tempMatrix.CreateRotateAroundZ(anAngle);
			tempMatrix.m31 = m31;
			tempMatrix.m32 = m32;
			return tempMatrix;
		}




		// The data of this matrix
		//
#pragma warning( disable : 4201 ) // NOTE: This works fine in VC2013 and GCC so I'm keeping it. //MAHAM
		union
		{
			struct
			{
				TYPE    m11, m12, m13,
					m21, m22, m23,
					m31, m32, m33;
			};
			struct
			{
				Vector3<TYPE> myRightVector;
				Vector3<TYPE> myUpVector;
				Vector3<TYPE> myForwardVector;
			};
#pragma warning( default : 4201 )

			std::array<TYPE, NumElements> myMatrix;
		};

		// Pre-created matrices
		//
		static const Matrix33   Identity,   // Identity matrix
			Zero;       // Matrix with all 0
	};


	// Alias for Matrix33<float>. Add more if needed.
	//
	using Matrix33f = Matrix33<float>;


	// Returns a new matrix which is the sum of aLeft and aRight
	//
	template<typename TYPE>
	Matrix33<TYPE> operator +(Matrix33<TYPE> aLeft, const Matrix33<TYPE> &aRight)
	{
		return aLeft += aRight;
	}


	// Returns a new matrix wich is the difference of aLeft and aRight
	//
	template<typename TYPE>
	Matrix33<TYPE> operator -(Matrix33<TYPE> aLeft, const Matrix33<TYPE> &aRight)
	{
		return aLeft -= aRight;
	}


	// Returns a new matrix which is the product of aLeft and aRight
	//
	template<typename TYPE>
	Matrix33<TYPE> operator *(Matrix33<TYPE> aLeftValue, const Matrix33<TYPE> &aRightValue)
	{
		return aLeftValue *= aRightValue;
	}


	// Returns a new vector which is the product of the vector aLeft and the matrix 
	// aRight. Make sure you convert aLeft to a Vector3 and set it's .z-value to 1 
	// 

	//template<typename TYPE>
	//Vector2<TYPE> operator *(Vector2<TYPE> aLeft, const Matrix33<TYPE> &aRight)
	//{
	//	Vector3<TYPE> temp(aLeft.x, aLeft.y, static_cast<TYPE>(0));
	//	temp = temp * aRight;
	//	return Vector2<TYPE>(temp.x, temp.y);
	//}
	template<typename TYPE>
	Vector2<TYPE> operator *(Vector2<TYPE> aLeftValue, const Matrix33<TYPE> &aRightValue)
	{
		Vector3<TYPE> tempVec(aLeftValue.x, aLeftValue.y, 1);
		Vector3<TYPE> returnVec;
		returnVec.x = aRightValue.m11 * tempVec.x
			+ aRightValue.m21 * tempVec.y
			+ aRightValue.m31 * tempVec.z;
		returnVec.y = aRightValue.m12 * tempVec.x
			+ aRightValue.m22 * tempVec.y
			+ aRightValue.m32 * tempVec.z;

		return Vector2<TYPE>(returnVec.x, returnVec.y);
	}

	// Returns a new matrix which is the product of the vector aLeft and the matrix aRight
	//
	template<typename TYPE>
	Vector3<TYPE> operator *(Vector3<TYPE> aLeft, const Matrix33<TYPE> &aRight)
	{
		Vector3<TYPE> temp;

		temp.x = (aLeft.x * aRight.m11) + (aLeft.y * aRight.m21) + (aLeft.z * aRight.m31);
		temp.y = (aLeft.x * aRight.m12) + (aLeft.y * aRight.m22) + (aLeft.z * aRight.m32);
		temp.z = (aLeft.x * aRight.m13) + (aLeft.y * aRight.m23) + (aLeft.z * aRight.m33);

		return temp;
	}


	// Compares aLeft and aRight componentwise
	//
	template<typename TYPE>
	bool operator ==(const Matrix33<TYPE> &aLeft, const Matrix33<TYPE> &aRight)
	{
		if (aLeft.m11 == aRight.m11 && aLeft.m12 == aRight.m12 && aLeft.m13 == aRight.m13 &&
			aLeft.m21 == aRight.m21 && aLeft.m22 == aRight.m22 && aLeft.m23 == aRight.m23 &&
			aLeft.m31 == aRight.m31 && aLeft.m32 == aRight.m32 && aLeft.m33 == aRight.m33)
		{
			return true;
		}
		return false;
	}


	// Returns the negated result of aLeft == aRight
	//
	template<typename TYPE>
	bool operator !=(const Matrix33<TYPE> &aLeft, const Matrix33<TYPE> &aRight)
	{
		return (!(aLeft == aRight));
	}

	template<typename TYPE> const Matrix33<TYPE> Matrix33<TYPE>::Zero = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	template<typename TYPE> const Matrix33<TYPE> Matrix33<TYPE>::Identity = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };

} 