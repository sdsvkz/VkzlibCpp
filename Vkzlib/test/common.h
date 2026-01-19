#ifndef VKZLIB_TEST_COMMON_H
#define VKZLIB_TEST_COMMON_H

#include <cstdio>
#include <typeinfo>
#include <type_traits>

template<typename T>
void printType(const char *name = "T") {
	std::printf("%s = %s\n", name, typeid(T).name());
}

template<typename T>
void _printOne(int index) {
	std::printf("%d: %s\n", index, typeid(T).name());
}

template<std::size_t N>
void _printOne(int index, std::integral_constant<std::size_t, N>) {
	std::printf("%d: %zu\n", index, N);
}

template<typename T>
void _printDispatch(int index) {
	_printOne<T>(index);
}

template<typename T, T V>
void _printDispatch(int index) {
	_printOne(index, std::integral_constant<T, V>{});
}

template<typename... Ts>
void printAllTypes() {
	int i = 0;
	(_printDispatch<Ts>(i++), ...);
}

// `TRUE` or `FALSE`
#define EXPECT_TEMPLATE(BOOL, Concept, ...)							\
	do {															\
		constexpr bool e = Concept<__VA_ARGS__>;					\
		std::printf("\n" #Concept " with template parameters: \n");	\
		printAllTypes<__VA_ARGS__>();								\
		std::printf("Evaluted to %s\n", e ? "true" : "false");		\
		EXPECT_##BOOL(e);											\
	} while (false);

#define POINTER_OF_MEMBER(ClassName, MemberTuple) \
	decltype(&ClassName::VKZLIB_PP_UNPACK_TUPLE MemberTuple)

#endif //VKZLIB_TEST_COMMON_H