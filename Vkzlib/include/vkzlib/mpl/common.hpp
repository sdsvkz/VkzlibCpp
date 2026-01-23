#ifndef VKZLIB_MPL_COMMON_HPP
#define VKZLIB_MPL_COMMON_HPP

/**
 * @brief Metaprogramming Library
 */
namespace vkz::mpl {}

#include <vkzlib/mpl/common/AnyOf.hpp>
#include <vkzlib/mpl/common/Class.hpp>
#include <vkzlib/mpl/common/conditionalCall.hpp>
#include <vkzlib/mpl/common/CString.hpp>
#include <vkzlib/mpl/common/DefaultPack.hpp>
#include <vkzlib/mpl/common/dup_const.hpp>
#include <vkzlib/mpl/common/dup_cvref.hpp>
#include <vkzlib/mpl/common/dup_reference.hpp>
#include <vkzlib/mpl/common/dup_volatile.hpp>
#include <vkzlib/mpl/common/HasCommonType.hpp>
#include <vkzlib/mpl/common/Homogeneous.hpp>
#include <vkzlib/mpl/common/NPOS.hpp>
#include <vkzlib/mpl/common/Size.hpp>

#include <vkzlib/mpl/common/ce/findFirstFor.hpp>
#include <vkzlib/mpl/common/ce/foldl.hpp>
#include <vkzlib/mpl/common/ce/min.hpp>
#include <vkzlib/mpl/common/ce/unroll.hpp>

#include <vkzlib/mpl/common/pack/is_empty_pack.hpp>
#include <vkzlib/mpl/common/pack/nth_of.hpp>
#include <vkzlib/mpl/common/pack/Pack.hpp>

#include <vkzlib/mpl/common/pack/unsafe/nth_of.hpp>
#include <vkzlib/mpl/common/pack/unsafe/uncons.hpp>

#include <vkzlib/mpl/common/tpl/fst/parse_template_spec.hpp>
#include <vkzlib/mpl/common/tpl/fst/WithTemplateParams.hpp>

#endif // VKZLIB_MPL_COMMON_HPP
