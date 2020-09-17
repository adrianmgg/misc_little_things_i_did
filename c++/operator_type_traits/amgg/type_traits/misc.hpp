#pragma once
#ifndef _AMGG_TYPE_TRAITS_MISC
#define _AMGG_TYPE_TRAITS_MISC

#include <type_traits>

namespace amgg{
	// from https://youtu.be/ybaE9qlhHvw?t=1357
	template<bool B>
	using bool_if_t = std::enable_if_t<B, bool>;
}

#endif
