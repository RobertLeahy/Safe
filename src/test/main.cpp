#define CATCH_CONFIG_MAIN


#include <safe/safe.hpp>
#include <sstream>
#include <string>


namespace Catch {


	template <typename T>
	std::string toString (Safe::Integer<T> i) {
	
		std::ostringstream ss;
		ss << i.Get();
		
		return ss.str();
	
	}


}


#include <catch.hpp>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <type_traits>
#include <utility>


using Safe::Integer;


template <typename T, typename... Args>
T Construct (Args &&... args) noexcept(std::is_nothrow_constructible<T,Args...>::value) {

	return T(std::forward<Args>(args)...);

}


SCENARIO("Safe integers may be constructed from integers of any type") {

	//	EQUAL WIDTH

	WHEN("A safe integer of unsigned type is constructed from an integer of equal width and signedness") {
	
		typedef unsigned int type;
		typedef Integer<type> stype;
		
		AND_WHEN("The integer is zero") {
		
			type i=0;
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK(stype(i)==i);
			
			}
		
		}
		
		AND_WHEN("The integer is a small positive value") {
		
			type i=1;
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK(stype(i)==i);
			
			}
		
		}
		
		AND_WHEN("The integer is the largest value") {
		
			type i=std::numeric_limits<type>::max();
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK(stype(i)==i);
			
			}
		
		}
	
	}
	
	WHEN("A safe integer of signed type is constructed from an integer of equal width and signedness") {
	
		typedef int type;
		typedef Integer<type> stype;
		
		AND_WHEN("The integer is zero") {
		
			type i=0;
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK(stype(i)==i);
			
			}
		
		}
		
		AND_WHEN("The integer is a small positive value") {
		
			type i=1;
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK(stype(i)==i);
			
			}
		
		}
		
		AND_WHEN("The integer is the largest value") {
		
			type i=std::numeric_limits<type>::max();
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK(stype(i)==i);
			
			}
		
		}
		
		AND_WHEN("The integer is a negative value") {
		
			type i=-1;
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK(stype(i)==i);
			
			}
		
		}
		
		AND_WHEN("The integer is the smallest value") {
		
			type i=std::numeric_limits<type>::min();
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK(stype(i)==i);
			
			}
		
		}
	
	}

	WHEN("A safe integer of unsigned type is constructed from an integer of equal width and of signed type") {
	
		typedef Integer<unsigned int> utype;
		typedef int stype;
	
		AND_WHEN("The integer is negative") {
		
			stype i=-1;
		
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(Construct<utype>(i),std::overflow_error);
			
			}
		
		}
		
		AND_WHEN("The integer is positive") {
		
			stype i=1;
		
			THEN("The safe integer is constructed successfully") {
			
				CHECK(utype(i)==i);
			
			}
		
		}
		
		AND_WHEN("The integer is zero") {
		
			stype i=0;
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK(utype(i)==i);
			
			}
		
		}
	
	}
	
	WHEN("A safe integer of signed type is constructed from an integer of equal width and of unsigned type") {
	
		typedef Integer<int> stype;
		typedef unsigned int utype;
		
		AND_WHEN("The integer is zero") {
		
			utype i=0;
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK(stype(i)==i);
			
			}
		
		}
		
		AND_WHEN("The integer is within the range of the signed type") {
		
			utype i=1;
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK(stype(i)==i);
			
			}
		
		}
		
		AND_WHEN("The integer is beyond the range of the signed type") {
		
			utype i=std::numeric_limits<utype>::max();
			
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(Construct<stype>(i),std::overflow_error);
			
			}
		
		}
	
	}

	//	NARROWING
	
	WHEN("A safe integer of unsigned type is constructed from an unsigned integer of greater width") {
	
		typedef Integer<std::uint8_t> stype;
		typedef std::uint16_t wtype;
		
		AND_WHEN("The integer is zero") {
		
			wtype i=0;
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK(stype(i)==i);
			
			}
		
		}
		
		AND_WHEN("The integer is within range") {
		
			wtype i=1;
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK(stype(i)==i);
			
			}
		
		}
		
		AND_WHEN("The integer is out of range") {
		
			wtype i=static_cast<wtype>(std::numeric_limits<std::uint8_t>::max())+1;
			
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(Construct<stype>(i),std::overflow_error);
			
			}
			
		}
	
	}
	
	WHEN("A safe integer of signed type is constructed from a signed integer of greater width") {
	
		typedef Integer<std::int8_t> stype;
		typedef std::int16_t wtype;
		
		AND_WHEN("The integer is zero") {
		
			wtype i=0;
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK(stype(i)==i);
			
			}
		
		}
		
		AND_WHEN("The integer is within range") {
		
			wtype i=1;
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK(stype(i)==i);
			
			}
		
		}
		
		AND_WHEN("The integer is too large") {
		
			wtype i=static_cast<wtype>(std::numeric_limits<std::int8_t>::max())+1;
			
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(Construct<stype>(i),std::overflow_error);
			
			}
			
		}
		
		AND_WHEN("The integer is too small") {
		
			wtype i=static_cast<wtype>(std::numeric_limits<std::int8_t>::min())-1;
			
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(Construct<stype>(i),std::overflow_error);
			
			}
			
		}
	
	}
	
	WHEN("A safe integer of unsigned type is constructed from a signed integer of greater width") {
	
		typedef std::int16_t type;
		typedef Integer<std::uint8_t> stype;
		
		AND_WHEN("The integer is zero") {
		
			type i=0;
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK(stype(i)==i);
			
			}
		
		}
		
		AND_WHEN("The integer is small and positive") {
		
			type i=1;
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK(stype(i)==i);
			
			}
		
		}
		
		AND_WHEN("The integer is large and positive") {
		
			type i=std::numeric_limits<type>::max();
			
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(Construct<stype>(i),std::overflow_error);
			
			}
		
		}
		
		AND_WHEN("The integer is negative") {
		
			type i=-1;
			
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(Construct<stype>(i),std::overflow_error);
			
			}
		
		}
	
	}
	
	WHEN("A safe integer of signed type is constructed from an unsigned integer of greater width") {
	
		typedef std::uint16_t type;
		typedef Integer<std::int8_t> stype;
		
		AND_WHEN("The integer is zero") {
		
			type i=0;
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK(stype(i)==i);
			
			}
		
		}
		
		AND_WHEN("The integer is small and positive") {
		
			type i=1;
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK(stype(i)==i);
			
			}
		
		}
		
		AND_WHEN("The integer is large and positive") {
		
			type i=std::numeric_limits<type>::max();
			
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(Construct<stype>(i),std::overflow_error);
			
			}
		
		}
	
	}
	
	//	WIDENING
	
	WHEN("A safe integer of unsigned type is constructed from an unsigned type of lesser width") {
	
		typedef Integer<std::uint16_t> stype;
		typedef std::uint8_t type;
		
		AND_WHEN("The integer is zero") {
		
			type i=0;
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK(stype(i)==i);
			
			}
		
		}
		
		AND_WHEN("The integer is positive") {
		
			type i=std::numeric_limits<type>::max();
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK(stype(i)==i);
			
			}
		
		}
	
	}
	
	WHEN("A safe integer of signed type is constructed from a signed type of lesser width") {
	
		typedef Integer<std::int16_t> stype;
		typedef std::int8_t type;
		
		AND_WHEN("The integer is zero") {
		
			type i=0;
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK(stype(i)==i);
			
			}
		
		}
		
		AND_WHEN("The integer is positive") {
		
			type i=std::numeric_limits<type>::max();
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK(stype(i)==i);
			
			}
		
		}
		
		AND_WHEN("The integer is negative") {
		
			type i=std::numeric_limits<type>::min();
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK(stype(i)==i);
			
			}
		
		}
	
	}
	
	WHEN("A safe integer of unsigned type is constructed from a signed integer of lesser width") {
	
		typedef Integer<std::uint16_t> stype;
		typedef std::int8_t type;
		
		AND_WHEN("The integer is zero") {
		
			type i=0;
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK(stype(i)==i);
			
			}
		
		}
		
		AND_WHEN("The integer is positive") {
		
			type i=std::numeric_limits<type>::max();
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK(stype(i)==i);
			
			}
		
		}
		
		AND_WHEN("The integer is negative") {
		
			type i=std::numeric_limits<type>::min();
			
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(Construct<stype>(i),std::overflow_error);
			
			}
		
		}
	
	}
	
	WHEN("A safe integer of signed type is constructed from an unsigned type of lesser width") {
	
		typedef Integer<std::int16_t> stype;
		typedef std::uint8_t type;
		
		AND_WHEN("The integer is zero") {
		
			type i=0;
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK(stype(i)==i);
			
			}
		
		}
		
		AND_WHEN("The integer is positive") {
		
			type i=std::numeric_limits<type>::max();
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK(stype(i)==i);
			
			}
		
		}
	
	}
	
}


SCENARIO("Safe integers may be safely cast to integers of any type") {

	WHEN("A safe integer of unsigned type is cast to an integer of equal width and of signed type") {
	
		typedef Integer<unsigned int> utype;
		typedef int stype;
		
		AND_WHEN("The safe integer is zero") {
		
			utype i(0);
			
			THEN("The integer is retrieved successfully") {
			
				CHECK((static_cast<stype>(i)==i));
			
			}
		
		}
		
		AND_WHEN("The safe integer is within range of the signed type") {
		
			utype i(1);
			
			THEN("The integer is retrieved successfully") {
			
				CHECK((static_cast<stype>(i)==i));
			
			}
		
		}
		
		AND_WHEN("The safe integer is outside the range of the signed type") {
		
			utype i(static_cast<utype::Type>(std::numeric_limits<stype>::max())+1);
			
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(i.Get<stype>(),std::overflow_error);
			
			}
		
		}
	
	}

}


SCENARIO("Safe integers may be safely converted to safe integers of other types") {

	WHEN("A safe integer of unsigned type is cast to a safe integer of type of equal width and of signed type") {
	
		typedef Integer<unsigned int> utype;
		typedef Integer<int> stype;
			
		AND_WHEN("The safe integer is zero") {
		
			utype i(0);
			
			THEN("No exception is thrown") {
			
				REQUIRE_NOTHROW(i.Get<stype>());
			
			}
		
		}
		
		AND_WHEN("The safe integer is within range of the signed type") {
		
			utype i(1);
			
			THEN("No exception is thrown") {
			
				REQUIRE_NOTHROW(i.Get<stype>());
			
			}
		
		}
		
		AND_WHEN("The safe integer is outside the range of the signed type") {
		
			utype i(static_cast<utype::Type>(std::numeric_limits<stype::Type>::max())+1);
			
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(i.Get<stype>(),std::overflow_error);
			
			}
		
		}
	
	}

}


SCENARIO("Safe integers may be safely added") {

	GIVEN("An unsigned safe integer") {
	
		Integer<unsigned int> s(1);
		
		WHEN("A number is added to it, such that the addition does not overflow") {
		
			s+=1U;
		
			THEN("The result is correct") {
			
				CHECK(s==2);
			
			}
		
		}
		
		WHEN("A number is added to it, such that the addition overflows") {
		
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(s+=std::numeric_limits<unsigned int>::max(),std::overflow_error);
			
			}
		
		}
		
		WHEN("A signed number is added to it, such that the addition does not overflow") {
		
			auto i=std::numeric_limits<int>::max();
			s+=i;
			
			THEN("The result is correct") {
			
				CHECK(s==(static_cast<unsigned int>(i)+1U));
			
			}
		
		}
		
		WHEN("A negative signed number is added to it") {
		
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(s+=-1,std::overflow_error);
			
			}
		
		}
	
	}
	
	GIVEN("A positive signed safe integer") {
	
		Integer<int> s(1);
		
		WHEN("A positive number is added to it, such that the addition does not overflow") {
		
			s+=1;
			
			THEN("The result is correct") {
			
				CHECK(s==2);
			
			}
		
		}
		
		WHEN("A number is added to it, such that the addition overflows") {
		
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(s+=std::numeric_limits<int>::max(),std::overflow_error);
			
			}
		
		}
		
		WHEN("An unsigned number that cannot be represented as a signed number of that width is added to it") {
		
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(s+=std::numeric_limits<unsigned int>::max(),std::overflow_error);
			
			}
		
		}
		
		WHEN("An unsigned number is added to it, such that addition overflows") {
			
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(s+=static_cast<unsigned int>(std::numeric_limits<int>::max()),std::overflow_error);
			
			}
		
		}
		
		WHEN("An unsigned number is added to it, such that the addition does not overflow") {
		
			s+=1U;
			
			THEN("The result is correct") {
			
				CHECK(s==2);
			
			}
		
		}
		
		WHEN("A negative number is added to it") {
		
			s+=std::numeric_limits<int>::min();
			
			THEN("The result is correct") {
			
				CHECK(s==(std::numeric_limits<int>::min()+1));
			
			}
		
		}
	
	}
	
	GIVEN("A negative signed safe integer") {
	
		Integer<int> s(-1);
		
		WHEN("A positive number is added to it, such that the addition does not overflow") {
		
			s+=-1;
			
			THEN("The result is correct") {
			
				CHECK(s==-2);
			
			}
		
		}
		
		WHEN("A negative number is added to it, such that the addition does not overflow") {
		
			s+=-1;
			
			THEN("The result is correct") {
			
				CHECK(s==-2);
			
			}
		
		}
		
		WHEN("A number is added to it, such that the addition overflows") {
		
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(s+=std::numeric_limits<int>::min(),std::overflow_error);
			
			}
		
		}
	
	}

}


SCENARIO("Safe integers may be safely incremented") {

	GIVEN("A safe integer which is not the maximum value") {
	
		int i=1;
		Integer<int> s(i);
		
		WHEN("It is pre incremented") {
		
			THEN("The result is correct") {
			
				CHECK(((++s)==(++i)));
				CHECK((s==i));
			
			}
		
		}
		
		WHEN("It is post incremented") {
		
			THEN("The result is correct") {
			
				CHECK(((s++)==(i++)));
				CHECK((s==i));
			
			}
		
		}
	
	}
	
	GIVEN("A safe integer which is the maximum value") {
	
		auto i=std::numeric_limits<int>::max();
		Integer<int> s(i);
		
		WHEN("It is pre incremented") {
		
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(++s,std::overflow_error);
				CHECK((s==i));
			
			}
		
		}
		
		WHEN("It is post incremented") {
		
			THEN("An exception is thrown and the safe integer does not change") {
			
				REQUIRE_THROWS_AS(s++,std::overflow_error);
				CHECK((s==i));
			
			}
		
		}
	
	}

}


SCENARIO("Safe integers may be safely subtracted") {

	GIVEN("An unsigned safe integer") {
	
		Integer<unsigned int> s(1);
		
		WHEN("A number is subtracted from it, such that the subtraction does not overflow") {
		
			s-=1U;
			
			THEN("The result is correct") {
			
				CHECK(s==0);
			
			}
		
		}
		
		WHEN("A number is subtracted from it, such that the subtraction overflows") {
			
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(s-=2U,std::overflow_error);
			
			}
		
		}
		
		WHEN("A signed number is subtracted from it, such that the subtraction does not overflow") {
		
			s-=1;
			
			THEN("The result is correct") {
			
				CHECK(s==0);
			
			}
		
		}
		
		WHEN("A negative signed number is subtracted from it") {
		
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(s-=-2,std::overflow_error);
			
			}
		
		}
	
	}

	GIVEN("A positive signed safe integer") {
	
		Integer<int> s(1);
		
		WHEN("A positive number is subtracted from it, such that the subtraction does not overflow") {
		
			s-=2;
			
			THEN("The result is correct") {
			
				CHECK(s==-1);
			
			}
		
		}
		
		WHEN("A number is subtracted from it, such that the subtraction overflows") {
		
			THEN("An exception is thrown") {
		
				REQUIRE_THROWS_AS(s-=std::numeric_limits<int>::min(),std::overflow_error);
				
			}
		
		}
		
		WHEN("An unsigned number that cannot be represented as a signed number of that width is subtracted from it") {
		
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(s-=std::numeric_limits<unsigned int>::max(),std::overflow_error);
			
			}
		
		}
		
		WHEN("An unsigned number is subtracted from it, such that subtraction does not overflow") {
		
			s-=2U;
			
			THEN("The result is correct") {
			
				CHECK(s==-1);
			
			}
		
		}
		
		WHEN("A negative number is subtracted from it") {
		
			s-=-2;
			
			THEN("The result is correct") {
			
				CHECK(s==3);
			
			}
		
		}
	
	}
	
	GIVEN("A negative signed safe integer") {
	
		Integer<int> s(-2);
		
		WHEN("A positive number is subtracted from it, such that the subtraction does not overflow") {
		
			s-=1;
			
			THEN("The result is correct") {
			
				CHECK(s==-3);
			
			}
		
		}
		
		WHEN("A negative number is subtracted from it, such that the subtraction does not overflow") {
		
			s-=-1;
			
			THEN("The result is correct") {
			
				CHECK(s==-1);
			
			}
		
		}
		
		WHEN("A number is subtracted from it, such that the subtraction overflows") {
		
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(s-=std::numeric_limits<int>::max(),std::overflow_error);
			
			}
		
		}
		
		WHEN("An unsigned number is subtracted from it, such that the subtraction overflows") {
		
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(s-=static_cast<unsigned int>(std::numeric_limits<int>::max()),std::overflow_error);
			
			}
		
		}
	
	}
	
}


SCENARIO("Safe integers may be safely decremented") {

	GIVEN("A safe integer which is not the minimum value") {
	
		int i=1;
		Integer<int> s(i);
		
		WHEN("It is pre decremented") {
		
			THEN("The result is correct") {
			
				CHECK(((--s)==(--i)));
				CHECK((s==i));
			
			}
		
		}
		
		WHEN("It is post decremented") {
		
			THEN("The result is correct") {
			
				CHECK(((s--)==(i--)));
				CHECK((s==i));
			
			}
		
		}
	
	}
	
	GIVEN("A safe integer which is the minimum value") {
	
		auto i=std::numeric_limits<int>::min();
		Integer<int> s(i);
		
		WHEN("It is pre decremented") {
		
			THEN("An exception is thrown and the safe integer does not change") {
			
				REQUIRE_THROWS_AS(--s,std::overflow_error);
				CHECK((s==i));
			
			}
		
		}
		
		WHEN("It is post decremented") {
		
			THEN("An exception is thrown and the safe integer does not change") {
			
				REQUIRE_THROWS_AS(s--,std::overflow_error);
				CHECK((s==i));
			
			}
		
		}
	
	}

}


SCENARIO("Safe integers may be safely multiplied") {

	GIVEN("An unsigned safe integer") {
	
		Integer<unsigned int> s(2);
		
		WHEN("It is multiplied by a number, such that the multiplication does not overflow") {
		
			s*=2U;
			
			THEN("The result is correct") {
			
				CHECK(s==4);
			
			}
		
		}
		
		WHEN("It is multiplied by a number, such that the multiplication overflows") {
		
			THEN("An exception is thrown") {
		
				REQUIRE_THROWS_AS(s*=(std::numeric_limits<unsigned int>::max()/2)+1,std::overflow_error);
				
			}
		
		}
		
		WHEN("It is multiplied by a signed number, such that the multiplication does not overflow") {
		
			s*=2;
			
			THEN("The result is correct") {
			
				CHECK(s==4);
			
			}
		
		}
		
		WHEN("It is multiplied by a negative signed number") {
		
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(s*=-1,std::overflow_error);
			
			}
		
		}
	
	}
	
	GIVEN("A positive signed safe integer") {
	
		Integer<int> s(2);
		
		WHEN("It is multiplied by a positive number, such that the multiplication does not overflow") {
		
			s*=2;
			
			THEN("The result is correct") {
			
				CHECK(s==4);
			
			}
		
		}
		
		WHEN("It is multiplied by a positive number, such that the multiplication overflows") {
		
			THEN("An exception is thrown") {
		
				REQUIRE_THROWS_AS(s*=(std::numeric_limits<int>::max()/2)+1,std::overflow_error);
				
			}
		
		}
		
		WHEN("It is multiplied by an unsigned number that cannot be represented as a signed number of that width") {
		
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(s*=std::numeric_limits<unsigned int>::max(),std::overflow_error);
			
			}
		
		}
		
		WHEN("It is multiplied by an unsigned number, such that the multiplication does not overflow") {
		
			s*=2U;
			
			THEN("The result is correct") {
			
				CHECK(s==4);
			
			}
		
		}
		
		WHEN("It is multiplied by an unsigned number, such that the multiplication overflows") {
		
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(s*=static_cast<unsigned int>(std::numeric_limits<int>::max()),std::overflow_error);
			
			}
		
		}
		
		WHEN("It is multiplied by a negative number") {
		
			s*=-1;
			
			THEN("The result is correct") {
			
				CHECK(s==-2);
			
			}
		
		}
		
		WHEN("It is multiplied by a negative number, such that the multiplication overflows") {
		
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(s*=(std::numeric_limits<int>::min()/2)-1,std::overflow_error);
			
			}
		
		}
	
	}
	
	GIVEN("A negative signed safe integer") {
	
		Integer<int> s(-2);
		
		WHEN("It is multiplied by a positive number") {
		
			s*=2;
			
			THEN("The result is correct") {
			
				CHECK(s==-4);
			
			}
		
		}
		
		WHEN("It is multiplied by a positive number, such that the multiplication overflows") {
		
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(s*=(std::numeric_limits<int>::max()/2)+1,std::overflow_error);
				
			}
		
		}
		
		WHEN("It is multiplied by a negative number") {
		
			s*=-2;
			
			THEN("The result is correct") {
			
				CHECK(s==4);
			
			}
		
		}
		
		WHEN("It is multiplied by a negative number, such that the multiplication overflows") {
		
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(s*=(std::numeric_limits<int>::min()/2)-1,std::overflow_error);
			
			}
		
		}
	
		WHEN("It is multiplied by an unsigned number") {
		
			s*=2U;
			
			THEN("The result is correct") {
			
				CHECK(s==-4);
			
			}
		
		}
		
		WHEN("It is multiplied by an unsigned number, such that the multiplication overflows") {
		
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(s*=static_cast<unsigned int>((std::numeric_limits<int>::max()/2)+1),std::overflow_error);
			
			}
		
		}
	
	}

}


SCENARIO("Safe integers may be safely divided") {

	GIVEN("An unsigned safe integer") {
	
		Integer<unsigned int> s(1);
		
		WHEN("It is divided by a non-zero number") {
		
			THEN("The result is correct") {
			
				CHECK((s/5)==0);
			
			}
			
			THEN("The remainder is correct") {
			
				CHECK((s%5)==1);
			
			}
		
		}
		
		WHEN("It is divided by zero") {
		
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(s/0,std::overflow_error);
				REQUIRE_THROWS_AS(s%0,std::overflow_error);
			
			}
		
		}
	
	}
	
	GIVEN("A signed safe integer") {
	
		Integer<int> s(-1);
		
		WHEN("It is divided by a non-zero number") {
		
			THEN("The result is correct") {
			
				CHECK((s/5)==0);
			
			}
			
			THEN("The remainder is correct") {
			
				CHECK((s%5)==-1);
			
			}
		
		}
		
		WHEN("It is divided by zero") {
		
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(s/0,std::overflow_error);
				REQUIRE_THROWS_AS(s%0,std::overflow_error);
			
			}
		
		}
	
	}
	
	GIVEN("The smallest safe integer") {
	
		Integer<unsigned int> s(std::numeric_limits<unsigned int>::min());
		
		WHEN("It is divided by negative one") {
		
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(s/-1,std::overflow_error);
				REQUIRE_THROWS_AS(s%-1,std::overflow_error);
			
			}
		
		}
	
	}

}


SCENARIO("The magnitude of safe integers may be found safely") {

	GIVEN("A zero unsigned safe integer") {
	
		Integer<unsigned int> s(0);
		
		THEN("The magnitude is zero") {
		
			CHECK(s.Abs()==0);
		
		}
	
	}
	
	GIVEN("A positive unsigned safe integer") {
	
		Integer<unsigned int> s(1);
		
		THEN("The magnitude is identical to the integer") {
		
			CHECK((s.Abs()==s));
		
		}
	
	}
	
	GIVEN("The largest unsigned safe integer") {
	
		Integer<unsigned int> s(std::numeric_limits<unsigned int>::max());
		
		THEN("The magnitude is identical to the integer") {
		
			CHECK((s.Abs()==s));
		
		}
	
	}
	
	GIVEN("A zero signed safe integer") {
	
		Integer<int> s(0);
		
		THEN("The magnitude is zero") {
		
			CHECK(s.Abs()==0);
		
		}
	
	}
	
	GIVEN("A positive signed safe integer") {
	
		Integer<int> s(1);
		
		THEN("The magnitude is identical to the integer") {
		
			CHECK((s.Abs()==s));
		
		}
	
	}
	
	GIVEN("The largest signed safe integer") {
	
		Integer<int> s(std::numeric_limits<int>::max());
		
		THEN("The magnitude is identical to the integer") {
		
			CHECK((s.Abs()==s));
		
		}
	
	}
	
	GIVEN("A negative signed safe integer") {
	
		Integer<int> s(-1);
		
		THEN("The magnitude is the integer multiplied by negative one") {
		
			CHECK((s.Abs()==(s*-1)));
		
		}
	
	}
	
	GIVEN("The smallest signed safe integer") {
	
		Integer<int> s(std::numeric_limits<int>::min());
		
		THEN("Attempting to find the magnitude throws an exception") {
		
			REQUIRE_THROWS_AS(s.Abs(),std::overflow_error);
		
		}
	
	}

}


SCENARIO("Unary plus does not change a safe integer") {

	GIVEN("An unsigned safe integer which contains zero") {
	
		Integer<unsigned int> s;
		
		WHEN("Unary plus is applied to it") {
		
			s=+s;
			
			THEN("The result is zero") {
			
				CHECK(s==0);
			
			}
		
		}
	
	}
	
	GIVEN("An unsigned safe integer which contains a positive integer") {
	
		unsigned int i=1;
		Integer<unsigned int> s(i);
		
		WHEN("Unary plus is applied to it") {
		
			s=+s;
			
			THEN("The result is the same as the integer") {
			
				CHECK(s==i);
			
			}
		
		}
	
	}
	
	GIVEN("An signed safe integer which contains zero") {
	
		Integer<int> s;
		
		WHEN("Unary plus is applied to it") {
		
			s=+s;
			
			THEN("The result is zero") {
			
				CHECK(s==0);
			
			}
		
		}
	
	}
	
	GIVEN("A signed safe integer which contains a positive integer") {
	
		int i=1;
		Integer<int> s(i);
		
		WHEN("Unary plus is applied to it") {
		
			s=+s;
			
			THEN("The result is the same as the integer") {
			
				CHECK(s==i);
			
			}
		
		}
	
	}
	
	GIVEN("A signed safe integer which contains a negative integer") {
	
		int i=-1;
		Integer<int> s(i);
		
		WHEN("Unary plus is applied to it") {
		
			s=+s;
			
			THEN("The result is the same as the integer") {
			
				CHECK(s==i);
			
			}
		
		}
	
	}

}


SCENARIO("Applying unary minus to an unsigned safe integer throws unless the integer is zero") {

	GIVEN("An unsigned safe integer which contains zero") {
	
		Integer<unsigned int> s;
		
		WHEN("Unary minus is applied to it") {
		
			s=-s;
		
			THEN("The result is zero") {
			
				CHECK(s==0);
			
			}
		
		}
	
	}
	
	GIVEN("An unsigned safe integer which does not contain zero") {
	
		Integer<unsigned int> s(1);
		
		WHEN("Unary minus is applied to it") {
			
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(-s,std::overflow_error);
			
			}
		
		}
	
	}

}


SCENARIO("Applying unary minus to a signed integer multiplies it by negative one") {

	typedef Integer<int> type;
	typedef std::numeric_limits<type::Type> limits;

	GIVEN("A signed safe integer which contains zero") {
	
		type i;
		
		WHEN("Unary minus is applied to it") {
		
			i=-i;
			
			THEN("The result is zero") {
			
				CHECK(i==0);
			
			}
		
		}
	
	}
	
	GIVEN("A signed safe integer which contains a positive integer") {
	
		auto i=limits::max();
		type s(i);
		
		WHEN("Unary minus is applied to it") {
		
			s=-s;
			
			THEN("The result is that integer multiplied by negative one") {
			
				CHECK(s==-i);
			
			}
		
		}
	
	}
	
	GIVEN("A signed safe integer which contains a negative number which is not the smallest value") {
	
		auto i=limits::min()+1;
		type s(i);
		
		WHEN("Unary minus is applied to it") {
		
			s=-s;
			
			THEN("The result is that integer multiplied by negative one") {
			
				CHECK(s==-i);
			
			}
		
		}
	
	}
	
	GIVEN("The smallest value") {
	
		type s(limits::min());
		
		WHEN("Unary minus is applied to it") {
		
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(-s,std::overflow_error);
			
			}
		
		}
	
	}

}


SCENARIO("Safe integers may be compared for equality") {

	GIVEN("A signed safe integer") {
	
		Integer<int> s(-1);
		
		GIVEN("An integer of the same type with the same value") {
		
			auto i=s.Get();
			
			THEN("They compare equal") {
			
				CHECK((s==i));
				CHECK(!(s!=i));
				CHECK((i==s));
				CHECK(!(i!=s));
				
				CHECK((s>=i));
				CHECK((s<=i));
				CHECK((i>=s));
				CHECK((i<=s));
				
				CHECK(!(s<i));
				CHECK(!(s>i));
				CHECK(!(i<s));
				CHECK(!(i>s));
			
			}
		
		}
		
		GIVEN("A safe integer of the same type with the same value") {
		
			auto i=s;
			
			THEN("They compare equal") {
			
				CHECK((s==i));
				CHECK(!(s!=i));
				CHECK((i==s));
				CHECK(!(i!=s));
				
				CHECK((s>=i));
				CHECK((s<=i));
				CHECK((i>=s));
				CHECK((i<=s));
				
				CHECK(!(s<i));
				CHECK(!(s>i));
				CHECK(!(i<s));
				CHECK(!(i>s));
			
			}
		
		}
		
		GIVEN("An integer of a different type with the same value") {
		
			auto i=s.Get<signed char>();
			
			THEN("They compare equal") {
			
				CHECK((s==i));
				CHECK(!(s!=i));
				CHECK((i==s));
				CHECK(!(i!=s));
				
				CHECK((s>=i));
				CHECK((s<=i));
				CHECK((i>=s));
				CHECK((i<=s));
				
				CHECK(!(s<i));
				CHECK(!(s>i));
				CHECK(!(i<s));
				CHECK(!(i>s));
			
			}
		
		}
		
		GIVEN("A safe integer of a different type with the same value") {
		
			Integer<signed char> i(s);
			
			THEN("They compare equal") {
			
				CHECK((s==i));
				CHECK(!(s!=i));
				CHECK((i==s));
				CHECK(!(i!=s));
				
				CHECK((s>=i));
				CHECK((s<=i));
				CHECK((i>=s));
				CHECK((i<=s));
				
				CHECK(!(s<i));
				CHECK(!(s>i));
				CHECK(!(i<s));
				CHECK(!(i>s));
			
			}
		
		}
		
		GIVEN("An integer of the same type with a different value") {
		
			int i=1;
			
			THEN("They compare unequal") {
			
				CHECK((s!=i));
				CHECK(!(s==i));
				CHECK((i!=s));
				CHECK(!(i==s));
			
			}
		
		}
		
		GIVEN("A safe integer of the same type with a different value") {
		
			Integer<int> i(1);
			
			THEN("They compare unequal") {
			
				CHECK((s!=i));
				CHECK(!(s==i));
				CHECK((i!=s));
				CHECK(!(i==s));
			
			}
		
		}
		
		GIVEN("An integer of a different type with a different value") {
		
			signed char i=1;
			
			THEN("They compare unequal") {
			
				CHECK((s!=i));
				CHECK(!(s==i));
				CHECK((i!=s));
				CHECK(!(i==s));
			
			}
		
		}
		
		GIVEN("A safe integer of a different type with a different value") {
		
			Integer<signed char> i(1);
			
			THEN("They compare unequal") {
			
				CHECK((s!=i));
				CHECK(!(s==i));
				CHECK((i!=s));
				CHECK(!(i==s));
			
			}
		
		}
	
	}
	
	GIVEN("An unsigned safe integer") {
	
		Integer<unsigned int> s(2);
		
		GIVEN("An integer of the same type with the same value") {
		
			auto i=s.Get();
			
			THEN("They compare equal") {
			
				CHECK((s==i));
				CHECK(!(s!=i));
				CHECK((i==s));
				CHECK(!(i!=s));
				
				CHECK((s>=i));
				CHECK((s<=i));
				CHECK((i>=s));
				CHECK((i<=s));
				
				CHECK(!(s<i));
				CHECK(!(s>i));
				CHECK(!(i<s));
				CHECK(!(i>s));
			
			}
		
		}
		
		GIVEN("A safe integer of the same type with the same value") {
		
			auto i=s;
			
			THEN("They compare equal") {
			
				CHECK((s==i));
				CHECK(!(s!=i));
				CHECK((i==s));
				CHECK(!(i!=s));
				
				CHECK((s>=i));
				CHECK((s<=i));
				CHECK((i>=s));
				CHECK((i<=s));
				
				CHECK(!(s<i));
				CHECK(!(s>i));
				CHECK(!(i<s));
				CHECK(!(i>s));
			
			}
		
		}
		
		GIVEN("An integer of a different type with the same value") {
		
			auto i=s.Get<signed char>();
			
			THEN("They compare equal") {
			
				CHECK((s==i));
				CHECK(!(s!=i));
				CHECK((i==s));
				CHECK(!(i!=s));
				
				CHECK((s>=i));
				CHECK((s<=i));
				CHECK((i>=s));
				CHECK((i<=s));
				
				CHECK(!(s<i));
				CHECK(!(s>i));
				CHECK(!(i<s));
				CHECK(!(i>s));
			
			}
		
		}
		
		GIVEN("A safe integer of a different type with the same value") {
		
			Integer<signed char> i(s);
			
			THEN("They compare equal") {
			
				CHECK((s==i));
				CHECK(!(s!=i));
				CHECK((i==s));
				CHECK(!(i!=s));
				
				CHECK((s>=i));
				CHECK((s<=i));
				CHECK((i>=s));
				CHECK((i<=s));
				
				CHECK(!(s<i));
				CHECK(!(s>i));
				CHECK(!(i<s));
				CHECK(!(i>s));
			
			}
		
		}
		
		GIVEN("An integer of the same type with a different value") {
		
			int i=1;
			
			THEN("They compare unequal") {
			
				CHECK((s!=i));
				CHECK(!(s==i));
				CHECK((i!=s));
				CHECK(!(i==s));
			
			}
		
		}
		
		GIVEN("A safe integer of the same type with a different value") {
		
			Integer<int> i(1);
			
			THEN("They compare unequal") {
			
				CHECK((s!=i));
				CHECK(!(s==i));
				CHECK((i!=s));
				CHECK(!(i==s));
			
			}
		
		}
		
		GIVEN("An integer of a different type with a different value") {
		
			signed char i=1;
			
			THEN("They compare unequal") {
			
				CHECK((s!=i));
				CHECK(!(s==i));
				CHECK((i!=s));
				CHECK(!(i==s));
			
			}
		
		}
		
		GIVEN("A safe integer of a different type with a different value") {
		
			Integer<signed char> i(1);
			
			THEN("They compare unequal") {
			
				CHECK((s!=i));
				CHECK(!(s==i));
				CHECK((i!=s));
				CHECK(!(i==s));
			
			}
		
		}
	
	}

	GIVEN("An unsigned safe integer of maximum value") {
	
		Integer<unsigned int> s(std::numeric_limits<unsigned int>::max());
		
		GIVEN("The same width of integer, signed, equal to negative one") {
		
			int i=-1;
			
			THEN("They compare unequal") {
			
				CHECK((s!=i));
				CHECK(!(s==i));
				CHECK((i!=s));
				CHECK(!(i==s));
			
			}
		
		}
	
	}
	
}


SCENARIO("Safe integers may be compared for inequality") {

	GIVEN("A signed safe integer") {
	
		Integer<int> s(1);
		
		GIVEN("An integer of the same type with a lesser value") {
		
			int i=-1;
			
			THEN("The latter compares less than the former") {
			
				CHECK((i<s));
				CHECK(!(s<i));
				CHECK((i<=s));
				CHECK(!(s<=i));
				
				CHECK((s>i));
				CHECK(!(i>s));
				CHECK((s>=i));
				CHECK(!(i>=s));
			
			}
		
		}
		
		GIVEN("A safe integer of the same type with a lesser value") {
		
			Integer<int> i(-1);
			
			THEN("The latter compares less than the former") {
			
				CHECK((i<s));
				CHECK(!(s<i));
				CHECK((i<=s));
				CHECK(!(s<=i));
				
				CHECK((s>i));
				CHECK(!(i>s));
				CHECK((s>=i));
				CHECK(!(i>=s));
			
			}
		
		}
		
		GIVEN("An integer of a different type with a lesser value") {
		
			unsigned int i=0;
			
			THEN("The latter compares less than the former") {
			
				CHECK((i<s));
				CHECK(!(s<i));
				CHECK((i<=s));
				CHECK(!(s<=i));
				
				CHECK((s>i));
				CHECK(!(i>s));
				CHECK((s>=i));
				CHECK(!(i>=s));
			
			}
		
		}
		
		GIVEN("A signed integer of a different type with a lesser value") {
		
			Integer<unsigned int> i(0);
			
			THEN("The latter compares less than the former") {
			
				CHECK((i<s));
				CHECK(!(s<i));
				CHECK((i<=s));
				CHECK(!(s<=i));
				
				CHECK((s>i));
				CHECK(!(i>s));
				CHECK((s>=i));
				CHECK(!(i>=s));
			
			}
		
		}
	
	}
	
	GIVEN("An unsigned safe integer") {
	
		Integer<unsigned int> s(1);
		
		GIVEN("An integer of a different type with a lesser value") {
		
			int i=-1;
			
			THEN("The latter compares less than the former") {
			
				CHECK((i<s));
				CHECK(!(s<i));
				CHECK((i<=s));
				CHECK(!(s<=i));
				
				CHECK((s>i));
				CHECK(!(i>s));
				CHECK((s>=i));
				CHECK(!(i>=s));
			
			}
		
		}
		
		GIVEN("A safe integer of a different type with a lesser value") {
		
			Integer<int> i(-1);
			
			THEN("The latter compares less than the former") {
			
				CHECK((i<s));
				CHECK(!(s<i));
				CHECK((i<=s));
				CHECK(!(s<=i));
				
				CHECK((s>i));
				CHECK(!(i>s));
				CHECK((s>=i));
				CHECK(!(i>=s));
			
			}
		
		}
		
		GIVEN("An integer of the same type with a lesser value") {
		
			unsigned int i=0;
			
			THEN("The latter compares less than the former") {
			
				CHECK((i<s));
				CHECK(!(s<i));
				CHECK((i<=s));
				CHECK(!(s<=i));
				
				CHECK((s>i));
				CHECK(!(i>s));
				CHECK((s>=i));
				CHECK(!(i>=s));
			
			}
		
		}
		
		GIVEN("A signed integer of the same type with a lesser value") {
		
			Integer<unsigned int> i(0);
			
			THEN("The latter compares less than the former") {
			
				CHECK((i<s));
				CHECK(!(s<i));
				CHECK((i<=s));
				CHECK(!(s<=i));
				
				CHECK((s>i));
				CHECK(!(i>s));
				CHECK((s>=i));
				CHECK(!(i>=s));
			
			}
		
		}
	
	}
	
}
