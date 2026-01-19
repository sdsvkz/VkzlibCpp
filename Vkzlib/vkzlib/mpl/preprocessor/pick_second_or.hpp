#ifndef VKZLIB_MPL_PREPROCESSOR_PICK_SECOND_OR_HPP
#define VKZLIB_MPL_PREPROCESSOR_PICK_SECOND_OR_HPP

// Pick second one
#define _VKZLIB_PP_PICK_SECOND_OR_IMPL(X, Y, ...) Y
/**
* @brief
* Pick second one, or X if there is no second one
*
* Combine this with `VKZLIB_PP_CONST` to achieve simple token equality check
*
* See implementation of `VKZLIB_PP_IS_ZERO`
*/
#define VKZLIB_PP_PICK_SECOND_OR(X, ...) _VKZLIB_PP_PICK_SECOND_OR_IMPL(__VA_ARGS__, X)

#endif // VKZLIB_MPL_PREPROCESSOR_PICK_SECOND_OR_HPP