/**
 *	\file
 */
 
 
#pragma once


#include <cstddef>
#include <functional>
#include <limits>
#include <stdexcept>
#include <type_traits>
#include <utility>


namespace Safe {


	/**
	 *	\cond
	 */


	[[noreturn]]
	inline void Raise () {
	
		throw std::overflow_error("Integer value out of range");
	
	}
	
	
	template <typename B, typename A, typename=void>
	class NoThrowConvertible : public std::false_type {	};
	
	
	template <typename B, typename A>
	class NoThrowConvertible<
		B,
		A,
		typename std::enable_if<
			((std::is_signed<A>::value==std::is_signed<B>::value) && (sizeof(B)>=sizeof(A))) ||
			(std::is_unsigned<A>::value && std::is_signed<B>::value && (sizeof(B)>sizeof(A)))
		>::type
	> : public std::true_type {	};
	
	
	//	-	A signed, B signed, B>=A
	//	-	A unsigned, B unsigned, B>=A
	//	-	A unsigned, B signed, B>A
	//
	//	Unconditionally true
	template <typename B, typename A>
	constexpr typename std::enable_if<
		NoThrowConvertible<B,A>::value,
		bool
	>::type InRange (A) noexcept {
	
		return true;
	
	}
	
	
	//	-	A signed, B signed, B<A
	//
	//	Checks min and max of B to ensure integer is
	//	bounded by both
	template <typename B, typename A>
	constexpr typename std::enable_if<
		std::is_signed<A>::value && std::is_signed<B>::value && (sizeof(B)<sizeof(A)),
		bool
	>::type InRange (A i) noexcept {
	
		return (i<=static_cast<A>(std::numeric_limits<B>::max())) && (i>=static_cast<A>(std::numeric_limits<B>::min()));
	
	}
	
	
	//	-	A unsigned, B unsigned, B<A
	//
	//	Checks max of B to ensure integer is bounded
	template <typename B, typename A>
	constexpr typename std::enable_if<
		std::is_unsigned<A>::value && std::is_unsigned<B>::value && (sizeof(B)<sizeof(A)),
		bool
	>::type InRange (A i) noexcept {
	
		return i<=static_cast<A>(std::numeric_limits<B>::max());
	
	}
	
	
	//	-	A signed, B unsigned
	//
	//	False when integer is negative, otherwise passes through to:
	//
	//	-	A unsigned, B unsigned
	template <typename B, typename A>
	constexpr typename std::enable_if<
		std::is_signed<A>::value && std::is_unsigned<B>::value,
		bool
	>::type InRange (A i) noexcept {
	
		return (i<0) ? false : InRange<B>(static_cast<typename std::make_unsigned<A>::type>(i));
	
	}
	
	
	//	-	A unsigned, B signed, B<=A
	//
	//	True if integer is less than max of B
	template <typename B, typename A>
	constexpr typename std::enable_if<
		std::is_unsigned<A>::value && std::is_signed<B>::value && (sizeof(B)<=sizeof(A)),
		bool
	>::type InRange (A i) noexcept {
	
		return i<=static_cast<A>(std::numeric_limits<B>::max());
	
	}
	
	
	template <typename T>
	constexpr typename std::enable_if<std::is_unsigned<T>::value,bool>::type IsNegative (T) noexcept {
	
		return false;
	
	}
	
	
	template <typename T>
	constexpr typename std::enable_if<std::is_signed<T>::value,bool>::type IsNegative (T i) noexcept {
	
		return i<0;
	
	}
	
	
	template <typename T, typename=void>
	class Arithmetic {
	
	
		private:
		
		
			static constexpr T max=std::numeric_limits<T>::max();
			
			
			static void division_check (T a, T b) {
			
				//	Integer division always makes numbers smaller, since
				//	there are no fractions
				//
				//	Therefore, the only dangerous condition is division
				//	by zero
				
				if (b==0) Raise();
			
			}
		
		
		public:
		
		
			static T Add (T a, T b) {
			
				//	If either value is zero, addition cannot
				//	overflow
				if (a==0) return b;
				if (b==0) return a;
				
				//	If the distance between the maximum value and
				//	a is less than b, then adding b to a will cross
				//	that distance and then some -- i.e. an overflow
				if ((max-a)<b) Raise();
				
				return a+b;
			
			}
			
			
			static T Subtract (T a, T b) {
			
				//	The only way subtraction can overflow
				//	is if what we're subtracting is larger than
				//	what is being subtracted from
				if (b>a) Raise();
				
				return a-b;
			
			}
			
			
			static T Multiply (T a, T b) {
			
				//	If either value is zero, the result is
				//	zero at once
				if ((a==0) || (b==0)) return 0;
				//	If either value is one, just return
				//	the other value
				if (a==1) return b;
				if (b==1) return a;
				
				//	Dividing the maximum value by a gives
				//	the number of times by which a must be
				//	multiplied to give the maximum value.
				//
				//	If this is less than b, then multiplication
				//	by b will result in a number much more
				//	than the maximum (i.e. an overflow).
				if ((max/a)<b) Raise();
				
				return a*b;
			
			}
			
			
			static T Divide (T a, T b) {
			
				division_check(a,b);
				
				return a/b;
			
			}
			
			
			constexpr static T Abs (T i) noexcept {
			
				//	Unsigned numbers already represent their
				//	own absolute value, since they cannot be
				//	negative
				return i;
			
			}
			
			
			static T Modulus (T a, T b) {
			
				division_check(a,b);
			
				return a%b;
			
			}
	
	
	};
	
	
	template <typename T>
	class Arithmetic<T,typename std::enable_if<std::is_signed<T>::value>::type> {
	
	
		private:
		
		
			static constexpr T min=std::numeric_limits<T>::min();
			static constexpr T max=std::numeric_limits<T>::max();
			
			
			enum class Sign {
			
				Negative,
				Positive,
				Neither
			
			};
			
			
			constexpr static Sign get_sign (T i) noexcept {
			
				return (i<0) ? Sign::Negative : ((i>0) ? Sign::Positive : Sign::Neither);
			
			}
			
			
			static std::pair<Sign,Sign> get_signs (T a, T b) noexcept {
			
				return std::pair<Sign,Sign>(get_sign(a),get_sign(b));
			
			}
			
			
			static void division_check (T a, T b) {
			
				if (
					//	Catch divide by zero
					(b==0) ||
					//	Dividing by negative one changes the sign of
					//	the first operand, which can cause an overflow
					//	if the smallest integer is the other operand
					//	(since the positive and negative spaces of
					//	two's complement numbers are asymmetric)
					((b==-1) && (a==min))
				) Raise();
			
			}
			
			
		public:
		
		
			static T Add (T a, T b) {
			
				//	If either operand is zero, addition cannot
				//	overflow
				if (a==0) return b;
				if (b==0) return a;
			
				//	Get signs of operands
				auto s=get_signs(a,b);
				
				//	If the signs are opposite, addition cannot
				//	overflow
				if (s.first!=s.second) return a+b;
				
				//	If both signs are positive, and a is less than
				//	b away from the maximum, then adding a to b will
				//	more than cross the gap between a and the maximum
				//	(i.e. the addition will overflow)
				if (s.first==Sign::Positive) {
				
					if ((max-a)<b) Raise();
				
				//	If both signs are negative, and subtracting a from
				//	the minimum (i.e. increasing the minimum by the
				//	absolute value of a) causes the minimum to exceed
				//	b, then adding b to a will cause a's value to decrease
				//	such that it goes beneath the minimum (i.e. the
				//	addition will overflow)
				} else if ((min-a)>b) {
				
					Raise();
					
				}
				
				return a+b;
			
			}
			
			
			static T Subtract (T a, T b) {
			
				//	If either operand is zero, subtraction cannot
				//	overflow
				if ((a==0) || (b==0)) return a-b;
				
				//	Get signs of operands
				auto s=get_signs(a,b);
				
				//	If the signs are the same, subtraction cannot overflow
				if (s.first==s.second) return a-b;
				
				//	If the second sign is negative, a-b is really
				//	equivalent to a+-b, so we use a test from the
				//	Add method
				if (s.second==Sign::Negative) {
				
					if ((max+b)<a) Raise();
				
				//	Otherwise the first sign is negative, which means
				//	that the second sign is positive, which means that
				//	a-b is really -b+a, so we use another check from
				//	the Add method
				//
				//	Since we know that the second sign is positive, we
				//	can use -b safely, due to the fact that all positive
				//	integers have a corresponding negative integer under
				//	two's complement
				} else if ((min-a)>-b) {
				
					Raise();
				
				}
				
				return a-b;
			
			}
			
			
			static T Abs (T i) {
			
				//	If the integer is zero or positive, we already
				//	have the absolute value
				if (i>=0) return i;
				
				//	Due to the unequal positive/negative range of
				//	two's complement numbers, the minimum value does
				//	not have a corresponding positive value, so this
				//	is the overflow condition
				if (i==min) Raise();
				
				//	Otherwise the value is negative and has a corresponding
				//	positive value, so we multiply it by -1 and return
				return -i;
			
			}
			
			
			static T Multiply (T a, T b) {
			
				//	If either value is zero, the result is
				//	zero at once
				if ((a==0) || (b==0)) return 0;
				//	If either value is one, just return
				//	the other value
				if (a==1) return b;
				if (b==1) return a;
				
				//	Perform identical check to the one used for
				//	unsigned, only using magnitudes to handle
				//	negative numbers
				if ((max/Abs(a))<Abs(b)) Raise();
				
				return a*b;
			
			}
			
			
			static T Divide (T a, T b) {
			
				division_check(a,b);
				
				return a/b;
			
			}
			
			
			static T Modulus (T a, T b) {
			
				division_check(a,b);
				
				return a%b;
			
			}
	
	
	};
	
	
	template <typename A, typename B>
	constexpr typename std::enable_if<std::is_signed<A>::value==std::is_signed<B>::value,bool>::type IsEqual (A a, B b) noexcept {
	
		return a==b;
	
	}
	
	
	template <typename A, typename B>
	constexpr typename std::enable_if<std::is_signed<A>::value!=std::is_signed<B>::value,bool>::type IsEqual (A a, B b) noexcept {
	
		return (IsNegative(a) || IsNegative(b)) ? false : (static_cast<typename std::make_unsigned<A>::type>(a)==static_cast<typename std::make_unsigned<B>::type>(b));
	
	}
	
	
	template <typename A, typename B>
	constexpr typename std::enable_if<std::is_signed<A>::value==std::is_signed<B>::value,int>::type Compare (A a, B b) noexcept {
	
		return (a>b) ? 1 : ((a==b) ? 0 : -1);
	
	}
	
	
	template <typename A, typename B>
	constexpr typename std::enable_if<std::is_signed<A>::value!=std::is_signed<B>::value,int>::type Compare (A a, B b) noexcept {
	
		return IsNegative(a) ? -1 : (IsNegative(b) ? 1 : Compare(
			static_cast<typename std::make_unsigned<A>::type>(a),
			static_cast<typename std::make_unsigned<B>::type>(b)
		));
	
	}

	
	/**
	 *	\endcond
	 */
	 
	
	/**
	 *	Safely casts from one integer type to another.
	 *
	 *	This overload activates when \em B can represent
	 *	every value of \em A, is a compile time constant
	 *	expression, and as such does not throw.
	 *
	 *	\tparam B
	 *		The type of integer to cast to.
	 *	\tparam A
	 *		The type of integer to cast from.
	 *
	 *	\param [in] from
	 *		An integer of type \em A.
	 *
	 *	\return
	 *		\em from represented as type \em B.
	 */
	template <typename B, typename A>
	constexpr typename std::enable_if<NoThrowConvertible<B,A>::value,B>::type Cast (A from) noexcept {
	
		return static_cast<B>(from);
	
	}
	
	
	/**
	 *	Safely casts from one integer type to another.
	 *
	 *	This overload activates when \em B cannot represent
	 *	every value of \em A, and as such may throw when
	 *	\em from is out of range of \em B.
	 *
	 *	\tparam B
	 *		The type of integer to cast to.
	 *	\tparam A
	 *		The type of integer to cast from.
	 *
	 *	\param [in] from
	 *		An integer of type \em A.
	 *
	 *	\return
	 *		\em from represented as type \em B.
	 */
	template <typename B, typename A>
	typename std::enable_if<!NoThrowConvertible<B,A>::value,B>::type Cast (A from) {
	
		if (!InRange<B>(from)) Raise();
		
		return static_cast<B>(from);
	
	}
	

	/**
	 *	An integer which may be converted, assigned to,
	 *	and constructed safely, in addition to providing
	 *	checked arithmetic.
	 *
	 *	\tparam IntegerType
	 *		The type of integer to wrap.
	 */
	template <typename IntegerType>
	class Integer {
	
	
		private:
		
		
			IntegerType i;
			
			
			typedef std::numeric_limits<IntegerType> limits;
			static constexpr IntegerType min=limits::min();
			static constexpr IntegerType max=limits::max();
			
			
		public:
		
		
			/**
			 *	The type of integer this safe integer wraps.
			 */
			typedef IntegerType Type;
			/**
			 *	The type of integer this safe integer wraps, converted
			 *	to signed.
			 */
			typedef typename std::make_signed<IntegerType>::type SignedType;
			/**
			 *	The type of integer this safe integer wraps, converted
			 *	to unsigned.
			 */
			typedef typename std::make_unsigned<IntegerType>::type UnsignedType;
			
			
			/**
			 *	\em true if \em IntegerType is signed, \em false
			 *	otherwise.
			 */
			static constexpr bool Signed=std::is_signed<IntegerType>::value;
			/**
			 *	\em true if \em IntegerType is unsigned, \em false
			 *	otherwise.
			 */
			static constexpr bool Unsigned=std::is_unsigned<IntegerType>::value;
			
			
			Integer (const Integer &) = default;
			Integer (Integer &&) = default;
			Integer & operator = (const Integer &) = default;
			Integer & operator = (Integer &&) = default;
		
		
			/**
			 *	Creates a safe integer by wrapping an integer.
			 *
			 *	\param [in] i
			 *		The integer to wrap.  Defaults to zero.
			 */
			constexpr Integer (IntegerType i=0) noexcept : i(i) {	}
			/**
			 *	Creates a safe integer by wrapping an integer of
			 *	another type.
			 *
			 *	\tparam T
			 *		The type of integer to wrap.
			 *
			 *	\param [in] i
			 *		The integer.
			 */
			template <typename T>
			Integer (T i) noexcept(noexcept(Cast<IntegerType>(i))) : i(Cast<IntegerType>(i)) {	}
			/**
			 *	Creates a safe integer from another safe integer.
			 *
			 *	\tparam T
			 *		The type of integer the other safe integer wraps.
			 *
			 *	\param [in] i
			 *		The other safe integer.
			 */
			template <typename T>
			Integer (Integer<T> i) noexcept(noexcept(Cast<IntegerType>(std::declval<T>()))) : i(Cast<IntegerType>(static_cast<T>(i))) {	}
			
			
			/**
			 *	Assigns an integer to this safe integer.
			 *
			 *	\param [in] i
			 *		The integer.
			 *
			 *	\return
			 *		A reference to this object.
			 */
			Integer & operator = (IntegerType i) noexcept {
			
				this->i=std::move(i);
				
				return *this;
			
			}
			
			
			/**
			 *	Safely assigns an integer of another type to this
			 *	safe integer.
			 *
			 *	\tparam T
			 *		The type of the integer to assign.
			 *
			 *	\param [in] i
			 *		The integer.
			 *
			 *	\return
			 *		A reference to this object.
			 */
			template <typename T>
			Integer & operator = (T i) noexcept(noexcept(Cast<IntegerType>(i))) {
			
				this->i=Cast<IntegerType>(i);
				
				return *this;
			
			}
			
			
			/**
			 *	Safely assigns a safe integer of another type to this
			 *	safe integer.
			 *
			 *	\tparam T
			 *		The type of integer wrapped by the safe integer
			 *		type which shall be assigned to this safe integer.
			 *
			 *	\param [in] i
			 *		The other safe integer.
			 *
			 *	\return
			 *		A reference to this object.
			 */
			template <typename T>
			Integer & operator = (Integer<T> i) noexcept(noexcept(Cast<IntegerType>(std::declval<T>()))) {
			
				this->i=Cast<IntegerType>(static_cast<T>(i));
				
				return *this;
			
			}
			
			
			/**
			 *	Retrieves the integer this safe integer represents.
			 *
			 *	\return
			 *		The integer.
			 */
			explicit constexpr operator IntegerType () const noexcept {
			
				return i;
			
			}
			
			
			/**
			 *	Retrieves the integer this safe integer represents
			 *	safely converted to an integer of another type.
			 *
			 *	\tparam T
			 *		The other type.
			 *
			 *	\return
			 *		The integer.
			 */
			template <typename T>
			explicit operator T () const noexcept(noexcept(Cast<T>(std::declval<IntegerType>()))) {
			
				return Cast<T>(i);
			
			}
			
			
			/**
			 *	Retrieves the integer this safe integer represents.
			 *
			 *	\return
			 *		The integer.
			 */
			constexpr IntegerType Get () const noexcept {
			
				return i;
			
			}
			
			
			/**
			 *	Retrieves the integer this safe integer represents
			 *	safely converted to an integer of another type.
			 *
			 *	\tparam T
			 *		The other type.
			 *
			 *	\return
			 *		The integer.
			 */
			template <typename T>
			T Get () const noexcept(noexcept(Cast<T>(std::declval<IntegerType>()))) {
			
				return Cast<T>(i);
			
			}
			
			
			/**
			 *	Safely converts this safe integer to a safe integer of
			 *	signed type.
			 *
			 *	\return
			 *		A safe integer which wraps an integer type of
			 *		the same width as \em IntegerType, except signed,
			 *		and which contains the same value as this safe
			 *		integer.
			 */
			Integer<SignedType> MakeSigned () const noexcept(std::is_same<SignedType,IntegerType>::value) {
			
				return Integer<SignedType>(i);
			
			}
			
			
			/**
			 *	Safely converts this safe integer to a safe integer of
			 *	unsigned type.
			 *
			 *	\return
			 *		A safe integer which wraps an integer type of
			 *		the same width as \em IntegerType, except unsigned,
			 *		and which contains the same value as this safe
			 *		integer.
			 */
			Integer<UnsignedType> MakeUnsigned () const noexcept(std::is_same<UnsignedType,IntegerType>::value) {
			
				return Integer<UnsignedType>(i);
			
			}
			
			
			/**
			 *	Finds the absolute value (i.e. magnitude) of a safe integer.
			 *
			 *	\return
			 *		A safe integer containing the magnitude of this safe
			 *		integer.
			 */
			Integer Abs () const {
			
				return Arithmetic<IntegerType>::Abs(i);
			
			}
	
	
	};
	
	
	/**
	 *	Adds \em a and \em b, and assigns the result of the
	 *	addition to \em a.
	 *
	 *	Note that \em b is always converted to type \em A before
	 *	the operation takes place.
	 *
	 *	\tparam A
	 *		The integer type of \em a.
	 *	\tparam B
	 *		The type of \em b.
	 *
	 *	\param [in,out] a
	 *		The safe integer which is on the left hand side.
	 *	\param [in] b
	 *		The integer which is on the right hand side.
	 *
	 *	\return
	 *		A reference to \em a.
	 */
	template <typename A, typename B>
	Integer<A> & operator += (Integer<A> & a, B b) {
	
		return a=Arithmetic<A>::Add(a.Get(),Integer<A>(b).Get());
	
	}
	
	
	/**
	 *	Adds \em a and \em b, and assigns the result of the
	 *	addition to \em a.
	 *
	 *	Note that \em b is always converted to type \em A before
	 *	the operation takes place.
	 *
	 *	\tparam A
	 *		The integer type of \em a.
	 *	\tparam B
	 *		The integer type of \em b.
	 *
	 *	\param [in,out] a
	 *		The safe integer which is on the left hand side.
	 *	\param [in] b
	 *		The safe integer which is on the right hand side.
	 *
	 *	\return
	 *		A reference to \em a.
	 */
	template <typename A, typename B>
	Integer<A> & operator += (Integer<A> & a, Integer<B> b) {
	
		return a+=b.Get();
	
	}
	
	
	/**
	 *	Adds \em a and \em b, and assigns the result of the
	 *	addition to \em a.
	 *
	 *	Note that \em b is always converted to type \em A before
	 *	the operation takes place.
	 *
	 *	\tparam A
	 *		The type of \em a.
	 *	\tparam B
	 *		The integer type of \em b.
	 *
	 *	\param [in,out] a
	 *		The integer which is on the left hand side.
	 *	\param [in] b
	 *		The safe integer which is on the right hand side.
	 *
	 *	\return
	 *		A reference to \em a.
	 */
	template <typename A, typename B>
	A & operator += (A & a, Integer<B> b) {
	
		return a=Arithmetic<A>::Add(a,b.template Get<A>());
	
	}
	
	
	/**
	 *	Adds \em a and \em b.
	 *
	 *	Note that \em b is always converted to type \em A before
	 *	the operation takes place.
	 *
	 *	\tparam A
	 *		The integer type of \em a.
	 *	\tparam B
	 *		The integer type of \em b.
	 *
	 *	\param [in] a
	 *		The safe integer which is on the left hand side.
	 *	\param [in] b
	 *		The safe integer which is on the right hand side.
	 *
	 *	\return
	 *		The result.
	 */
	template <typename A, typename B>
	Integer<A> operator + (Integer<A> a, Integer<B> b) {
	
		return a+=b;
	
	}
	
	
	/**
	 *	Adds \em a and \em b.
	 *
	 *	Note that \em b is always converted to type \em A before
	 *	the operation takes place.
	 *
	 *	\tparam A
	 *		The integer type of \em a.
	 *	\tparam B
	 *		The type of \em b.
	 *
	 *	\param [in] a
	 *		The safe integer which is on the left hand side.
	 *	\param [in] b
	 *		The integer which is on the right hand side.
	 *
	 *	\return
	 *		The result.
	 */
	template <typename A, typename B>
	Integer<A> operator + (Integer<A> a, B b) {
	
		return a+=b;
	
	}
	
	
	/**
	 *	Adds \em a and \em b.
	 *
	 *	Note that \em b is always converted to type \em A before
	 *	the operation takes place.
	 *
	 *	\tparam A
	 *		The type of \em a.
	 *	\tparam B
	 *		The integer type of \em b.
	 *
	 *	\param [in] a
	 *		The integer which is on the left hand side.
	 *	\param [in] b
	 *		The safe integer which is on the right hand side.
	 *
	 *	\return
	 *		The result.
	 */
	template <typename A, typename B>
	Integer<A> operator + (A a, Integer<B> b) {
	
		return a+=b;
	
	}
	
	
	/**
	 *	Increments a safe integer.
	 *
	 *	\tparam T
	 *		The integer type of the safe integer.
	 *
	 *	\param [in,out] i
	 *		The safe integer.
	 *
	 *	\return
	 *		The safe integer.
	 */
	template <typename T>
	Integer<T> & operator ++ (Integer<T> & i) {
	
		return i+=1;
	
	}
	
	
	/**
	 *	Increments a safe integer.
	 *
	 *	\tparam T
	 *		The integer type of the safe integer.
	 *
	 *	\param [in,out] i
	 *		The safe integer.
	 *
	 *	\return
	 *		A copy of the safe integer before it was
	 *		incremented.
	 */
	template <typename T>
	Integer<T> operator ++ (Integer<T> & i, int) {
	
		auto retr=i;
		
		++i;
		
		return retr;
	
	}
	
	
	/**
	 *	Subtracts \em a and \em b, and assigns the result of the
	 *	subtraction to \em a.
	 *
	 *	Note that \em b is always converted to type \em A before
	 *	the operation takes place.
	 *
	 *	\tparam A
	 *		The integer type of \em a.
	 *	\tparam B
	 *		The type of \em b.
	 *
	 *	\param [in,out] a
	 *		The safe integer which is on the left hand side.
	 *	\param [in] b
	 *		The integer which is on the right hand side.
	 *
	 *	\return
	 *		A reference to \em a.
	 */
	template <typename A, typename B>
	Integer<A> & operator -= (Integer<A> & a, B b) {
	
		return a=Arithmetic<A>::Subtract(a.Get(),Integer<A>(b).Get());
	
	}
	
	
	/**
	 *	Subtracts \em a and \em b, and assigns the result of the
	 *	subtraction to \em a.
	 *
	 *	Note that \em b is always converted to type \em A before
	 *	the operation takes place.
	 *
	 *	\tparam A
	 *		The integer type of \em a.
	 *	\tparam B
	 *		The integer type of \em b.
	 *
	 *	\param [in,out] a
	 *		The safe integer which is on the left hand side.
	 *	\param [in] b
	 *		The safe integer which is on the right hand side.
	 *
	 *	\return
	 *		A reference to \em a.
	 */
	template <typename A, typename B>
	Integer<A> & operator -= (Integer<A> & a, Integer<B> b) {
	
		return a-=b.Get();
	
	}
	
	
	/**
	 *	Subtracts \em a and \em b, and assigns the result of the
	 *	subtraction to \em a.
	 *
	 *	Note that \em b is always converted to type \em A before
	 *	the operation takes place.
	 *
	 *	\tparam A
	 *		The type of \em a.
	 *	\tparam B
	 *		The integer type of \em b.
	 *
	 *	\param [in,out] a
	 *		The integer which is on the left hand side.
	 *	\param [in] b
	 *		The safe integer which is on the right hand side.
	 *
	 *	\return
	 *		A reference to \em a.
	 */
	template <typename A, typename B>
	A & operator -= (A & a, Integer<B> b) {
	
		return a=Arithmetic<A>::Subtract(a,b.template Get<A>());
	
	}
	
	
	/**
	 *	Subtracts \em a and \em b.
	 *
	 *	Note that \em b is always converted to type \em A before
	 *	the operation takes place.
	 *
	 *	\tparam A
	 *		The integer type of \em a.
	 *	\tparam B
	 *		The integer type of \em b.
	 *
	 *	\param [in] a
	 *		The safe integer which is on the left hand side.
	 *	\param [in] b
	 *		The safe integer which is on the right hand side.
	 *
	 *	\return
	 *		The result.
	 */
	template <typename A, typename B>
	Integer<A> operator - (Integer<A> a, Integer<B> b) {
	
		return a-=b;
	
	}
	
	
	/**
	 *	Subtracts \em a and \em b.
	 *
	 *	Note that \em b is always converted to type \em A before
	 *	the operation takes place.
	 *
	 *	\tparam A
	 *		The integer type of \em a.
	 *	\tparam B
	 *		The type of \em b.
	 *
	 *	\param [in] a
	 *		The safe integer which is on the left hand side.
	 *	\param [in] b
	 *		The integer which is on the right hand side.
	 *
	 *	\return
	 *		The result.
	 */
	template <typename A, typename B>
	Integer<A> operator - (Integer<A> a, B b) {
	
		return a-=b;
	
	}
	
	
	/**
	 *	Subtracts \em a and \em b.
	 *
	 *	Note that \em b is always converted to type \em A before
	 *	the operation takes place.
	 *
	 *	\tparam A
	 *		The type of \em a.
	 *	\tparam B
	 *		The integer type of \em b.
	 *
	 *	\param [in] a
	 *		The integer which is on the left hand side.
	 *	\param [in] b
	 *		The safe integer which is on the right hand side.
	 *
	 *	\return
	 *		The result.
	 */
	template <typename A, typename B>
	Integer<A> operator - (A a, Integer<B> b) {
	
		return a-=b;
	
	}
	
	
	/**
	 *	Decrements a safe integer.
	 *
	 *	\tparam T
	 *		The integer type of the safe integer.
	 *
	 *	\param [in,out] i
	 *		The safe integer.
	 *
	 *	\return
	 *		The safe integer.
	 */
	template <typename T>
	Integer<T> & operator -- (Integer<T> & i) {
	
		return i-=1;
	
	}
	
	
	/**
	 *	Decrements a safe integer.
	 *
	 *	\tparam T
	 *		The integer type of the safe integer.
	 *
	 *	\param [in,out] i
	 *		The safe integer.
	 *
	 *	\return
	 *		A copy of the safe integer before it was
	 *		decremented.
	 */
	template <typename T>
	Integer<T> operator -- (Integer<T> & i, int) {
	
		auto retr=i;
		
		--i;
		
		return retr;
	
	}
	
	
	/**
	 *	Multiplies \em a and \em b, and assigns the result of the
	 *	multiplication to \em a.
	 *
	 *	Note that \em b is always converted to type \em A before
	 *	the operation takes place.
	 *
	 *	\tparam A
	 *		The integer type of \em a.
	 *	\tparam B
	 *		The type of \em b.
	 *
	 *	\param [in,out] a
	 *		The safe integer which is on the left hand side.
	 *	\param [in] b
	 *		The integer which is on the right hand side.
	 *
	 *	\return
	 *		A reference to \em a.
	 */
	template <typename A, typename B>
	Integer<A> & operator *= (Integer<A> & a, B b) {
	
		return a=Arithmetic<A>::Multiply(a.Get(),Integer<A>(b).Get());
	
	}
	
	
	/**
	 *	Multiplies \em a and \em b, and assigns the result of the
	 *	multiplication to \em a.
	 *
	 *	Note that \em b is always converted to type \em A before
	 *	the operation takes place.
	 *
	 *	\tparam A
	 *		The integer type of \em a.
	 *	\tparam B
	 *		The integer type of \em b.
	 *
	 *	\param [in,out] a
	 *		The safe integer which is on the left hand side.
	 *	\param [in] b
	 *		The safe integer which is on the right hand side.
	 *
	 *	\return
	 *		A reference to \em a.
	 */
	template <typename A, typename B>
	Integer<A> & operator *= (Integer<A> & a, Integer<B> b) {
	
		return a*=b.Get();
	
	}
	
	
	/**
	 *	Multiplies \em a and \em b, and assigns the result of the
	 *	multiplication to \em a.
	 *
	 *	Note that \em b is always converted to type \em A before
	 *	the operation takes place.
	 *
	 *	\tparam A
	 *		The type of \em a.
	 *	\tparam B
	 *		The integer type of \em b.
	 *
	 *	\param [in,out] a
	 *		The integer which is on the left hand side.
	 *	\param [in] b
	 *		The safe integer which is on the right hand side.
	 *
	 *	\return
	 *		A reference to \em a.
	 */
	template <typename A, typename B>
	A & operator *= (A & a, Integer<B> b) {
	
		return a=Arithmetic<A>::Multiply(a,b.template Get<A>());
	
	}
	
	
	/**
	 *	Multiplies \em a and \em b.
	 *
	 *	Note that \em b is always converted to type \em A before
	 *	the operation takes place.
	 *
	 *	\tparam A
	 *		The integer type of \em a.
	 *	\tparam B
	 *		The integer type of \em b.
	 *
	 *	\param [in] a
	 *		The safe integer which is on the left hand side.
	 *	\param [in] b
	 *		The safe integer which is on the right hand side.
	 *
	 *	\return
	 *		The result.
	 */
	template <typename A, typename B>
	Integer<A> operator * (Integer<A> a, Integer<B> b) {
	
		return a*=b;
	
	}
	
	
	/**
	 *	Multiplies \em a and \em b.
	 *
	 *	Note that \em b is always converted to type \em A before
	 *	the operation takes place.
	 *
	 *	\tparam A
	 *		The integer type of \em a.
	 *	\tparam B
	 *		The type of \em b.
	 *
	 *	\param [in] a
	 *		The safe integer which is on the left hand side.
	 *	\param [in] b
	 *		The integer which is on the right hand side.
	 *
	 *	\return
	 *		The result.
	 */
	template <typename A, typename B>
	Integer<A> operator * (Integer<A> a, B b) {
	
		return a*=b;
	
	}
	
	
	/**
	 *	Multiplies \em a and \em b.
	 *
	 *	Note that \em b is always converted to type \em A before
	 *	the operation takes place.
	 *
	 *	\tparam A
	 *		The type of \em a.
	 *	\tparam B
	 *		The integer type of \em b.
	 *
	 *	\param [in] a
	 *		The integer which is on the left hand side.
	 *	\param [in] b
	 *		The safe integer which is on the right hand side.
	 *
	 *	\return
	 *		The result.
	 */
	template <typename A, typename B>
	Integer<A> operator * (A a, Integer<B> b) {
	
		return a*=b;
	
	}
	
	
	/**
	 *	Divides \em a by \em b, and assigns the result of the
	 *	division to \em a.
	 *
	 *	Note that \em b is always converted to type \em A before
	 *	the operation takes place.
	 *
	 *	\tparam A
	 *		The integer type of \em a.
	 *	\tparam B
	 *		The type of \em b.
	 *
	 *	\param [in,out] a
	 *		The safe integer which is on the left hand side.
	 *	\param [in] b
	 *		The integer which is on the right hand side.
	 *
	 *	\return
	 *		A reference to \em a.
	 */
	template <typename A, typename B>
	Integer<A> & operator /= (Integer<A> & a, B b) {
	
		return a=Arithmetic<A>::Divide(a.Get(),Integer<A>(b).Get());
	
	}
	
	
	/**
	 *	Divides \em a by \em b, and assigns the result of the
	 *	division to \em a.
	 *
	 *	Note that \em b is always converted to type \em A before
	 *	the operation takes place.
	 *
	 *	\tparam A
	 *		The integer type of \em a.
	 *	\tparam B
	 *		The integer type of \em b.
	 *
	 *	\param [in,out] a
	 *		The safe integer which is on the left hand side.
	 *	\param [in] b
	 *		The safe integer which is on the right hand side.
	 *
	 *	\return
	 *		A reference to \em a.
	 */
	template <typename A, typename B>
	Integer<A> & operator /= (Integer<A> & a, Integer<B> b) {
	
		return a/=b.Get();
	
	}
	
	
	/**
	 *	Divides \em a by \em b, and assigns the result of the
	 *	division to \em a.
	 *
	 *	Note that \em b is always converted to type \em A before
	 *	the operation takes place.
	 *
	 *	\tparam A
	 *		The type of \em a.
	 *	\tparam B
	 *		The integer type of \em b.
	 *
	 *	\param [in,out] a
	 *		The integer which is on the left hand side.
	 *	\param [in] b
	 *		The safe integer which is on the right hand side.
	 *
	 *	\return
	 *		A reference to \em a.
	 */
	template <typename A, typename B>
	A & operator /= (A & a, Integer<B> b) {
	
		return a=Arithmetic<A>::Divide(a,b.template Get<A>());
	
	}
	
	
	/**
	 *	Divides \em a by \em b.
	 *
	 *	Note that \em b is always converted to type \em A before
	 *	the operation takes place.
	 *
	 *	\tparam A
	 *		The integer type of \em a.
	 *	\tparam B
	 *		The integer type of \em b.
	 *
	 *	\param [in] a
	 *		The safe integer which is on the left hand side.
	 *	\param [in] b
	 *		The safe integer which is on the right hand side.
	 *
	 *	\return
	 *		The result.
	 */
	template <typename A, typename B>
	Integer<A> operator / (Integer<A> a, Integer<B> b) {
	
		return a/=b;
	
	}
	
	
	/**
	 *	Divides \em a by \em b.
	 *
	 *	Note that \em b is always converted to type \em A before
	 *	the operation takes place.
	 *
	 *	\tparam A
	 *		The integer type of \em a.
	 *	\tparam B
	 *		The type of \em b.
	 *
	 *	\param [in] a
	 *		The safe integer which is on the left hand side.
	 *	\param [in] b
	 *		The integer which is on the right hand side.
	 *
	 *	\return
	 *		The result.
	 */
	template <typename A, typename B>
	Integer<A> operator / (Integer<A> a, B b) {
	
		return a/=b;
	
	}
	
	
	/**
	 *	Divides \em a by \em b.
	 *
	 *	Note that \em b is always converted to type \em A before
	 *	the operation takes place.
	 *
	 *	\tparam A
	 *		The type of \em a.
	 *	\tparam B
	 *		The integer type of \em b.
	 *
	 *	\param [in] a
	 *		The integer which is on the left hand side.
	 *	\param [in] b
	 *		The safe integer which is on the right hand side.
	 *
	 *	\return
	 *		The result.
	 */
	template <typename A, typename B>
	Integer<A> operator / (A a, Integer<B> b) {
	
		return a/=b;
	
	}
	
	
	/**
	 *	Divides \em a by \em b, and assigns the remainder to \em a.
	 *
	 *	Note that \em b is always converted to type \em A before
	 *	the operation takes place.
	 *
	 *	\tparam A
	 *		The integer type of \em a.
	 *	\tparam B
	 *		The type of \em b.
	 *
	 *	\param [in,out] a
	 *		The safe integer which is on the left hand side.
	 *	\param [in] b
	 *		The integer which is on the right hand side.
	 *
	 *	\return
	 *		A reference to \em a.
	 */
	template <typename A, typename B>
	Integer<A> & operator %= (Integer<A> & a, B b) {
	
		return a=Arithmetic<A>::Modulus(a.Get(),Integer<A>(b).Get());
	
	}
	
	
	/**
	 *	Divides \em a by \em b, and assigns the remainder to \em a.
	 *
	 *	Note that \em b is always converted to type \em A before
	 *	the operation takes place.
	 *
	 *	\tparam A
	 *		The integer type of \em a.
	 *	\tparam B
	 *		The integer type of \em b.
	 *
	 *	\param [in,out] a
	 *		The safe integer which is on the left hand side.
	 *	\param [in] b
	 *		The safe integer which is on the right hand side.
	 *
	 *	\return
	 *		A reference to \em a.
	 */
	template <typename A, typename B>
	Integer<A> & operator %= (Integer<A> & a, Integer<B> b) {
	
		return a%=b.Get();
	
	}
	
	
	/**
	 *	Divides \em a by \em b, and assigns the remainder to \em a.
	 *
	 *	Note that \em b is always converted to type \em A before
	 *	the operation takes place.
	 *
	 *	\tparam A
	 *		The type of \em a.
	 *	\tparam B
	 *		The integer type of \em b.
	 *
	 *	\param [in,out] a
	 *		The integer which is on the left hand side.
	 *	\param [in] b
	 *		The safe integer which is on the right hand side.
	 *
	 *	\return
	 *		A reference to \em a.
	 */
	template <typename A, typename B>
	A & operator %= (A & a, Integer<B> b) {
	
		return a=Arithmetic<A>::Modulus(a,b.template Get<A>());
	
	}
	
	
	/**
	 *	Obtains the remainder of division of \em a by \em b.
	 *
	 *	Note that \em b is always converted to type \em A before
	 *	the operation takes place.
	 *
	 *	\tparam A
	 *		The integer type of \em a.
	 *	\tparam B
	 *		The integer type of \em b.
	 *
	 *	\param [in] a
	 *		The safe integer which is on the left hand side.
	 *	\param [in] b
	 *		The safe integer which is on the right hand side.
	 *
	 *	\return
	 *		The result.
	 */
	template <typename A, typename B>
	Integer<A> operator % (Integer<A> a, Integer<B> b) {
	
		return a%=b;
	
	}
	
	
	/**
	 *	Obtains the remainder of division of \em a by \em b.
	 *
	 *	Note that \em b is always converted to type \em A before
	 *	the operation takes place.
	 *
	 *	\tparam A
	 *		The integer type of \em a.
	 *	\tparam B
	 *		The type of \em b.
	 *
	 *	\param [in] a
	 *		The safe integer which is on the left hand side.
	 *	\param [in] b
	 *		The integer which is on the right hand side.
	 *
	 *	\return
	 *		The result.
	 */
	template <typename A, typename B>
	Integer<A> operator % (Integer<A> a, B b) {
	
		return a%=b;
	
	}
	
	
	/**
	 *	Obtains the remainder of division of \em a by \em b.
	 *
	 *	Note that \em b is always converted to type \em A before
	 *	the operation takes place.
	 *
	 *	\tparam A
	 *		The type of \em a.
	 *	\tparam B
	 *		The integer type of \em b.
	 *
	 *	\param [in] a
	 *		The integer which is on the left hand side.
	 *	\param [in] b
	 *		The safe integer which is on the right hand side.
	 *
	 *	\return
	 *		The result.
	 */
	template <typename A, typename B>
	Integer<A> operator % (A a, Integer<B> b) {
	
		return a%=b;
	
	}
	
	
	/**
	 *	Determines whether some safe integer \em a is equal to another
	 *	safe integer \em b.
	 *
	 *	Comparing integers of different width and signedness
	 *	using this operator results in well-defined and mathematically
	 *	correct results.
	 *
	 *	\tparam A
	 *		The integer type of \em b.
	 *	\tparam B
	 *		The integer type of \em b.
	 *
	 *	\param [in] a
	 *		The safe integer which is on the left hand side
	 *		of the comparison.
	 *	\param [in] b
	 *		The safe integer which is on the right hand side
	 *		of the comparison.
	 *
	 *	\return
	 *		\em true if \em a is equal to \em b, \em false otherwise. 
	 */
	template <typename A, typename B>
	constexpr bool operator == (Integer<A> a, Integer<B> b) noexcept {
	
		return IsEqual(a.Get(),b.Get());
	
	}
	
	
	/**
	 *	Determines whether some safe integer \em a is equal to
	 *	some integer \em b.
	 *
	 *	Comparing integers of different width and signedness
	 *	using this operator results in well-defined and mathematically
	 *	correct results.
	 *
	 *	\tparam A
	 *		The integer type of \em b.
	 *	\tparam B
	 *		The type of \em b.
	 *
	 *	\param [in] a
	 *		The safe integer which is on the left hand side of
	 *		the comparison.
	 *	\param [in] b
	 *		The integer which is on the right hand side of the
	 *		comparison.
	 *
	 *	\return
	 *		\em true if \em a is equal to \em b, \em false otherwise. 
	 */
	template <typename A, typename B>
	constexpr bool operator == (Integer<A> a, B b) noexcept {
	
		return IsEqual(a.Get(),b);
	
	}
	
	
	/**
	 *	Determines whether some integer \em a is equal to
	 *	some safe integer \em b.
	 *
	 *	Comparing integers of different width and signedness
	 *	using this operator results in well-defined and mathematically
	 *	correct results.
	 *
	 *	\tparam A
	 *		The type of \em a.
	 *	\tparam B
	 *		The integer type of \em b.
	 *
	 *	\param [in] a
	 *		The integer which is on the left hand side of
	 *		the comparison.
	 *	\param [in] b
	 *		The safe integer which is on the right hand side
	 *		of the comparison.
	 *
	 *	\return
	 *		\em true if \em a is equal to \em b, \em false otherwise. 
	 */
	template <typename A, typename B>
	constexpr bool operator == (A a, Integer<B> b) noexcept {
	
		return IsEqual(a,b.Get());
	
	}
	
	
	/**
	 *	Determines whether some safe integer \em a is not equal to another
	 *	safe integer \em b.
	 *
	 *	Comparing integers of different width and signedness
	 *	using this operator results in well-defined and mathematically
	 *	correct results.
	 *
	 *	\tparam A
	 *		The integer type of \em b.
	 *	\tparam B
	 *		The integer type of \em b.
	 *
	 *	\param [in] a
	 *		The safe integer which is on the left hand side
	 *		of the comparison.
	 *	\param [in] b
	 *		The safe integer which is on the right hand side
	 *		of the comparison.
	 *
	 *	\return
	 *		\em true if \em a is not equal to \em b, \em false otherwise. 
	 */
	template <typename A, typename B>
	constexpr bool operator != (Integer<A> a, Integer<B> b) noexcept {
	
		return !(a==b);
	
	}
	
	
	/**
	 *	Determines whether some safe integer \em a is not equal to
	 *	some integer \em b.
	 *
	 *	Comparing integers of different width and signedness
	 *	using this operator results in well-defined and mathematically
	 *	correct results.
	 *
	 *	\tparam A
	 *		The integer type of \em b.
	 *	\tparam B
	 *		The type of \em b.
	 *
	 *	\param [in] a
	 *		The safe integer which is on the left hand side of
	 *		the comparison.
	 *	\param [in] b
	 *		The integer which is on the right hand side of the
	 *		comparison.
	 *
	 *	\return
	 *		\em true if \em a is not equal to \em b, \em false otherwise. 
	 */
	template <typename A, typename B>
	constexpr bool operator != (Integer<A> a, B b) noexcept {
	
		return !(a==b);
	
	}
	
	
	/**
	 *	Determines whether some integer \em a is not equal to
	 *	some safe integer \em b.
	 *
	 *	Comparing integers of different width and signedness
	 *	using this operator results in well-defined and mathematically
	 *	correct results.
	 *
	 *	\tparam A
	 *		The type of \em a.
	 *	\tparam B
	 *		The integer type of \em b.
	 *
	 *	\param [in] a
	 *		The integer which is on the left hand side of
	 *		the comparison.
	 *	\param [in] b
	 *		The safe integer which is on the right hand side
	 *		of the comparison.
	 *
	 *	\return
	 *		\em true if \em a is not equal to \em b, \em false otherwise. 
	 */
	template <typename A, typename B>
	constexpr bool operator != (A a, Integer<B> b) noexcept {
	
		return !(a==b);
	
	}
	
	
	/**
	 *	Determines whether some safe integer \em a is greater than
	 *	another safe integer \em b.
	 *
	 *	Comparing integers of different width and signedness
	 *	using this operator results in well-defined and mathematically
	 *	correct results.
	 *
	 *	\tparam A
	 *		The integer type of \em b.
	 *	\tparam B
	 *		The integer type of \em b.
	 *
	 *	\param [in] a
	 *		The safe integer which is on the left hand side
	 *		of the comparison.
	 *	\param [in] b
	 *		The safe integer which is on the right hand side
	 *		of the comparison.
	 *
	 *	\return
	 *		\em true if \em a is greater than \em b, \em false otherwise. 
	 */
	template <typename A, typename B>
	constexpr bool operator > (Integer<A> a, Integer<B> b) noexcept {
	
		return Compare(a.Get(),b.Get())>0;
	
	}
	
	
	/**
	 *	Determines whether some safe integer \em a is greater than
	 *	some integer \em b.
	 *
	 *	Comparing integers of different width and signedness
	 *	using this operator results in well-defined and mathematically
	 *	correct results.
	 *
	 *	\tparam A
	 *		The integer type of \em b.
	 *	\tparam B
	 *		The type of \em b.
	 *
	 *	\param [in] a
	 *		The safe integer which is on the left hand side of
	 *		the comparison.
	 *	\param [in] b
	 *		The integer which is on the right hand side of the
	 *		comparison.
	 *
	 *	\return
	 *		\em true if \em a is greater than \em b, \em false otherwise. 
	 */
	template <typename A, typename B>
	constexpr bool operator > (Integer<A> a, B b) noexcept {
	
		return Compare(a.Get(),b)>0;
	
	}
	
	
	/**
	 *	Determines whether some integer \em a is greater than
	 *	some safe integer \em b.
	 *
	 *	Comparing integers of different width and signedness
	 *	using this operator results in well-defined and mathematically
	 *	correct results.
	 *
	 *	\tparam A
	 *		The type of \em a.
	 *	\tparam B
	 *		The integer type of \em b.
	 *
	 *	\param [in] a
	 *		The integer which is on the left hand side of
	 *		the comparison.
	 *	\param [in] b
	 *		The safe integer which is on the right hand side
	 *		of the comparison.
	 *
	 *	\return
	 *		\em true if \em a is greater than \em b, \em false otherwise. 
	 */
	template <typename A, typename B>
	constexpr bool operator > (A a, Integer<B> b) noexcept {
	
		return Compare(a,b.Get())>0;
	
	}
	
	
	/**
	 *	Determines whether some safe integer \em a is greater than
	 *	or equal to another safe integer \em b.
	 *
	 *	Comparing integers of different width and signedness
	 *	using this operator results in well-defined and mathematically
	 *	correct results.
	 *
	 *	\tparam A
	 *		The integer type of \em b.
	 *	\tparam B
	 *		The integer type of \em b.
	 *
	 *	\param [in] a
	 *		The safe integer which is on the left hand side
	 *		of the comparison.
	 *	\param [in] b
	 *		The safe integer which is on the right hand side
	 *		of the comparison.
	 *
	 *	\return
	 *		\em true if \em a is greater than or equal to \em b, \em false
	 *		otherwise. 
	 */
	template <typename A, typename B>
	constexpr bool operator >= (Integer<A> a, Integer<B> b) noexcept {
	
		return Compare(a.Get(),b.Get())>=0;
	
	}
	
	
	/**
	 *	Determines whether some safe integer \em a is greater than
	 *	or equal to some integer \em b.
	 *
	 *	Comparing integers of different width and signedness
	 *	using this operator results in well-defined and mathematically
	 *	correct results.
	 *
	 *	\tparam A
	 *		The integer type of \em b.
	 *	\tparam B
	 *		The type of \em b.
	 *
	 *	\param [in] a
	 *		The safe integer which is on the left hand side of
	 *		the comparison.
	 *	\param [in] b
	 *		The integer which is on the right hand side of the
	 *		comparison.
	 *
	 *	\return
	 *		\em true if \em a is greater than or equal to \em b, \em false
	 *		otherwise. 
	 */
	template <typename A, typename B>
	constexpr bool operator >= (Integer<A> a, B b) noexcept {
	
		return Compare(a.Get(),b)>=0;
	
	}
	
	
	/**
	 *	Determines whether some integer \em a is greater than
	 *	or equal to some safe integer \em b.
	 *
	 *	Comparing integers of different width and signedness
	 *	using this operator results in well-defined and mathematically
	 *	correct results.
	 *
	 *	\tparam A
	 *		The type of \em a.
	 *	\tparam B
	 *		The integer type of \em b.
	 *
	 *	\param [in] a
	 *		The integer which is on the left hand side of
	 *		the comparison.
	 *	\param [in] b
	 *		The safe integer which is on the right hand side
	 *		of the comparison.
	 *
	 *	\return
	 *		\em true if \em a is greater than or equal to \em b, \em false
	 *		otherwise. 
	 */
	template <typename A, typename B>
	constexpr bool operator >= (A a, Integer<B> b) noexcept {
	
		return Compare(a,b.Get())>=0;
	
	}
	
	
	/**
	 *	Determines whether some safe integer \em a is less than
	 *	another safe integer \em b.
	 *
	 *	Comparing integers of different width and signedness
	 *	using this operator results in well-defined and mathematically
	 *	correct results.
	 *
	 *	\tparam A
	 *		The integer type of \em b.
	 *	\tparam B
	 *		The integer type of \em b.
	 *
	 *	\param [in] a
	 *		The safe integer which is on the left hand side
	 *		of the comparison.
	 *	\param [in] b
	 *		The safe integer which is on the right hand side
	 *		of the comparison.
	 *
	 *	\return
	 *		\em true if \em a is less than \em b, \em false otherwise. 
	 */
	template <typename A, typename B>
	constexpr bool operator < (Integer<A> a, Integer<B> b) noexcept {
	
		return Compare(a.Get(),b.Get())<0;
	
	}
	
	
	/**
	 *	Determines whether some safe integer \em a is less than
	 *	some integer \em b.
	 *
	 *	Comparing integers of different width and signedness
	 *	using this operator results in well-defined and mathematically
	 *	correct results.
	 *
	 *	\tparam A
	 *		The integer type of \em b.
	 *	\tparam B
	 *		The type of \em b.
	 *
	 *	\param [in] a
	 *		The safe integer which is on the left hand side of
	 *		the comparison.
	 *	\param [in] b
	 *		The integer which is on the right hand side of the
	 *		comparison.
	 *
	 *	\return
	 *		\em true if \em a is less than \em b, \em false otherwise. 
	 */
	template <typename A, typename B>
	constexpr bool operator < (Integer<A> a, B b) noexcept {
	
		return Compare(a.Get(),b)<0;
	
	}
	
	
	/**
	 *	Determines whether some integer \em a is less than
	 *	some safe integer \em b.
	 *
	 *	Comparing integers of different width and signedness
	 *	using this operator results in well-defined and mathematically
	 *	correct results.
	 *
	 *	\tparam A
	 *		The type of \em a.
	 *	\tparam B
	 *		The integer type of \em b.
	 *
	 *	\param [in] a
	 *		The integer which is on the left hand side of
	 *		the comparison.
	 *	\param [in] b
	 *		The safe integer which is on the right hand side
	 *		of the comparison.
	 *
	 *	\return
	 *		\em true if \em a is less than \em b, \em false otherwise. 
	 */
	template <typename A, typename B>
	constexpr bool operator < (A a, Integer<B> b) noexcept {
	
		return Compare(a,b.Get())<0;
	
	}
	
	
	/**
	 *	Determines whether some safe integer \em a is less than
	 *	or equal to another safe integer \em b.
	 *
	 *	Comparing integers of different width and signedness
	 *	using this operator results in well-defined and mathematically
	 *	correct results.
	 *
	 *	\tparam A
	 *		The integer type of \em b.
	 *	\tparam B
	 *		The integer type of \em b.
	 *
	 *	\param [in] a
	 *		The safe integer which is on the left hand side
	 *		of the comparison.
	 *	\param [in] b
	 *		The safe integer which is on the right hand side
	 *		of the comparison.
	 *
	 *	\return
	 *		\em true if \em a is less than or equal to \em b, \em false
	 *		otherwise. 
	 */
	template <typename A, typename B>
	constexpr bool operator <= (Integer<A> a, Integer<B> b) noexcept {
	
		return Compare(a.Get(),b.Get())<=0;
	
	}
	
	
	/**
	 *	Determines whether some safe integer \em a is less than
	 *	or equal to some integer \em b.
	 *
	 *	Comparing integers of different width and signedness
	 *	using this operator results in well-defined and mathematically
	 *	correct results.
	 *
	 *	\tparam A
	 *		The integer type of \em b.
	 *	\tparam B
	 *		The type of \em b.
	 *
	 *	\param [in] a
	 *		The safe integer which is on the left hand side of
	 *		the comparison.
	 *	\param [in] b
	 *		The integer which is on the right hand side of the
	 *		comparison.
	 *
	 *	\return
	 *		\em true if \em a is less than or equal to \em b, \em false
	 *		otherwise. 
	 */
	template <typename A, typename B>
	constexpr bool operator <= (Integer<A> a, B b) noexcept {
	
		return Compare(a.Get(),b)<=0;
	
	}
	
	
	/**
	 *	Determines whether some integer \em a is less than
	 *	or equal to some safe integer \em b.
	 *
	 *	Comparing integers of different width and signedness
	 *	using this operator results in well-defined and mathematically
	 *	correct results.
	 *
	 *	\tparam A
	 *		The type of \em a.
	 *	\tparam B
	 *		The integer type of \em b.
	 *
	 *	\param [in] a
	 *		The integer which is on the left hand side of
	 *		the comparison.
	 *	\param [in] b
	 *		The safe integer which is on the right hand side
	 *		of the comparison.
	 *
	 *	\return
	 *		\em true if \em a is less than or equal to \em b, \em false
	 *		otherwise. 
	 */
	template <typename A, typename B>
	constexpr bool operator <= (A a, Integer<B> b) noexcept {
	
		return Compare(a,b.Get())<=0;
	
	}
	
	
	/**
	 *	Applies unary plus to a safe integer.
	 *
	 *	\tparam T
	 *		The integer type of the safe integer type.
	 *
	 *	\param [in] a
	 *		The safe integer.
	 *
	 *	\return
	 *		\em a.
	 */
	template <typename T>
	constexpr Integer<T> operator + (Integer<T> a) noexcept {
	
		return a;
	
	}
	
	
	/**
	 *	Applies unary minus to a safe integer.
	 *
	 *	This overload activates when the safe integer
	 *	type-in-question is unsigned.  In this case
	 *	an exception is thrown unless the safe
	 *	integer-in-question is exactly zero.
	 *
	 *	\tparam T
	 *		The integer type of the safe integer type.
	 *
	 *	\param [in] a
	 *		The safe integer.
	 *
	 *	\return
	 *		Zero.
	 */
	template <typename T>
	typename std::enable_if<Integer<T>::Unsigned,Integer<T>>::type operator - (Integer<T> a) {
	
		if (a==0) return a;
		
		Raise();
	
	}
	
	
	/**
	 *	Applies unary minus to a safe integer.
	 *
	 *	This overload activates when the safe integer
	 *	type-in-question is signed.
	 *
	 *	\tparam T
	 *		The integer type of the safe integer type.
	 *
	 *	\param [in] a
	 *		The safe integer.
	 *
	 *	\return
	 *		The result of multiplying \em a by negative
	 *		one.
	 */
	template <typename T>
	typename std::enable_if<Integer<T>::Signed,Integer<T>>::type operator - (Integer<T> a) {
	
		return a*-1;
	
	}
	
	
	/**
	 *	Creates a safe integer by wrapping an integer.
	 *
	 *	\tparam T
	 *		The type of integer to wrap.
	 *
	 *	\param [in] i
	 *		An integer of type \em T.
	 *
	 *	\return
	 *		A safe integer which wraps \em i.
	 */
	template <typename T>
	Integer<T> Make (T i) noexcept {
	
		return Integer<T>(i);
	
	}
	
	
	/**
	 *	A safe integer type which may be used to store and
	 *	perform arithmetic with sizes.
	 */
	typedef Integer<std::size_t> SizeType;
	/**
	 *	A safe integer type which may be used to store and
	 *	perform arithmetic with sizes.
	 */
	typedef Integer<std::size_t> size_t;
	/**
	 *	A safe integer type which may be used to perform and
	 *	store the results of pointer arithmetic.
	 */
	typedef Integer<std::ptrdiff_t> PointerDifferenceType;
	/**
	 *	A safe integer type which may be used to perform and
	 *	store the results of pointer arithmetic.
	 */
	typedef Integer<std::ptrdiff_t> SignedSizeType;
	/**
	 *	A safe integer type which may be used to perform and
	 *	store the results of pointer arithmetic.
	 */
	typedef Integer<std::ptrdiff_t> ptrdiff_t;
	/**
	 *	A safe integer type which may be used to perform and
	 *	store the results of pointer arithmetic.
	 */
	typedef Integer<std::ptrdiff_t> ssize_t;


}


/**
 *	\cond
 */


namespace std {


	template <typename T>
	struct hash<Safe::Integer<T>> {
	
	
		public:
		
		
			size_t operator () (const Safe::Integer<T> & i) const noexcept {
			
				return hash<T>{}(i.Get());
			
			}
	
	
	};
	
	
	template <typename T>
	class numeric_limits<Safe::Integer<T>> : public numeric_limits<T> {
	
	
		private:
		
		
			typedef Safe::Integer<T> type;
			typedef numeric_limits<T> base;
	
	
		public:
		
		
			static constexpr type min () noexcept {
			
				return base::min();
			
			}
			
			
			static constexpr type lowest () noexcept {
			
				return base::lowest();
			
			}
			
			
			static constexpr type max () noexcept {
			
				return base::max();
			
			}
			
			
			static constexpr type epsilon () noexcept {
			
				return base::epsilon();
			
			}
			
			
			static constexpr type round_error () noexcept {
			
				return base::round_error();
			
			}
			
			
			static constexpr type infinity () noexcept {
			
				return base::infinity();
			
			}
			
			
			static constexpr type quiet_NaN () noexcept {
			
				return base::quiet_NaN();
			
			}
			
			
			static constexpr type signaling_NaN () noexcept {
			
				return base::signaling_NaN();
			
			}
			
			
			static constexpr type denorm_min () noexcept {
			
				return base::denorm_min();
			
			}
	
	
	};
	
	
	template <typename T>
	struct is_signed<Safe::Integer<T>> : public is_signed<T> {	};
	
	
	template <typename T>
	struct is_unsigned<Safe::Integer<T>> : public is_unsigned<T> {	};
	
	
	template <typename T>
	struct make_signed<Safe::Integer<T>> {
	
	
		public:
		
		
			typedef Safe::Integer<typename make_signed<T>::type> type;
	
	
	};
	
	
	template <typename T>
	struct make_unsigned<Safe::Integer<T>> {
	
	
		public:
		
		
			typedef Safe::Integer<typename make_unsigned<T>::type> type;
	
	
	};


}


/**
 *	\endcond
 */
