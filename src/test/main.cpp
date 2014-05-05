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
#include <cstddef>
#include <cstdint>
#include <functional>
#include <limits>
#include <stdexcept>
#include <type_traits>
#include <utility>


using Safe::Cast;
using Safe::Integer;


template <typename T, typename... Args>
T Construct (Args &&... args) noexcept(std::is_nothrow_constructible<T,Args...>::value) {

	return T(std::forward<Args>(args)...);

}


template <typename To, typename From>
To Convert (From && from) noexcept(std::is_nothrow_constructible<To,From>::value) {

	return To(std::forward<From>(from));

}


template <typename T>
std::size_t Hash (T && obj) noexcept(noexcept(std::hash<typename std::decay<T>::type>{}(std::forward<T>(obj)))) {

	return std::hash<typename std::decay<T>::type>{}(std::forward<T>(obj));

}


SCENARIO("Default constructed safe integers have a value of zero") {

	GIVEN("A default constructed safe integer of signed type") {
	
		Integer<int> i;
		
		THEN("Its value is zero") {
		
			CHECK((i==0));
		
		}
	
	}
	
	GIVEN("A default constructed safe integer of unsigned type") {
	
		Integer<unsigned int> i;
		
		THEN("Its value is zero") {
		
			CHECK((i==0));
		
		}
	
	}

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


SCENARIO("Safe integers may be constructed from safe integers of any type") {

	//	EQUAL WIDTH

	WHEN("A safe integer of unsigned type is constructed from an integer of equal width and of signed type") {
	
		typedef Integer<unsigned int> utype;
		typedef Integer<int> stype;
	
		AND_WHEN("The integer is negative") {
		
			stype i=-1;
		
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(Construct<utype>(i),std::overflow_error);
			
			}
		
		}
		
		AND_WHEN("The integer is positive") {
		
			stype i=1;
		
			THEN("The safe integer is constructed successfully") {
			
				CHECK((utype(i)==i));
			
			}
		
		}
		
		AND_WHEN("The integer is zero") {
		
			stype i=0;
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK((utype(i)==i));
			
			}
		
		}
	
	}
	
	WHEN("A safe integer of signed type is constructed from an integer of equal width and of unsigned type") {
	
		typedef Integer<int> stype;
		typedef Integer<unsigned int> utype;
		
		AND_WHEN("The integer is zero") {
		
			utype i=0;
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK((stype(i)==i));
			
			}
		
		}
		
		AND_WHEN("The integer is within the range of the signed type") {
		
			utype i=1;
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK((stype(i)==i));
			
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
		typedef Integer<std::uint16_t> wtype;
		
		AND_WHEN("The integer is zero") {
		
			wtype i=0;
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK((stype(i)==i));
			
			}
		
		}
		
		AND_WHEN("The integer is within range") {
		
			wtype i=1;
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK((stype(i)==i));
			
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
		typedef Integer<std::int16_t> wtype;
		
		AND_WHEN("The integer is zero") {
		
			wtype i=0;
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK((stype(i)==i));
			
			}
		
		}
		
		AND_WHEN("The integer is within range") {
		
			wtype i=1;
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK((stype(i)==i));
			
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
	
		typedef Integer<std::int16_t> type;
		typedef Integer<std::uint8_t> stype;
		
		AND_WHEN("The integer is zero") {
		
			type i=0;
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK((stype(i)==i));
			
			}
		
		}
		
		AND_WHEN("The integer is small and positive") {
		
			type i=1;
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK((stype(i)==i));
			
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
	
		typedef Integer<std::uint16_t> type;
		typedef Integer<std::int8_t> stype;
		
		AND_WHEN("The integer is zero") {
		
			type i=0;
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK((stype(i)==i));
			
			}
		
		}
		
		AND_WHEN("The integer is small and positive") {
		
			type i=1;
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK((stype(i)==i));
			
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
		typedef Integer<std::uint8_t> type;
		
		AND_WHEN("The integer is zero") {
		
			type i=0;
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK((stype(i)==i));
			
			}
		
		}
		
		AND_WHEN("The integer is positive") {
		
			type i=std::numeric_limits<type>::max();
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK((stype(i)==i));
			
			}
		
		}
	
	}
	
	WHEN("A safe integer of signed type is constructed from a signed type of lesser width") {
	
		typedef Integer<std::int16_t> stype;
		typedef Integer<std::int8_t> type;
		
		AND_WHEN("The integer is zero") {
		
			type i=0;
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK((stype(i)==i));
			
			}
		
		}
		
		AND_WHEN("The integer is positive") {
		
			type i=std::numeric_limits<type>::max();
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK((stype(i)==i));
			
			}
		
		}
		
		AND_WHEN("The integer is negative") {
		
			type i=std::numeric_limits<type>::min();
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK((stype(i)==i));
			
			}
		
		}
	
	}
	
	WHEN("A safe integer of unsigned type is constructed from a signed integer of lesser width") {
	
		typedef Integer<std::uint16_t> stype;
		typedef Integer<std::int8_t> type;
		
		AND_WHEN("The integer is zero") {
		
			type i=0;
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK((stype(i)==i));
			
			}
		
		}
		
		AND_WHEN("The integer is positive") {
		
			type i=std::numeric_limits<type>::max();
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK((stype(i)==i));
			
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
		typedef Integer<std::uint8_t> type;
		
		AND_WHEN("The integer is zero") {
		
			type i=0;
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK((stype(i)==i));
			
			}
		
		}
		
		AND_WHEN("The integer is positive") {
		
			type i=std::numeric_limits<type>::max();
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK((stype(i)==i));
			
			}
		
		}
	
	}
	
}


SCENARIO("Safe integers may be converted to integers of any type") {

	//	EQUAL WIDTH

	WHEN("A safe integer of unsigned type is converted to an integer of equal width and signedness") {
	
		typedef unsigned int type;
		typedef Integer<type> stype;
		
		AND_WHEN("The integer is zero") {
		
			stype i;
			
			THEN("The conversion is successful") {
			
				CHECK((static_cast<type>(i)==i));
				CHECK((i.Get()==i));
				CHECK((i.Get<type>()==i));
			
			}
		
		}
		
		AND_WHEN("The integer is a small positive value") {
		
			stype i(1);
			
			THEN("The conversion is successful") {
			
				CHECK((static_cast<type>(i)==i));
				CHECK((i.Get()==i));
				CHECK((i.Get<type>()==i));
			
			}
		
		}
		
		AND_WHEN("The integer is the largest value") {
		
			stype i(std::numeric_limits<type>::max());
			
			THEN("The conversion is successful") {
			
				CHECK((static_cast<type>(i)==i));
				CHECK((i.Get()==i));
				CHECK((i.Get<type>()==i));
			
			}
		
		}
	
	}
	
	WHEN("A safe integer of signed type is converted to an integer of equal width and signedness") {
	
		typedef int type;
		typedef Integer<type> stype;
		
		AND_WHEN("The integer is zero") {
		
			stype i;
			
			THEN("The conversion is successful") {
			
				CHECK((static_cast<type>(i)==i));
				CHECK((i.Get()==i));
				CHECK((i.Get<type>()==i));
			
			}
		
		}
		
		AND_WHEN("The integer is a small positive value") {
		
			stype i(1);
			
			THEN("The conversion is successful") {
			
				CHECK((static_cast<type>(i)==i));
				CHECK((i.Get()==i));
				CHECK((i.Get<type>()==i));
			
			}
		
		}
		
		AND_WHEN("The integer is the largest value") {
		
			stype i(std::numeric_limits<type>::max());
			
			THEN("The conversion is successful") {
			
				CHECK((static_cast<type>(i)==i));
				CHECK((i.Get()==i));
				CHECK((i.Get<type>()==i));
			
			}
		
		}
		
		AND_WHEN("The integer is a negative value") {
		
			stype i(-1);
			
			THEN("The conversion is successful") {
			
				CHECK((static_cast<type>(i)==i));
				CHECK((i.Get()==i));
				CHECK((i.Get<type>()==i));
			
			}
		
		}
		
		AND_WHEN("The integer is the smallest value") {
		
			stype i(std::numeric_limits<type>::min());
			
			THEN("The conversion is successful") {
			
				CHECK((static_cast<type>(i)==i));
				CHECK((i.Get()==i));
				CHECK((i.Get<type>()==i));
			
			}
		
		}
	
	}

	WHEN("A safe integer of unsigned type is converted to an integer of equal width and of signed type") {
	
		typedef Integer<unsigned int> utype;
		typedef int stype;
	
		AND_WHEN("The integer is too large to be represented as the signed type") {
		
			utype i(std::numeric_limits<stype>::max());
			++i;
		
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(Convert<stype>(i),std::overflow_error);
				REQUIRE_THROWS_AS(i.Get<stype>(),std::overflow_error);
			
			}
		
		}
		
		AND_WHEN("The integer is positive") {
		
			utype i=1;
		
			THEN("The conversion is successful") {
			
				CHECK((static_cast<stype>(i)==i));
				CHECK((i.Get<stype>()==i));
			
			}
		
		}
		
		AND_WHEN("The integer is zero") {
		
			utype i;
			
			THEN("The conversion is successful") {
			
				CHECK((static_cast<stype>(i)==i));
				CHECK((i.Get<stype>()==i));
			
			}
		
		}
	
	}
	
	WHEN("A safe integer of signed type is converted to an integer of equal width and of unsigned type") {
	
		typedef Integer<int> stype;
		typedef unsigned int utype;
		
		AND_WHEN("The integer is zero") {
		
			stype i;
			
			THEN("The conversion is successful") {
			
				CHECK((static_cast<utype>(i)==i));
				CHECK((i.Get<utype>()==i));
			
			}
		
		}
		
		AND_WHEN("The integer is positive") {
		
			stype i(std::numeric_limits<int>::max());
			
			THEN("The conversion is successful") {
			
				CHECK((static_cast<utype>(i)==i));
				CHECK((i.Get<utype>()==i));
			
			}
		
		}
		
		AND_WHEN("The integer is negative") {
		
			stype i(-1);
			
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(Convert<utype>(i),std::overflow_error);
				REQUIRE_THROWS_AS(i.Get<utype>(),std::overflow_error);
			
			}
		
		}
	
	}

	//	WIDENING
	
	WHEN("A safe integer of unsigned type is converted to an unsigned integer of greater width") {
	
		typedef Integer<std::uint8_t> stype;
		typedef std::uint16_t wtype;
		
		AND_WHEN("The integer is zero") {
		
			stype i;
			
			THEN("The conversion is successful") {
			
				CHECK((static_cast<wtype>(i)==i));
				CHECK((i.Get<wtype>()==i));
			
			}
		
		}
		
		AND_WHEN("The integer is positive") {
		
			stype i(std::numeric_limits<stype>::max());
			
			THEN("The conversion is successful") {
			
				CHECK((static_cast<wtype>(i)==i));
				CHECK((i.Get<wtype>()==i));
			
			}
		
		}
	
	}
	
	WHEN("A safe integer of signed type is converted to a signed integer of greater width") {
	
		typedef Integer<std::int8_t> stype;
		typedef std::int16_t wtype;
		
		AND_WHEN("The integer is zero") {
		
			stype i;
			
			THEN("The conversion is successful") {
			
				CHECK((static_cast<wtype>(i)==i));
				CHECK((i.Get<wtype>()==i));
			
			}
		
		}
		
		AND_WHEN("The integer is positive") {
		
			stype i(std::numeric_limits<stype>::max());
			
			THEN("The conversion is successful") {
			
				CHECK((static_cast<wtype>(i)==i));
				CHECK((i.Get<wtype>()==i));
			
			}
		
		}
		
		AND_WHEN("The integer is negative") {
		
			stype i(std::numeric_limits<stype>::min());
			
			THEN("The conversion is successful") {
			
				CHECK((static_cast<wtype>(i)==i));
				CHECK((i.Get<wtype>()==i));
			
			}
		
		}
	
	}
	
	WHEN("A safe integer of unsigned type is converted to a signed integer of greater width") {
	
		typedef std::int16_t type;
		typedef Integer<std::uint8_t> stype;
		
		AND_WHEN("The integer is zero") {
		
			stype i;
			
			THEN("The conversion is successful") {
			
				CHECK((static_cast<type>(i)==i));
				CHECK((i.Get<type>()==i));
			
			}
		
		}
		
		AND_WHEN("The integer is small and positive") {
		
			stype i(1);
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK((static_cast<type>(i)==i));
				CHECK((i.Get<type>()==i));
			
			}
		
		}
		
		AND_WHEN("The integer is large and positive") {
		
			stype i=std::numeric_limits<std::uint8_t>::max();
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK((static_cast<type>(i)==i));
				CHECK((i.Get<type>()==i));
			
			}
		
		}
	
	}
	
	WHEN("A safe integer of signed type is converted to an unsigned integer of greater width") {
	
		typedef std::uint16_t type;
		typedef Integer<std::int8_t> stype;
		
		AND_WHEN("The integer is zero") {
		
			stype i;
			
			THEN("The conversion is successful") {
			
				CHECK((static_cast<type>(i)==i));
				CHECK((i.Get<type>()==i));
			
			}
		
		}
		
		AND_WHEN("The integer is small and positive") {
		
			stype i(1);
			
			THEN("The conversion is successful") {
			
				CHECK((static_cast<type>(i)==i));
				CHECK((i.Get<type>()==i));
			
			}
		
		}
		
		AND_WHEN("The integer is large and positive") {
		
			stype i(std::numeric_limits<std::int8_t>::max());
			
			THEN("The conversion is successful") {
			
				CHECK((static_cast<type>(i)==i));
				CHECK((i.Get<type>()==i));
			
			}
		
		}
		
		AND_WHEN("The integer is negative") {
		
			stype i(-1);
			
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(Convert<type>(i),std::overflow_error);
				REQUIRE_THROWS_AS(i.Get<type>(),std::overflow_error);
			
			}
		
		}
	
	}
	
	//	NARROWING
	
	WHEN("A safe integer of unsigned type is converted to an unsigned type of lesser width") {
	
		typedef Integer<std::uint16_t> stype;
		typedef std::uint8_t type;
		
		AND_WHEN("The integer is zero") {
		
			stype i;
			
			THEN("The conversion is successful") {
			
				CHECK((static_cast<type>(i)==i));
				CHECK((i.Get<type>()==i));
				
			}
		
		}
		
		AND_WHEN("The integer is positive") {
		
			stype i(std::numeric_limits<type>::max());
			
			THEN("The conversion is successful") {
			
				CHECK((static_cast<type>(i)==i));
				CHECK((i.Get<type>()==i));
				
			}
		
		}
		
		AND_WHEN("The integer is beyond the range of the destination type") {
		
			stype i(std::numeric_limits<stype>::max());
			
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(Convert<type>(i),std::overflow_error);
				REQUIRE_THROWS_AS(i.Get<type>(),std::overflow_error);
			
			}
		
		}
	
	}
	
	WHEN("A safe integer of signed type is converted to a signed type of lesser width") {
	
		typedef Integer<std::int16_t> stype;
		typedef std::int8_t type;
		
		AND_WHEN("The integer is zero") {
		
			stype i;
			
			THEN("The conversion is successful") {
			
				CHECK((static_cast<type>(i)==i));
				CHECK((i.Get<type>()==i));
				
			}
		
		}
		
		AND_WHEN("The integer is positive") {
		
			stype i(std::numeric_limits<type>::max());
			
			THEN("The conversion is successful") {
			
				CHECK((static_cast<type>(i)==i));
				CHECK((i.Get<type>()==i));
				
			}
		
		}
		
		AND_WHEN("The integer is positive and beyond the range of the destination type") {
		
			stype i(std::numeric_limits<stype>::max());
			
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(Convert<type>(i),std::overflow_error);
				REQUIRE_THROWS_AS(i.Get<type>(),std::overflow_error);
			
			}
		
		}
		
		AND_WHEN("The integer is negative") {
		
			stype i(std::numeric_limits<type>::min());
			
			THEN("The conversion is successful") {
			
				CHECK((static_cast<type>(i)==i));
				CHECK((i.Get<type>()==i));
				
			}
		
		}
		
		AND_WHEN("The integer is negative and beyond the range of the destination type") {
		
			stype i(std::numeric_limits<stype>::min());
			
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(Convert<type>(i),std::overflow_error);
				REQUIRE_THROWS_AS(i.Get<type>(),std::overflow_error);
			
			}
		
		}
	
	}
	
	WHEN("A safe integer of unsigned type is converted to a signed integer of lesser width") {
	
		typedef Integer<std::uint16_t> stype;
		typedef std::int8_t type;
		
		AND_WHEN("The integer is zero") {
		
			stype i;
			
			THEN("The conversion is successful") {
			
				CHECK((static_cast<type>(i)==i));
				CHECK((i.Get<type>()==i));
				
			}
		
		}
		
		AND_WHEN("The integer is positive") {
		
			stype i(std::numeric_limits<type>::max());
			
			THEN("The conversion is successful") {
			
				CHECK((Convert<type>(i)==i));
				CHECK((i.Get<type>()==i));
				
			}
		
		}
		
		AND_WHEN("The integer is positive and beyond the range of the destination type") {
		
			stype i(std::numeric_limits<stype>::max());
			
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(Convert<type>(i),std::overflow_error);
				REQUIRE_THROWS_AS(i.Get<type>(),std::overflow_error);
			
			}
		
		}
	
	}
	
	WHEN("A safe integer of signed type is converted to an unsigned type of lesser width") {
	
		typedef Integer<std::int16_t> stype;
		typedef std::uint8_t type;
		
		AND_WHEN("The integer is zero") {
		
			stype i;
			
			THEN("The conversion is successful") {
			
				CHECK((static_cast<type>(i)==i));
				CHECK((i.Get<type>()==i));
				
			}
		
		}
		
		AND_WHEN("The integer is positive") {
		
			stype i(1);
			
			THEN("The conversion is successful") {
			
				CHECK((static_cast<type>(i)==i));
				CHECK((i.Get<type>()==i));
				
			}
		
		}
		
		AND_WHEN("The integer is positive and beyond the range of the destination type") {
		
			stype i(std::numeric_limits<type>::max());
			++i;
			
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(Convert<type>(i),std::overflow_error);
				REQUIRE_THROWS_AS(i.Get<type>(),std::overflow_error);
			
			}
		
		}
		
		AND_WHEN("The integer is negative") {
		
			stype i(-1);
			
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(Convert<type>(i),std::overflow_error);
				REQUIRE_THROWS_AS(i.Get<type>(),std::overflow_error);
			
			}
		
		}
	
	}
	
}


SCENARIO("Integers may be safely converted to integers of other types") {

	//	EQUAL WIDTH

	WHEN("An integer of unsigned type is converted to the same type") {
	
		typedef unsigned int type;
		
		AND_WHEN("The integer is zero") {
		
			type i=0;
			
			THEN("The conversion is successful") {
			
				CHECK(Cast<type>(i)==i);
			
			}
		
		}
		
		AND_WHEN("The integer is a small positive value") {
		
			type i=1;
			
			THEN("The conversion is successful") {
			
				CHECK(Cast<type>(i)==i);
			
			}
		
		}
		
		AND_WHEN("The integer is the largest value") {
		
			type i=std::numeric_limits<type>::max();
			
			THEN("The conversion is successful") {
			
				CHECK(Cast<type>(i)==i);
			
			}
		
		}
	
	}
	
	WHEN("An integer of signed type is converted to the same type") {
	
		typedef int type;
		
		AND_WHEN("The integer is zero") {
		
			type i=0;
			
			THEN("The conversion is successful") {
			
				CHECK(Cast<type>(i)==i);
			
			}
		
		}
		
		AND_WHEN("The integer is a small positive value") {
		
			type i=1;
			
			THEN("The conversion is successful") {
			
				CHECK(Cast<type>(i)==i);
			
			}
		
		}
		
		AND_WHEN("The integer is the largest value") {
		
			type i=std::numeric_limits<type>::max();
			
			THEN("The conversion is successful") {
			
				CHECK(Cast<type>(i)==i);
			
			}
		
		}
		
		AND_WHEN("The integer is a negative value") {
		
			type i=-1;
			
			THEN("The conversion is successful") {
			
				CHECK(Cast<type>(i)==i);
			
			}
		
		}
		
		AND_WHEN("The integer is the smallest value") {
		
			type i=std::numeric_limits<type>::min();
			
			THEN("The conversion is successful") {
			
				CHECK(Cast<type>(i)==i);
			
			}
		
		}
	
	}

	WHEN("An integer of unsigned type is converted to an integer of equal width and of signed type") {
	
		typedef unsigned int utype;
		typedef int stype;
	
		AND_WHEN("The integer is too large to be represented as the signed type") {
		
			auto i=static_cast<utype>(std::numeric_limits<stype>::max());
			++i;
		
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(Cast<stype>(i),std::overflow_error);
			
			}
		
		}
		
		AND_WHEN("The integer is positive") {
		
			utype i=1;
		
			THEN("The conversion is successful") {
			
				CHECK(Cast<stype>(i)==i);
			
			}
		
		}
		
		AND_WHEN("The integer is zero") {
		
			utype i=0;
			
			THEN("The conversion is successful") {
			
				CHECK(Cast<stype>(i)==i);
			
			}
		
		}
	
	}
	
	WHEN("An integer of signed type is converted to an integer of equal width and of unsigned type") {
	
		typedef int stype;
		typedef unsigned int utype;
		
		AND_WHEN("The integer is zero") {
		
			stype i=0;
			
			THEN("The conversion is successful") {
			
				CHECK(Cast<utype>(i)==i);
			
			}
		
		}
		
		AND_WHEN("The integer is positive") {
		
			stype i=std::numeric_limits<int>::max();
			
			THEN("The conversion is successful") {
			
				CHECK(Cast<utype>(i)==i);
			
			}
		
		}
		
		AND_WHEN("The integer is negative") {
		
			stype i=-1;
			
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(Cast<utype>(i),std::overflow_error);
			
			}
		
		}
	
	}

	//	WIDENING
	
	WHEN("An integer of unsigned type is converted to an unsigned integer of greater width") {
	
		typedef std::uint8_t stype;
		typedef std::uint16_t wtype;
		
		AND_WHEN("The integer is zero") {
		
			stype i=0;
			
			THEN("The conversion is successful") {
			
				CHECK(Cast<wtype>(i)==i);
			
			}
		
		}
		
		AND_WHEN("The integer is positive") {
		
			stype i=std::numeric_limits<stype>::max();
			
			THEN("The conversion is successful") {
			
				CHECK(Cast<wtype>(i)==i);
			
			}
		
		}
	
	}
	
	WHEN("An integer of signed type is converted to a signed integer of greater width") {
	
		typedef std::int8_t stype;
		typedef std::int16_t wtype;
		
		AND_WHEN("The integer is zero") {
		
			stype i=0;
			
			THEN("The conversion is successful") {
			
				CHECK(Cast<wtype>(i)==i);
			
			}
		
		}
		
		AND_WHEN("The integer is positive") {
		
			stype i=std::numeric_limits<stype>::max();
			
			THEN("The conversion is successful") {
			
				CHECK(Cast<wtype>(i)==i);
			
			}
		
		}
		
		AND_WHEN("The integer is negative") {
		
			stype i=std::numeric_limits<stype>::min();
			
			THEN("The conversion is successful") {
			
				CHECK(Cast<wtype>(i)==i);
			
			}
		
		}
	
	}
	
	WHEN("An integer of unsigned type is converted to a signed integer of greater width") {
	
		typedef std::int16_t type;
		typedef std::uint8_t stype;
		
		AND_WHEN("The integer is zero") {
		
			stype i=0;
			
			THEN("The conversion is successful") {
			
				CHECK(Cast<type>(i)==i);
			
			}
		
		}
		
		AND_WHEN("The integer is small and positive") {
		
			stype i=1;
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK(Cast<type>(i)==i);
			
			}
		
		}
		
		AND_WHEN("The integer is large and positive") {
		
			stype i=std::numeric_limits<std::uint8_t>::max();
			
			THEN("The safe integer is constructed successfully") {
			
				CHECK(Cast<type>(i)==i);
			
			}
		
		}
	
	}
	
	WHEN("An integer of signed type is converted to an unsigned integer of greater width") {
	
		typedef std::uint16_t type;
		typedef std::int8_t stype;
		
		AND_WHEN("The integer is zero") {
		
			stype i=0;
			
			THEN("The conversion is successful") {
			
				CHECK(Cast<type>(i)==i);
			
			}
		
		}
		
		AND_WHEN("The integer is small and positive") {
		
			stype i=1;
			
			THEN("The conversion is successful") {
			
				CHECK(Cast<type>(i)==i);
			
			}
		
		}
		
		AND_WHEN("The integer is large and positive") {
		
			auto i=std::numeric_limits<stype>::max();
			
			THEN("The conversion is successful") {
			
				CHECK(Cast<type>(i)==i);
			
			}
		
		}
		
		AND_WHEN("The integer is negative") {
		
			stype i=-1;
			
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(Cast<type>(i),std::overflow_error);
			
			}
		
		}
	
	}
	
	//	NARROWING
	
	WHEN("An integer of unsigned type is converted to an unsigned type of lesser width") {
	
		typedef std::uint16_t stype;
		typedef std::uint8_t type;
		
		AND_WHEN("The integer is zero") {
		
			stype i=0;
			
			THEN("The conversion is successful") {
			
				CHECK(Cast<type>(i)==i);
				
			}
		
		}
		
		AND_WHEN("The integer is positive") {
		
			auto i=static_cast<stype>(std::numeric_limits<type>::max());
			
			THEN("The conversion is successful") {
			
				CHECK(Cast<type>(i)==i);
				
			}
		
		}
		
		AND_WHEN("The integer is beyond the range of the destination type") {
		
			auto i=std::numeric_limits<stype>::max();
			
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(Cast<type>(i),std::overflow_error);
			
			}
		
		}
	
	}
	
	WHEN("A safe integer of signed type is converted to a signed type of lesser width") {
	
		typedef std::int16_t stype;
		typedef std::int8_t type;
		
		AND_WHEN("The integer is zero") {
		
			stype i=0;
			
			THEN("The conversion is successful") {
			
				CHECK(Cast<type>(i)==i);
				
			}
		
		}
		
		AND_WHEN("The integer is positive") {
		
			auto i=static_cast<stype>(std::numeric_limits<type>::max());
			
			THEN("The conversion is successful") {
			
				CHECK(Cast<type>(i)==i);
				
			}
		
		}
		
		AND_WHEN("The integer is positive and beyond the range of the destination type") {
		
			stype i=std::numeric_limits<stype>::max();
			
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(Cast<type>(i),std::overflow_error);
			
			}
		
		}
		
		AND_WHEN("The integer is negative") {
		
			auto i=static_cast<stype>(std::numeric_limits<type>::min());
			
			THEN("The conversion is successful") {
			
				CHECK(Cast<type>(i)==i);
				
			}
		
		}
		
		AND_WHEN("The integer is negative and beyond the range of the destination type") {
		
			auto i=std::numeric_limits<stype>::min();
			
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(Cast<type>(i),std::overflow_error);
			
			}
		
		}
	
	}
	
	WHEN("A safe integer of unsigned type is converted to a signed integer of lesser width") {
	
		typedef std::uint16_t stype;
		typedef std::int8_t type;
		
		AND_WHEN("The integer is zero") {
		
			stype i=0;
			
			THEN("The conversion is successful") {
			
				CHECK(Cast<type>(i)==i);
				
			}
		
		}
		
		AND_WHEN("The integer is positive") {
		
			auto i=static_cast<stype>(std::numeric_limits<type>::max());
			
			THEN("The conversion is successful") {
			
				CHECK(Cast<type>(i)==i);
				
			}
		
		}
		
		AND_WHEN("The integer is positive and beyond the range of the destination type") {
		
			auto i=std::numeric_limits<stype>::max();
			
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(Cast<type>(i),std::overflow_error);
			
			}
		
		}
	
	}
	
	WHEN("A safe integer of signed type is converted to an unsigned type of lesser width") {
	
		typedef std::int16_t stype;
		typedef std::uint8_t type;
		
		AND_WHEN("The integer is zero") {
		
			stype i=0;
			
			THEN("The conversion is successful") {
			
				CHECK(Cast<type>(i)==i);
				
			}
		
		}
		
		AND_WHEN("The integer is positive") {
		
			stype i=1;
			
			THEN("The conversion is successful") {
			
				CHECK(Cast<type>(i)==i);
				
			}
		
		}
		
		AND_WHEN("The integer is positive and beyond the range of the destination type") {
		
			auto i=static_cast<stype>(std::numeric_limits<type>::max())+1;
			
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(Cast<type>(i),std::overflow_error);
			
			}
		
		}
		
		AND_WHEN("The integer is negative") {
		
			stype i=-1;
			
			THEN("An exception is thrown") {
			
				REQUIRE_THROWS_AS(Cast<type>(i),std::overflow_error);
			
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


SCENARIO("Safe integers may be added and add-assigned to other safe integers, or integers, either on the right or the left") {

	GIVEN("A safe integer") {
	
		int a=1;
		Integer<int> s(a);
		
		GIVEN("A safe integer, such that the addition of the former and the latter does not overflow") {
		
			int b=2;
			Integer<int> i(b);
			int r=a+b;
			
			THEN("The former may be added to the latter") {
			
				CHECK((i+s)==r);
				CHECK((i==b));
				CHECK((s==a));
			
			}
			
			THEN("The latter may be added to the former") {
			
				CHECK((s+i)==r);
				CHECK((i==b));
				CHECK((s==a));
			
			}
			
			THEN("The former may be add-assigned to the latter") {
			
				CHECK((i+=s)==r);
				CHECK((i==r));
				CHECK((s==a));
			
			}
			
			THEN("The latter may be add-assigned to the former") {
			
				CHECK((s+=i)==r);
				CHECK((s==r));
				CHECK((i==b));
			
			}
		
		}
		
		GIVEN("A safe integer, such that the addition of the former and the latter overflows") {
		
			auto b=std::numeric_limits<int>::max();
			Integer<int> i(b);
			
			THEN("Adding the former to the latter raises an exception") {
			
				REQUIRE_THROWS_AS(s+i,std::overflow_error);
				CHECK((s==a));
				CHECK((i==b));
			
			}
			
			THEN("Adding the latter to the former raises an exception") {
			
				REQUIRE_THROWS_AS(i+s,std::overflow_error);
				CHECK((s==a));
				CHECK((i==b));
			
			}
			
			THEN("Add-assigning the former to the latter raises an exception") {
			
				REQUIRE_THROWS_AS(s+=i,std::overflow_error);
				CHECK((s==a));
				CHECK((i==b));
			
			}
			
			THEN("Add-assigning the latter to the former raises an exception") {
			
				REQUIRE_THROWS_AS(i+=s,std::overflow_error);
				CHECK((s==a));
				CHECK((i==b));
			
			}
		
		}
		
		GIVEN("An integer, such that the addition of the former and the latter does not overflow") {
		
			int b=2;
			int i=b;
			int r=a+b;
			
			THEN("The former may be added to the latter") {
			
				CHECK((i+s)==r);
				CHECK((i==b));
				CHECK((s==a));
			
			}
			
			THEN("The latter may be added to the former") {
			
				CHECK((s+i)==r);
				CHECK((i==b));
				CHECK((s==a));
			
			}
			
			THEN("The former may be add-assigned to the latter") {
			
				CHECK((i+=s)==r);
				CHECK((i==r));
				CHECK((s==a));
			
			}
			
			THEN("The latter may be add-assigned to the former") {
			
				CHECK((s+=i)==r);
				CHECK((s==r));
				CHECK((i==b));
			
			}
		
		}
		
		GIVEN("An integer, such that the addition of the former and the latter overflows") {
		
			auto b=std::numeric_limits<int>::max();
			int i=b;
			
			THEN("Adding the former to the latter raises an exception") {
			
				REQUIRE_THROWS_AS(s+i,std::overflow_error);
				CHECK((s==a));
				CHECK((i==b));
			
			}
			
			THEN("Adding the latter to the former raises an exception") {
			
				REQUIRE_THROWS_AS(i+s,std::overflow_error);
				CHECK((s==a));
				CHECK((i==b));
			
			}
			
			THEN("Add-assigning the former to the latter raises an exception") {
			
				REQUIRE_THROWS_AS(s+=i,std::overflow_error);
				CHECK((s==a));
				CHECK((i==b));
			
			}
			
			THEN("Add-assigning the latter to the former raises an exception") {
			
				REQUIRE_THROWS_AS(i+=s,std::overflow_error);
				CHECK((s==a));
				CHECK((i==b));
			
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


SCENARIO("Safe integers and integers may be subtracted and subtract-assigned from other safe integers, and safe integers may be subtracted and subtract-assignedfrom integers") {

	GIVEN("A safe integer") {
	
		auto a=1;
		Integer<int> s(a);
		
		GIVEN("A safe integer, such that the subtraction of the former from the latter, and the latter from the former do not overflow") {
		
			int b=3;
			Integer<int> i(b);
			int rs=a-b;
			int ri=b-a;
			
			THEN("The latter may be subtracted from the former") {
			
				CHECK((i-s)==ri);
				CHECK((i==b));
				CHECK((s==a));
			
			}
			
			THEN("The former may be subtracted from the latter") {
			
				CHECK((s-i)==rs);
				CHECK((i==b));
				CHECK((s==a));
			
			}
			
			THEN("The latter may be subtract-assigned from the former") {
			
				CHECK((i-=s)==ri);
				CHECK((i==ri));
				CHECK((s==a));
			
			}
			
			THEN("The former may be subtract-assigned from the latter") {
			
				CHECK((s-=i)==rs);
				CHECK((i==b));
				CHECK((s==rs));
			
			}
		
		}
		
		GIVEN("A safe integer, such that subtraction overflows") {
		
			auto b=std::numeric_limits<int>::min();
			Integer<int> i(b);
			
			THEN("When the latter is subtracted from the former, an exception is thrown") {
			
				REQUIRE_THROWS_AS(i-s,std::overflow_error);
				CHECK((i==b));
				CHECK((s==a));
			
			}
			
			THEN("When the former is subtracted from the latter, an exception is thrown") {
			
				REQUIRE_THROWS_AS(s-i,std::overflow_error);
				CHECK((i==b));
				CHECK((s==a));
			
			}
			
			THEN("When the latter is subtract-assigned from the former, an exception is thrown") {
			
				REQUIRE_THROWS_AS(i-=s,std::overflow_error);
				CHECK((i==b));
				CHECK((s==a));
			
			}
			
			THEN("When the former is subtract-assigned from the latter, an exception is thrown") {
			
				REQUIRE_THROWS_AS(s-=i,std::overflow_error);
				CHECK((i==b));
				CHECK((s==a));
			
			}
		
		}
		
		GIVEN("An integer, such that the subtraction of the former from the latter, and the latter from the former do not overflow") {
		
			int b=3;
			int i=b;
			int rs=a-b;
			int ri=b-a;
			
			THEN("The latter may be subtracted from the former") {
			
				CHECK((i-s)==ri);
				CHECK((i==b));
				CHECK((s==a));
			
			}
			
			THEN("The former may be subtracted from the latter") {
			
				CHECK((s-i)==rs);
				CHECK((i==b));
				CHECK((s==a));
			
			}
			
			THEN("The latter may be subtract-assigned from the former") {
			
				CHECK((i-=s)==ri);
				CHECK((i==ri));
				CHECK((s==a));
			
			}
			
			THEN("The former may be subtract-assigned from the latter") {
			
				CHECK((s-=i)==rs);
				CHECK((i==b));
				CHECK((s==rs));
			
			}
		
		}
		
		GIVEN("An integer, such that subtraction overflows") {
		
			auto b=std::numeric_limits<int>::min();
			int i=b;
			
			THEN("When the latter is subtracted from the former, an exception is thrown") {
			
				REQUIRE_THROWS_AS(i-s,std::overflow_error);
				CHECK((i==b));
				CHECK((s==a));
			
			}
			
			THEN("When the former is subtracted from the latter, an exception is thrown") {
			
				REQUIRE_THROWS_AS(s-i,std::overflow_error);
				CHECK((i==b));
				CHECK((s==a));
			
			}
			
			THEN("When the latter is subtract-assigned from the former, an exception is thrown") {
			
				REQUIRE_THROWS_AS(i-=s,std::overflow_error);
				CHECK((i==b));
				CHECK((s==a));
			
			}
			
			THEN("When the former is subtract-assigned from the latter, an exception is thrown") {
			
				REQUIRE_THROWS_AS(s-=i,std::overflow_error);
				CHECK((i==b));
				CHECK((s==a));
			
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


SCENARIO("Safe integers may be multiplied and multiply-assigned by other safe integers, or integers, either on the right or the left") {

	GIVEN("A safe integer") {
	
		int a=2;
		Integer<int> s(a);
		
		GIVEN("A safe integer, such that the multiplication of the former and the latter does not overflow") {
		
			int b=2;
			Integer<int> i(b);
			int r=a*b;
			
			THEN("The latter may be multiplied by the former") {
			
				CHECK((i*s)==r);
				CHECK((i==b));
				CHECK((s==a));
			
			}
			
			THEN("The former may be multiplied by the latter") {
			
				CHECK((s*i)==r);
				CHECK((i==b));
				CHECK((s==a));
			
			}
			
			THEN("The latter may be multiply-assigned by the former") {
			
				CHECK((i*=s)==r);
				CHECK((i==r));
				CHECK((s==a));
			
			}
			
			THEN("The former may be multiply-assigned by the latter") {
			
				CHECK((s*=i)==r);
				CHECK((s==r));
				CHECK((i==b));
			
			}
		
		}
		
		GIVEN("A safe integer, such that the multiplication of the former and the latter overflows") {
		
			int b=std::numeric_limits<int>::max();
			Integer<int> i(b);
			
			THEN("When the latter is multiplied by the former, an exception is thrown") {
			
				REQUIRE_THROWS_AS(i*s,std::overflow_error);
				CHECK((i==b));
				CHECK((s==a));
			
			}
			
			THEN("When the former is multiplied by the latter, an exception is thrown") {
			
				REQUIRE_THROWS_AS(s*i,std::overflow_error);
				CHECK((i==b));
				CHECK((s==a));
			
			}
			
			THEN("When the latter is multiply-assigned by the former, an exception is thrown") {
			
				REQUIRE_THROWS_AS(i*=s,std::overflow_error);
				CHECK((i==b));
				CHECK((s==a));
			
			}
			
			THEN("When the former is multiply-assigned by the latter, an exception is thrown") {
			
				REQUIRE_THROWS_AS(s*=i,std::overflow_error);
				CHECK((s==a));
				CHECK((i==b));
			
			}
		
		}
		
		GIVEN("An integer, such that the multiplication of the former and the latter does not overflow") {
		
			int b=2;
			int i=b;
			int r=a*b;
			
			THEN("The latter may be multiplied by the former") {
			
				CHECK((i*s)==r);
				CHECK((i==b));
				CHECK((s==a));
			
			}
			
			THEN("The former may be multiplied by the latter") {
			
				CHECK((s*i)==r);
				CHECK((i==b));
				CHECK((s==a));
			
			}
			
			THEN("The latter may be multiply-assigned by the former") {
			
				CHECK((i*=s)==r);
				CHECK((i==r));
				CHECK((s==a));
			
			}
			
			THEN("The former may be multiply-assigned by the latter") {
			
				CHECK((s*=i)==r);
				CHECK((s==r));
				CHECK((i==b));
			
			}
		
		}
		
		GIVEN("An integer, such that the multiplication of the former and the latter overflows") {
		
			int b=std::numeric_limits<int>::max();
			int i=b;
			
			THEN("When the latter is multiplied by the former, an exception is thrown") {
			
				REQUIRE_THROWS_AS(i*s,std::overflow_error);
				CHECK((i==b));
				CHECK((s==a));
			
			}
			
			THEN("When the former is multiplied by the latter, an exception is thrown") {
			
				REQUIRE_THROWS_AS(s*i,std::overflow_error);
				CHECK((i==b));
				CHECK((s==a));
			
			}
			
			THEN("When the latter is multiply-assigned by the former, an exception is thrown") {
			
				REQUIRE_THROWS_AS(i*=s,std::overflow_error);
				CHECK((i==b));
				CHECK((s==a));
			
			}
			
			THEN("When the former is multiply-assigned by the latter, an exception is thrown") {
			
				REQUIRE_THROWS_AS(s*=i,std::overflow_error);
				CHECK((s==a));
				CHECK((i==b));
			
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


SCENARIO("Safe integers may be divided and divide-assigned by other safe integers, or integers, and integers may be divided and divide-assigned by safe integers") {

	GIVEN("A safe integer") {
	
		int a=10;
		Integer<int> s(a);
		
		GIVEN("A safe integer") {
		
			int b=2;
			Integer<int> i(b);
			int q=a/b;
			int r=a%b;
			
			THEN("They may be divided") {
			
				CHECK((s/i)==q);
				CHECK((b==i));
				CHECK((a==s));
			
			}
			
			THEN("Their remainder may be found") {
			
				CHECK((s%i)==r);
				CHECK((b==i));
				CHECK((a==s));
			
			}
			
			THEN("They may be divide-assigned") {
			
				CHECK((s/=i)==q);
				CHECK((s==q));
				CHECK((i==b));
			
			}
			
			THEN("They may be modulus-assigned") {
			
				CHECK((s%=i)==r);
				CHECK((s==r));
				CHECK((i==b));
			
			}
		
		}
		
		GIVEN("A safe integer by which division overflows") {
		
			Integer<int> i(0);
			
			THEN("Division throws an exception") {
			
				REQUIRE_THROWS_AS(s/i,std::overflow_error);
				CHECK((0==i));
				CHECK((a==s));
			
			}
			
			THEN("Modulus throws an exception") {
			
				REQUIRE_THROWS_AS(s%i,std::overflow_error);
				CHECK((0==i));
				CHECK((a==s));
			
			}
			
			THEN("Divide-assignment throws an exception") {
			
				REQUIRE_THROWS_AS(s/=i,std::overflow_error);
				CHECK((0==i));
				CHECK((a==s));
			
			}
			
			THEN("Modulus-assignment throws an exception") {
			
				REQUIRE_THROWS_AS(s%=i,std::overflow_error);
				CHECK((0==i));
				CHECK((a==s));
			
			}
		
		}
		
		GIVEN("An integer") {
		
			int b=2;
			int i=b;
			int q=a/b;
			int r=a%b;
			int ql=b/a;
			int rl=b%a;
			
			THEN("They may be divided with the integer on the right") {
			
				CHECK((s/i)==q);
				CHECK((b==i));
				CHECK((a==s));
			
			}
			
			THEN("Their remainder may be found with the integer on the right") {
			
				CHECK((s%i)==r);
				CHECK((b==i));
				CHECK((a==s));
			
			}
			
			THEN("They may be divide-assigned with the integer on the right") {
			
				CHECK((s/=i)==q);
				CHECK((s==q));
				CHECK((i==b));
			
			}
			
			THEN("They may be modulus-assigned with the integer on the right") {
			
				CHECK((s%=i)==r);
				CHECK((s==r));
				CHECK((i==b));
			
			}
			
			THEN("They may be divided with the integer on the left") {
			
				CHECK((i/s)==ql);
				CHECK((b==i));
				CHECK((a==s));
			
			}
			
			THEN("Their remainder may be found with the integer on the left") {
			
				CHECK((i%s)==rl);
				CHECK((b==i));
				CHECK((a==s));
			
			}
			
			THEN("They may be divide-assigned with the integer on the left") {
			
				CHECK((i/=s)==ql);
				CHECK((s==a));
				CHECK((i==ql));
			
			}
			
			THEN("They may be modulus-assigned with the integer on the left") {
			
				CHECK((i%=s)==rl);
				CHECK((s==a));
				CHECK((i==rl));
			
			}
		
		}
		
		GIVEN("An integer by which division overflows") {
		
			int i=0;
			
			THEN("Division throws an exception") {
			
				REQUIRE_THROWS_AS(s/i,std::overflow_error);
				CHECK((0==i));
				CHECK((a==s));
			
			}
			
			THEN("Modulus throws an exception") {
			
				REQUIRE_THROWS_AS(s%i,std::overflow_error);
				CHECK((0==i));
				CHECK((a==s));
			
			}
			
			THEN("Divide-assignment throws an exception") {
			
				REQUIRE_THROWS_AS(s/=i,std::overflow_error);
				CHECK((0==i));
				CHECK((a==s));
			
			}
			
			THEN("Modulus-assignment throws an exception") {
			
				REQUIRE_THROWS_AS(s%=i,std::overflow_error);
				CHECK((0==i));
				CHECK((a==s));
			
			}
		
		}
	
	}
	
	GIVEN("A safe integer with a value of zero") {
	
		Integer<int> s(0);
		
		GIVEN("An integer") {
		
			int a=2;
			int i=a;
			
			THEN("Division throws an exception") {
			
				REQUIRE_THROWS_AS(i/s,std::overflow_error);
				CHECK((a==i));
				CHECK((0==s));
			
			}
			
			THEN("Modulus throws an exception") {
			
				REQUIRE_THROWS_AS(i%s,std::overflow_error);
				CHECK((a==i));
				CHECK((0==s));
			
			}
			
			THEN("Divide-assignment throws an exception") {
			
				REQUIRE_THROWS_AS(i/=s,std::overflow_error);
				CHECK((a==i));
				CHECK((0==s));
			
			}
			
			THEN("Modulus-assignment throws an exception") {
			
				REQUIRE_THROWS_AS(i%=s,std::overflow_error);
				CHECK((a==i));
				CHECK((0==s));
			
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


SCENARIO("Safe integers may be hashed") {

	GIVEN("A safe integer") {
	
		Integer<unsigned int> s(1);
		
		GIVEN("An integer of identical value and type") {
		
			unsigned int i=1;
			
			WHEN("They are hashed") {
			
				THEN("Their hashes are the same") {
				
					CHECK(Hash(s)==Hash(i));
				
				}
			
			}
		
		}
		
		GIVEN("An integer of different value and identical type") {
		
			unsigned int i=2;
			
			WHEN("They are hashed") {
			
				THEN("Their hashes are not the same") {
				
					CHECK(Hash(s)!=Hash(i));
				
				}
			
			}
		
		}
	
	}

}


SCENARIO("The signedness of safe integer types may be determined") {

	GIVEN("An unsigned safe integer type") {
	
		typedef Integer<unsigned int> type;
		
		THEN("It is identified as being unsigned") {
		
			CHECK(std::is_unsigned<type>::value);
			CHECK(type::Unsigned);
		
		}
		
		THEN("It is not identified as being signed") {
		
			CHECK(!std::is_signed<type>::value);
			CHECK(!type::Signed);
		
		}
	
	}
	
	GIVEN("A signed safe integer type") {
	
		typedef Integer<int> type;
		
		THEN("It is not identified as being unsigned") {
		
			CHECK(!std::is_unsigned<type>::value);
			CHECK(!type::Unsigned);
		
		}
		
		THEN("It is identified as being signed") {
		
			CHECK(std::is_signed<type>::value);
			CHECK(type::Signed);
		
		}
	
	}

}


SCENARIO("Safe integer types may be converted to signed or unsigned") {

	GIVEN("An unsigned safe integer type") {
	
		typedef Integer<unsigned int> type;
		
		THEN("Its unsigned type is the same as its type") {
		
			auto value=std::is_same<type::Type,type::UnsignedType>::value;
			CHECK(value);
		
		}
		
		THEN("Converting it to unsigned yields the same type") {
		
			auto value=std::is_same<type,std::make_unsigned<type>::type>::value;
			CHECK(value);
		
		}
		
		GIVEN("A safe integer of equivalent signed type") {
		
			typedef Integer<int> stype;
			
			THEN("The unsigned type of the former is the same as the unsigned type of the latter") {
			
				auto value=std::is_same<type::UnsignedType,stype::UnsignedType>::value;
				CHECK(value);
			
			}
			
			THEN("The signed type of the former is the same as the signed type of the latter") {
			
				auto value=std::is_same<type::SignedType,stype::SignedType>::value;
				CHECK(value);
			
			}
			
			THEN("Converting the former to signed yields the latter") {
			
				auto value=std::is_same<std::make_signed<type>::type,stype>::value;
				CHECK(value);
			
			}
			
			THEN("Converting the latter to unsigned yields the former") {
			
				auto value=std::is_same<std::make_unsigned<stype>::type,type>::value;
				CHECK(value);
			
			}
		
		}
	
	}
	
	GIVEN("A signed safe integer type") {
	
		typedef Integer<int> type;
		
		THEN("Its signed type is the same as its type") {
		
			auto value=std::is_same<type::Type,type::SignedType>::value;
			CHECK(value);
		
		}
		
		THEN("Converting it to signed yields the same type") {
		
			auto value=std::is_same<type,std::make_signed<type>::type>::value;
			CHECK(value);
		
		}
	
	}

}
