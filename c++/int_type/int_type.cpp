#include <cstdint> // u?int\d+_t

#pragma region _int_type_lookup
template<unsigned int size, bool issigned>
struct _int_type_lookup{
	constexpr static bool is_valid = false;
};
template<typename T>
struct _reverse_int_type_lookup{
	constexpr static bool is_valid = false;
};

#define INT_TYPE_LOOKUP_SPECIALIZATION(size_, issigned_, type_) \
template<> \
struct _int_type_lookup<size_, issigned_> { \
	using type = type_; \
	constexpr static bool is_valid = true; \
}; \
template<> \
struct _reverse_int_type_lookup<type_> { \
	constexpr static bool is_valid = true; \
	constexpr static unsigned int size = size_; \
	constexpr static bool is_signed = issigned_; \
};

#define INT_TYPE_LOOKUP_SPECIALIZATION_PAIR(size) \
INT_TYPE_LOOKUP_SPECIALIZATION(size, false, uint ## size ## _t ) \
INT_TYPE_LOOKUP_SPECIALIZATION(size, true, int ## size ## _t )

INT_TYPE_LOOKUP_SPECIALIZATION_PAIR(8)
INT_TYPE_LOOKUP_SPECIALIZATION_PAIR(16)
INT_TYPE_LOOKUP_SPECIALIZATION_PAIR(32)
INT_TYPE_LOOKUP_SPECIALIZATION_PAIR(64)

#undef INT_TYPE_SPECIALIZATION_PAIR
#undef INT_TYPE_SPECIALIZATION
#pragma endregion


#pragma region int_type
template<unsigned int size, bool issigned>
class int_type{
private:
	using _int_type = _int_type_lookup<size, issigned>;
public:
	static_assert(_int_type::is_valid, "found no int type with those properties"); // TODO give better message
	using type = typename _int_type::type;
};

template<unsigned int size>
using unsigned_int_type = int_type<size, false>;

template<unsigned int size>
using signed_int_type = int_type<size, true>;
#pragma endregion
