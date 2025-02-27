// Copyright (c) 2025, neonbyte - All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of the project nor the
//    names of its contributors may be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#pragma once

#include <zen/core/definitions.h>
#include <bit>
#include <memory>
#include <string>
#include <string_view>

#if !defined(ZEN_CXX_SUPPORTS)
#   define ZEN_CXX_SUPPORTS(x) __has_cpp_attribute(x)
#endif //ZEN_CXX_SUPPORTS

#if !defined(NODISCARD)
#   if ZEN_CXX_SUPPORTS(nodiscard)
#       define NODISCARD [[nodiscard]]
#   else
#       define NODISCARD
#   endif
#endif //NODISCARD

#if !defined(MAYBE_UNUSED)
#   if ZEN_CXX_SUPPORTS(maybe_unused)
#       define MAYBE_UNUSED [[maybe_unused]]
#   else
#       define MAYBE_UNUSED
#   endif
#endif //MAYBE_UNUSED

#if !defined(ZEN_CXX_SUPPORTS)
#   define ZEN_CXX_SUPPORTS(x) __has_cpp_attribute(x)
#endif //ZEN_CXX_SUPPORTS

#if !defined(ZEN_CXX_LANG)
#   if defined(ZEN_OS_WINDOWS)
#       if !defined(_STL_LANG)
#           if defined(_MSVC_LANG) && _MSVC_LANG > __cplusplus
#               define ZEN_CXX_LANG _MSVC_LANG
#           else
#               define ZEN_CXX_LANG __cplusplus
#           endif
#       else
#           define ZEN_CXX_LANG _STL_LANG
#       endif
#   else
#       define ZEN_CXX_LANG __cplusplus
#   endif
#endif //ZEN_CXX_LANG

#if !defined(ZEN_HAS_CXX20)
#   if ZEN_CXX_LANG > 201703L
#       define ZEN_HAS_CXX20 1
#   else
#       define ZEN_HAS_CXX20 0
#       error "zen requires a compiler that supports at least C++ 20"
#   endif
#endif //ZEN_HAS_CXX20

#if !defined(ZEN_HAS_CXX23)
#   if ZEN_HAS_CXX20 && ZEN_CXX_LANG > 202002L
#       define ZEN_HAS_CXX23 1
#   else
#       define ZEN_HAS_CXX23 0
#       if defined(ZEN_CXX_MSVC) || defined(ZEN_CXX_INTEL)
#           pragma warning "zen would benefit from a compiler that supports at C++ 23"
#       else
#           warning "zen would benefit from a compiler that supports at C++ 23"
#       endif
#   endif
#endif //ZEN_HAS_CXX23

#if !defined(ZEN_CXX23_CONSTEXPR)
#   if ZEN_HAS_CXX23
#       define ZEN_CXX23_CONSTEXPR constexpr
#       define ZEN_CXX23_CONSTEXPR_IMPL constexpr
#   else
#       define ZEN_CXX23_CONSTEXPR
#       define ZEN_CXX23_CONSTEXPR_IMPL inline
#   endif
#endif //ZEN_CXX23_CONSTEXPR

#if !defined(ZEN_CXX_SUPPORTS)
#   define ZEN_CXX_SUPPORTS(x) __has_cpp_attribute(x)
#endif //ZEN_CXX_SUPPORTS

#if !defined(ZEN_CXX_MSVC_SUPPORTS)
#   if defined(ZEN_OS_WINDOWS)
#       if defined(__CUDACC__)
#           define ZEN_CXX_MSVC_SUPPORTS(x) 0
#       else
#           define ZEN_CXX_MSVC_SUPPORTS(x) ZEN_CXX_SUPPORTS(msvc::x)
#       endif
#   else
#       define ZEN_CXX_MSVC_SUPPORTS(x)
#   endif
#endif //ZEN_CXX_MSVC_SUPPORTS

#if !defined(ZEN_CXX_MSVC_INTRINSIC)
#   if ZEN_CXX_MSVC_SUPPORTS(intrinsic)
#       define ZEN_CXX_MSVC_INTRINSIC [[msvc::intrinsic]]
#   else
#       define ZEN_CXX_MSVC_INTRINSIC
#   endif
#endif //ZEN_CXX_MSVC_INTRINSIC

#if !defined(NODISCARD)
#   if ZEN_CXX_SUPPORTS(nodiscard)
#       define NODISCARD [[nodiscard]]
#   else
#       define NODISCARD
#   endif
#endif //NODISCARD

#if !defined(MAYBE_UNUSED)
#   if ZEN_CXX_SUPPORTS(maybe_unused)
#       define MAYBE_UNUSED [[maybe_unused]]
#   else
#       define MAYBE_UNUSED
#   endif
#endif //MAYBE_UNUSED

#if !ZEN_HAS_CXX23
namespace std {
template<zen::enum_type T>
NODISCARD
ZEN_CXX_MSVC_INTRINSIC
constexpr
auto
to_underlying(
    const T value
) noexcept -> std::underlying_type_t<T>
{
    return static_cast<std::underlying_type_t<T>>(value);
}
} //namespace std
#endif

#if !defined(ZEN_ENUM_OPERATORS)
#   define ZEN_ENUM_OPERATORS(EnumClass)              \
        constexpr                                     \
        auto                                          \
        operator~(                                    \
            const EnumClass lhs                       \
        ) noexcept -> EnumClass                       \
        {                                             \
            return static_cast<EnumClass>(            \
                ~std::to_underlying(lhs)              \
            );                                        \
        }                                             \
        constexpr                                     \
        auto                                          \
        operator|(                                    \
            const EnumClass lhs,                      \
            const EnumClass rhs                       \
        ) noexcept -> EnumClass                       \
        {                                             \
            return static_cast<EnumClass>(            \
                std::to_underlying(lhs)               \
                | std::to_underlying(rhs)             \
            );                                        \
        }                                             \
        constexpr                                     \
        auto                                          \
        operator|=(                                   \
            EnumClass&      lhs,                      \
            const EnumClass rhs                       \
        ) noexcept -> EnumClass&                      \
        {                                             \
            auto value = std::to_underlying(lhs);     \
            value |= std::to_underlying(rhs);         \
            lhs = static_cast<EnumClass>(value);      \
            return lhs;                               \
        }                                             \
        constexpr                                     \
        auto                                          \
        operator&(                                    \
            const EnumClass lhs,                      \
            const EnumClass rhs                       \
        ) noexcept -> EnumClass                       \
        {                                             \
            return static_cast<EnumClass>(            \
                std::to_underlying(lhs)               \
                & std::to_underlying(rhs)             \
            );                                        \
        }                                             \
        constexpr                                     \
        auto                                          \
        operator&=(                                   \
            EnumClass&      lhs,                      \
            const EnumClass rhs                       \
        ) noexcept -> EnumClass&                      \
        {                                             \
            auto value = std::to_underlying(lhs);     \
            value &= std::to_underlying(rhs);         \
            lhs = static_cast<EnumClass>(value);      \
            return lhs;                               \
        }                                             \
        constexpr                                     \
        auto                                          \
        operator^(                                    \
            const EnumClass lhs,                      \
            const EnumClass rhs                       \
        ) noexcept -> EnumClass                       \
        {                                             \
            return static_cast<EnumClass>(            \
                std::to_underlying(lhs)               \
                ^ std::to_underlying(rhs)             \
            );                                        \
        }                                             \
        constexpr                                     \
        auto                                          \
        operator^=(                                   \
            EnumClass&      lhs,                      \
            const EnumClass rhs                       \
        ) noexcept -> EnumClass&                      \
        {                                             \
            auto value = std::to_underlying(lhs);     \
            value ^= std::to_underlying(rhs);         \
            lhs = static_cast<EnumClass>(value);      \
            return lhs;                               \
        }
#endif //ZEN_ENUM_OPERATORS

namespace zen {
template<class T>
concept scalar
    = (std::is_arithmetic_v<T> && !std::is_same_v<bool, T>)
    || std::is_floating_point_v<T>
    || std::is_enum_v<T>
#if ZEN_HAS_CXX23
    || std::is_scoped_enum_v<T>
#endif
    ;

template<class T>
concept enum_type
    = std::is_enum_v<T>
#if ZEN_HAS_CXX23
    || std::is_scoped_enum_v<T>
#endif
    ;

namespace detail {
inline constexpr auto is_64_bit        = sizeof(void*) == sizeof(u64);
inline constexpr auto is_little_endian = std::endian::native == std::endian::little;
} //namespace detail

namespace win {
template<std::integral Ret = szt, std::integral T>
NODISCARD
constexpr
auto
convert_alignment(
    const T align_flag
) noexcept -> Ret
{
    if (align_flag == 0) {
        return 16;
    }
    if (align_flag >= 0xF) {
        return 0;
    }
    return static_cast<T>(1ull << (align_flag - 1));
}

template<std::integral Ret = u8, std::integral T>
NODISCARD
constexpr
auto
reflect_alignment(
    const T alignment
) noexcept -> Ret
{
    for (u8 flag{1}; flag != 0xF; ++flag) {
        if (static_cast<szt>(1ull << (flag - 1)) >= static_cast<szt>(alignment)) {
            return static_cast<Ret>(flag);
        }
    }

    return 0;
}

template<class T = void*>
requires(std::is_integral_v<T> || std::is_pointer_v<T>)
NODISCARD
ZEN_CXX23_CONSTEXPR
auto
invalid_handle_value() noexcept -> T
{
    if constexpr (std::is_integral_v<T>) {
        return static_cast<T>(-1);
    } else {
        return reinterpret_cast<T*>(invalid_handle_value<iptr>());
    }
}

template<class T>
requires(std::is_integral_v<T> || std::is_pointer_v<T>)
NODISCARD
ZEN_CXX23_CONSTEXPR
auto
is_valid_handle(
    T handle
) noexcept -> bool
{
    return handle && handle != invalid_handle_value<T>();
}
} //namespace win

template<bool X64 = detail::is_64_bit>
using va_t    = std::conditional_t<X64, u64, u32>;
using long_t  = std::conditional_t<sizeof(long) == sizeof(i32), long, i32>;
using ulong_t = std::conditional_t<sizeof(long) == sizeof(i32), unsigned long, u32>;
} //namespace zen
