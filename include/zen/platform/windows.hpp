// Copyright (c) 2025 - 2026, neonbyte - All rights reserved.
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
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
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

#include <zen/core/xors.hpp>
#include <zen/platform/rtl/api_set_schema.hpp>
#include <zen/platform/version_info.hpp>
#include <functional>

namespace zen {
namespace win {
NODISCARD
auto
to_wide(
    std::string_view input
) noexcept -> std::wstring;

NODISCARD
auto
to_ansi(
    std::wstring_view input
) noexcept -> std::string;
} //namespace win

namespace detail {
template<class T>
constexpr
auto
add_host(
    T& modules,
    std::wstring&& host
) noexcept -> void
{
    if (!host.empty()) {
        if constexpr (std::is_same_v<std::vector<std::string>, T>) {
            modules.emplace_back(win::to_ansi(host));
        } else {
            modules.emplace_back(std::move(host));
        }
    }
}

NODISCARD
inline
auto
get_wstring(
    const u8* const base,
    const u32       offset,
    const u32       num_bytes
) noexcept -> std::wstring
{
    std::wstring ret;

    if (base && offset && num_bytes) {
        const auto* const ptr = reinterpret_cast<const wchar_t*>(base + offset);
        const szt         len = num_bytes / sizeof(wchar_t);

        ret.assign(ptr, ptr + len);
    }

    return ret;
}
} //namespace detail

namespace win {
using api_set_schema_a = std::unordered_map<std::string, std::vector<std::string>>;
using api_set_schema_w = std::unordered_map<std::wstring, std::vector<std::wstring>>;

NODISCARD
auto
version_info() noexcept -> version_info_t&;

NODISCARD
auto
get_peb() noexcept -> uptr;

NODISCARD
auto
get_pid() noexcept -> u32;

NODISCARD
auto
get_tid() noexcept -> u32;

auto
enum_modules(
    const std::function<bool(std::wstring_view, va_t<>)>& callback
) noexcept -> void;

NODISCARD
auto
get_module_handle(
    u32  name,
    bool lowercase = true
) noexcept -> uptr;

NODISCARD
auto
get_module_handle(
    std::string_view name,
    bool             lowercase = true
) noexcept -> uptr;

NODISCARD
auto
get_module_handle(
    std::wstring_view name,
    bool              lowercase = true
) noexcept -> uptr;

NODISCARD
auto
get_proc_address(
    uptr             handle,
    std::string_view name,
    bool             lowercase = true
) noexcept -> uptr;

NODISCARD
auto
get_proc_address(
    uptr handle,
    u32  name,
    bool lowercase = true
) noexcept -> uptr;

template<class T>
requires(sizeof(T) == sizeof(uptr))
NODISCARD
auto
get_proc_address(
    const uptr             handle,
    const std::string_view name,
    const bool             lowercase = true
) noexcept -> T
{
    if constexpr (std::is_integral_v<T>) {
        return static_cast<T>(get_proc_address(handle, name, lowercase));
    } else {
        return reinterpret_cast<T>(get_proc_address(handle, name, lowercase));
    }
}

template<class T>
requires(sizeof(T) == sizeof(uptr))
NODISCARD
auto
get_proc_address(
    const uptr handle,
    const u32  name,
    const bool lowercase = true
) noexcept -> T
{
    if constexpr (std::is_integral_v<T>) {
        return static_cast<T>(get_proc_address(handle, name, lowercase));
    } else {
        return reinterpret_cast<T>(get_proc_address(handle, name, lowercase));
    }
}

template<class T>
requires(std::is_same_v<std::string, T> || std::is_same_v<std::wstring, T>)
NODISCARD
auto
dump_api_set_schema_v2(
    const void* const api_set_map
) noexcept -> std::conditional_t<std::is_same_v<std::string, T>, api_set_schema_a, api_set_schema_w>
{
    using result_t = std::conditional_t<std::is_same_v<std::string, T>, api_set_schema_a, api_set_schema_w>;

    if (!api_set_map) {
        return result_t{};
    }

    result_t          ret{};
    const auto* const base    = reinterpret_cast<const u8*>(api_set_map);
    const auto* const header  = reinterpret_cast<const u32*>(base); // 0: version, 1: count
    const auto        count   = header[1];
    const auto* const entries = reinterpret_cast<const rtl::api_set_namespace_entry_v2*>(base + sizeof(u32) * 2);

    for (u32 i{}; i < count; ++i) {
        std::vector<T> modules;

        const auto&       e           = entries[i];
        auto              proxy       = detail::get_wstring(base, e.name_offset, e.name_len);
        const auto* const values      = reinterpret_cast<const rtl::api_set_value_entry_v2*>(base + e.data_offset);
        const auto        num_values  = reinterpret_cast<const u32*>(base + e.data_offset)[0];
        const auto* const value_entry = reinterpret_cast<const rtl::api_set_value_entry_v2*>(base + e.data_offset + sizeof(u32));

        for (u32 j{}; j < num_values; ++j) {
            const auto& v    = value_entry[j];
            auto        host = detail::get_wstring(base, v.value_offset, v.value_len);

            detail::add_host(modules, std::move(host));
        }

        if (!modules.empty()) {
            if constexpr (std::is_same_v<std::string, T>) {
                ret.insert(std::make_pair(to_ansi(proxy), std::move(modules)));
            } else {
                ret.insert(std::make_pair(std::move(proxy), std::move(modules)));
            }
        }
    }

    return ret;
}

template<class T>
requires(std::is_same_v<std::string, T> || std::is_same_v<std::wstring, T>)
NODISCARD
auto
dump_api_set_schema_v4(
    const void* const api_set_map
) noexcept -> std::conditional_t<std::is_same_v<std::string, T>, api_set_schema_a, api_set_schema_w>
{
    using result_t = std::conditional_t<std::is_same_v<std::string, T>, api_set_schema_a, api_set_schema_w>;

    if (!api_set_map) {
        return result_t{};
    }

    result_t          ret{};
    const auto* const base    = reinterpret_cast<const uint8_t*>(api_set_map);
    const auto* const header  = reinterpret_cast<const u32*>(base);
    const auto        version = header[0];
    const auto        count   = header[3];
    const auto* const entries = reinterpret_cast<const rtl::api_set_namespace_entry_v4*>(base + sizeof(u32) * 6);

    for (u32 i{}; i < count; ++i) {
        std::vector<T>    modules;
        const auto&       e           = entries[i];
        auto              proxy       = detail::get_wstring(base, e.name_offset, e.name_len);
        auto              alias       = detail::get_wstring(base, e.alias_offset, e.alias_len);
        const auto* const values      = reinterpret_cast<const rtl::api_set_value_entry_v4*>(base + e.data_offset);
        const auto        num_values  = *reinterpret_cast<const u32*>(base + e.data_offset);
        const auto* const value_entry = reinterpret_cast<const rtl::api_set_value_entry_v4*>(base + e.data_offset + sizeof(u32));

        detail::add_host(modules, std::move(alias));

        for (u32 j{}; j < num_values; ++j) {
            const auto& v    = value_entry[j];
            auto        host = detail::get_wstring(base, v.value_offset, v.value_len);

            detail::add_host(modules, std::move(host));
        }

        if (!modules.empty()) {
            if constexpr (std::is_same_v<std::string, T>) {
                ret.insert(std::make_pair(to_ansi(proxy), std::move(modules)));
            } else {
                ret.insert(std::make_pair(std::move(proxy), std::move(modules)));
            }
        }
    }

    return ret;
}

template<class T>
requires(std::is_same_v<std::string, T> || std::is_same_v<std::wstring, T>)
NODISCARD
auto
dump_api_set_schema_v6(
    const void* const api_set_map
) noexcept -> std::conditional_t<std::is_same_v<std::string, T>, api_set_schema_a, api_set_schema_w>
{
    using result_t = std::conditional_t<std::is_same_v<std::string, T>, api_set_schema_a, api_set_schema_w>;

    result_t          ret{};
    const auto* const base       = reinterpret_cast<const uint8_t*>(api_set_map);
    const auto* const ns         = reinterpret_cast<const rtl::api_set_namespace_v6*>(base);
    const auto* const entry_base = reinterpret_cast<const rtl::api_set_namespace_entry_v6*>(base + ns->entry_offset);

    for (u32 i{}; i < ns->count; ++i) {
        std::vector<T>    modules;
        const auto&       e          = entry_base[i];
        auto              proxy      = detail::get_wstring(base, e.name_offset, e.name_len);
        const auto* const value_base = reinterpret_cast<const rtl::api_set_value_entry_v6*>(base + e.value_offset);

        for (u32 j{}; j < e.value_count; ++j) {
            const auto& v = value_base[j];
            auto host     = detail::get_wstring(base, v.value_offset, v.value_len);

            detail::add_host(modules, std::move(host));
        }

        if (!modules.empty()) {
            if constexpr (std::is_same_v<std::string, T>) {
                ret.insert(std::make_pair(to_ansi(proxy), std::move(modules)));
            } else {
                ret.insert(std::make_pair(std::move(proxy), std::move(modules)));
            }
        }
    }

    return ret;
}

template<class T>
requires(std::is_same_v<api_set_schema_a, T> || std::is_same_v<api_set_schema_w, T>)
NODISCARD
auto
resolve_api_schema(
    const T& cache,
    const std::conditional_t<
        std::is_same_v<api_set_schema_a, T>,
        std::string,
        std::wstring
    >& name
) noexcept
-> const std::conditional_t<
    std::is_same_v<api_set_schema_a, T>,
    std::string,
    std::wstring
>*
{
    using key_t   = std::conditional_t<std::is_same_v<api_set_schema_a, T>, std::string, std::wstring>;
    using value_t = key_t;

    const auto it = std::ranges::find_if(cache, [&name](const auto& v) noexcept
    {
        return name.rfind(v.first, 0) == 0;
    });

    if (it == cache.end()) {
        return nullptr;
    }

    for (const auto& host : it->second) {
        if constexpr (std::is_same_v<key_t, std::wstring>) {
            if (host.starts_with(xors(L"api-")) || host.starts_with(xors(L"ext-"))) {
                if (const auto* const r = resolve_api_schema(cache, host)) {
                    return r;
                }
            }
        }
        else {
            if (host.starts_with(xors("api-")) || host.starts_with(xors("ext-"))) {
                if (const auto* const r = resolve_api_schema(cache, host)) {
                    return r;
                }
            }
        }

        return &host;
    }

    return nullptr;
}
template<class T>
requires(std::is_same_v<api_set_schema_a, T> || std::is_same_v<api_set_schema_w, T>)
NODISCARD
auto
resolve_api_schema(
    const T&   cache,
    const u32  name_hash,
    const bool lowercase = true
) noexcept
-> const std::conditional_t<
    std::is_same_v<api_set_schema_a, T>,
    std::string,
    std::wstring
>*
{
    using key_t = std::conditional_t<std::is_same_v<api_set_schema_a, T>, std::string, std::wstring>;

    for (const auto& [key, values] : cache) {
        // ApiSet keys are stored without ".dll"; callers hash the full name including it
        const auto hash = lowercase
            ? fnv<>::hash<true>(".dll", fnv<>::get<true>(key))
            : fnv<>::hash(".dll", fnv<>::get(key));

        if (hash == name_hash) {
            return resolve_api_schema(cache, key);
        }
    }

    return nullptr;
}
} //namespace win
} //namespace zen
