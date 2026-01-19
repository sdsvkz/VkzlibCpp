#ifndef VKZLIB_MPL_PREPROCESSOR_UNPACK_TUPLE_HPP
#define VKZLIB_MPL_PREPROCESSOR_UNPACK_TUPLE_HPP

/**
 * @brief Helper to remove parenthesis, `(typename T, typename U)` -> `typename T, typename U`
 */
#define VKZLIB_PP_UNPACK_TUPLE(...) __VA_ARGS__

#endif // VKZLIB_MPL_PREPROCESSOR_UNPACK_TUPLE_HPP