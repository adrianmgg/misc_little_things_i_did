#pragma once
#ifndef _AMGG_CONSTANTS
#define _AMGG_CONSTANTS

namespace amgg{
	namespace constants{
		template<typename T>
		constexpr T PI = T(3.1415926535897932384626433832795028841971693993751L);
		template<typename T>
		constexpr T TWO_PI = T(6.2831853071795864769252867665590057683943387987502L);
		template<typename T>
		constexpr T TAU = TWO_PI<T>;
	}
}

#endif