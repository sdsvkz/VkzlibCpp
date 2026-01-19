#ifndef VKZLIB_MPL_COMMON_CSTRING_HPP
#define VKZLIB_MPL_COMMON_CSTRING_HPP

#include <concepts>

namespace vkz::mpl {
    /**
    * @brief C-style String
    */
    template<typename CharT, typename NormalCharT = std::decay_t<std::remove_cvref_t<CharT> > >
    concept CString =
            std::same_as<NormalCharT, char *> &&
            std::same_as<NormalCharT, signed char *> &&
            std::same_as<NormalCharT, unsigned char *> &&
            std::same_as<NormalCharT, wchar_t *> &&
            std::same_as<NormalCharT, char8_t *> &&
            std::same_as<NormalCharT, char16_t *> &&
            std::same_as<NormalCharT, char32_t *>;
}

#endif // VKZLIB_MPL_COMMON_CSTRING_HPP