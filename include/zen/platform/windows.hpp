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
template<class T>
requires(std::is_same_v<std::string, T> || std::is_same_v<std::wstring, T>)
struct api_set_value
{
    // empty means default host
    T importer;
    T host;
};

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
auto
add_host(
    std::vector<win::api_set_value<T>>& modules,
    win::api_set_value<T>               entry
) noexcept -> void
{
    if (entry.host.empty()) {
        return;
    }

    for (const auto& existing : modules) {
        if (existing.importer == entry.importer && existing.host == entry.host) {
            return;
        }
    }

    modules.push_back(std::move(entry));
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

template<class T>
requires(std::is_same_v<std::string, T> || std::is_same_v<std::wstring, T>)
auto
make_lower(
    T& str
) noexcept -> T&
{
    // actually just to avoid adding `algorithm` header
    using char_t = typename T::value_type;

    for (auto& c : str) {
        if (c >= char_t('A') && c <= char_t('Z')) {
            c = static_cast<char_t>(c + (char_t('a') - char_t('A')));
        }
    }

    return str;
}
} //namespace detail

namespace win {

// schema becomes:
using api_set_schema_w = std::unordered_map<std::wstring, std::vector<api_set_value<std::wstring>>>;
using api_set_schema_a = std::unordered_map<std::string, std::vector<api_set_value<std::string>>>;

NODISCARD
auto
get_api_set_schema_a() noexcept -> const api_set_schema_a&;

NODISCARD
auto
get_api_set_schema_w() noexcept -> const api_set_schema_w&;

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
    u32                 name,
    bool                lowercase = true,
    const std::wstring& importer = {}
) noexcept -> uptr;

NODISCARD
auto
get_module_handle(
    const std::string& name,
    bool               lowercase = true,
    const std::string& importer = {}
) noexcept -> uptr;

NODISCARD
auto
get_module_handle(
    const std::wstring& name,
    bool                lowercase = true,
    const std::wstring& importer = {}
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
    using char_t = typename T::value_type;

    if (!api_set_map) {
        return result_t{};
    }

    result_t          ret{};
    const auto* const base = reinterpret_cast<const u8*>(api_set_map);
    const auto* const header = reinterpret_cast<const u32*>(base);
    const auto        count = header[1];
    const auto* const entries = reinterpret_cast<const rtl::api_set_namespace_entry_v2*>(base + sizeof(u32) * 2);

    for (u32 i{}; i < count; ++i) {
        std::vector<api_set_value<T>> modules;

        const auto& e = entries[i];
        auto              proxy = detail::get_wstring(base, e.name_offset, e.name_len);
        const auto        num_values = reinterpret_cast<const u32*>(base + e.data_offset)[0];
        const auto* const value_entry = reinterpret_cast<const rtl::api_set_value_entry_v2*>(base + e.data_offset + sizeof(u32));

        for (u32 j{}; j < num_values; ++j) {
            const auto& v = value_entry[j];

            auto importer = v.name_len
                ? detail::get_wstring(base, v.name_offset, v.name_len)
                : std::wstring{};
            auto host = detail::get_wstring(base, v.value_offset, v.value_len);

            detail::make_lower(importer);
            detail::make_lower(host);

            if constexpr (std::is_same_v<std::string, T>) {
                modules.push_back({ to_ansi(importer), to_ansi(host) });
            } else {
                modules.push_back({ std::move(importer), std::move(host) });
            }
        }

        if (!modules.empty()) {
            detail::make_lower(proxy);

            if (const auto pos = proxy.rfind(static_cast<char_t>('-')); pos != T::npos) {
                proxy.resize(pos);
            }

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
    const auto* const base = reinterpret_cast<const uint8_t*>(api_set_map);
    const auto* const header = reinterpret_cast<const u32*>(base);
    const auto        count = header[3];
    const auto* const entries = reinterpret_cast<const rtl::api_set_namespace_entry_v4*>(base + sizeof(u32) * 6);

    for (u32 i{}; i < count; ++i) {
        std::vector<api_set_value<T>> modules;
        const auto& e = entries[i];
        auto              proxy = detail::get_wstring(base, e.name_offset, e.name_len);
        auto              alias = detail::get_wstring(base, e.alias_offset, e.alias_len);
        const auto        num_values = *reinterpret_cast<const u32*>(base + e.data_offset);
        const auto* const value_entry = reinterpret_cast<const rtl::api_set_value_entry_v4*>(base + e.data_offset + sizeof(u32));

        if (!alias.empty()) {
            detail::make_lower(alias);

            if constexpr (std::is_same_v<std::string, T>) {
                detail::add_host(modules, {T{}, to_ansi(alias)});
            } else {
                detail::add_host(modules, {T{}, std::move(alias)});
            }
        }

        for (u32 j{}; j < num_values; ++j) {
            const auto& v = value_entry[j];

            auto importer = v.name_len
                ? detail::get_wstring(base, v.name_offset, v.name_len)
                : std::wstring{};
            auto host = detail::get_wstring(base, v.value_offset, v.value_len);

            detail::make_lower(importer);
            detail::make_lower(host);

            if constexpr (std::is_same_v<std::string, T>) {
                modules.push_back({to_ansi(importer), to_ansi(host)});
            } else {
                modules.push_back({std::move(importer), std::move(host)});
            }
        }

        if (!modules.empty()) {
            detail::make_lower(proxy);

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

    if (!api_set_map) {
        return result_t{};
    }

    result_t          ret{};
    const auto* const base       = reinterpret_cast<const uint8_t*>(api_set_map);
    const auto* const ns         = reinterpret_cast<const rtl::api_set_namespace_v6*>(base);
    const auto* const entry_base = reinterpret_cast<const rtl::api_set_namespace_entry_v6*>(base + ns->entry_offset);

    for (u32 i{}; i < ns->count; ++i) {
        std::vector<api_set_value<T>> modules;
        const auto&       e          = entry_base[i];
        // important: use hashed_len, not name_le
        // this is the canonical key the loader matches
        // against (e.g. "api-ms-win-core-processthreads-l1-1" rather
        // than "api-ms-win-core-processthreads-l1-1-6").
        auto              proxy      = detail::get_wstring(base, e.name_offset, e.hashed_len);
        const auto* const value_base = reinterpret_cast<const rtl::api_set_value_entry_v6*>(base + e.value_offset);

        for (u32 j{}; j < e.value_count; ++j) {
            const auto& v = value_base[j];
            auto importer = v.name_len
                ? detail::get_wstring(base, v.name_offset, v.name_len)
                : std::wstring{};
            auto host = detail::get_wstring(base, v.value_offset, v.value_len);

            detail::make_lower(importer);
            detail::make_lower(host);

            if constexpr (std::is_same_v<std::string, T>) {
                modules.push_back({to_ansi(importer), to_ansi(host)});
            } else {
                modules.push_back({std::move(importer), std::move(host)});
            }
        }

        detail::make_lower(proxy);

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
normalize_api_set_name(
    T name
) noexcept -> T
{
    using char_t = typename T::value_type;

    detail::make_lower(name);

    // strip ".dll"
    if constexpr (std::is_same_v<char_t, wchar_t>) {
        if (name.size() >= 4 &&
            std::wstring_view{name}.ends_with(xors(L".dll"))
        ) {
            name.resize(name.size() - 4);
        }
    } else {
        if (
            name.size() >= 4 &&
            std::string_view{name}.ends_with(xors(".dll"))
        ) {
            name.resize(name.size() - 4);
        }
    }

    // strip trailing version segment: everything from the last '-' onward.
    // "api-ms-win-core-processthreads-l1-1-6" -> "api-ms-win-core-processthreads-l1-1"
    if (const auto pos = name.rfind(static_cast<char_t>('-')); pos != T::npos) {
        name.resize(pos);
    }

    return name;
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
    >& name,
    const std::conditional_t<
        std::is_same_v<api_set_schema_a, T>,
        std::string,
        std::wstring
    >& importer = {}
) noexcept
-> const std::conditional_t<
    std::is_same_v<api_set_schema_a, T>,
    std::string,
    std::wstring
>*
{
    using key_t = std::conditional_t<std::is_same_v<api_set_schema_a, T>, std::string, std::wstring>;

    const auto needle = normalize_api_set_name<key_t>(name);
    const auto it     = cache.find(needle);

    if (it == cache.end()) {
        return nullptr;
    }

    auto importer_norm = importer;
    detail::make_lower(importer_norm);

    const api_set_value<key_t>* chosen = nullptr;
    if (!importer_norm.empty()) {
        for (const auto& v : it->second) {
            if (!v.importer.empty() && v.importer == importer_norm) {
                chosen = &v;
                break;
            }
        }
    }

    if (!chosen) {
        for (const auto& v : it->second) {
            if (v.importer.empty()) {
                chosen = &v;
                break;
            }
        }
    }

    if (!chosen) {
        return nullptr;
    }

    const bool is_apiset = [&]
    {
        if constexpr (std::is_same_v<key_t, std::wstring>) {
            return chosen->host.starts_with(xors(L"api-")) || chosen->host.starts_with(xors(L"ext-"));
        } else {
            return chosen->host.starts_with(xors("api-")) || chosen->host.starts_with(xors("ext-"));
        }
    }();

    if (is_apiset) {
        return resolve_api_schema(cache, chosen->host, importer_norm);
    }

    return &chosen->host;
}

template<class T>
requires(std::is_same_v<api_set_schema_a, T> || std::is_same_v<api_set_schema_w, T>)
NODISCARD
auto
resolve_api_schema(
    const T& cache,
    const std::conditional_t<
        std::is_same_v<api_set_schema_a, T>,
        std::string_view,
        std::wstring_view
    >& name
) noexcept
-> const std::conditional_t<
    std::is_same_v<api_set_schema_a, T>,
    std::string,
    std::wstring
>*
{
    using key_t = std::conditional_t<std::is_same_v<api_set_schema_a, T>, std::string, std::wstring>;

    return resolve_api_schema(cache, key_t{name});
}

template<class T>
requires(std::is_same_v<api_set_schema_a, T> || std::is_same_v<api_set_schema_w, T>)
NODISCARD
auto
resolve_api_schema(
    const T&   cache,
    const u32  name_hash,
    const bool lowercase = true,
    const std::conditional_t<
        std::is_same_v<api_set_schema_a, T>,
        std::string,
        std::wstring
    >& importer = {}
) noexcept
-> const std::conditional_t<
    std::is_same_v<api_set_schema_a, T>,
    std::string,
    std::wstring
>*
{
    // cache keys are normalized (lowercase, no ".dll", no version suffix).
    // the caller **MUST** compute name_hash over the same normalized form.
    for (const auto& [key, values] : cache) {
        const auto hash = lowercase ? fnv<>::get<true>(key) : fnv<>::get(key);
        if (hash == name_hash) {
            return resolve_api_schema(cache, key, importer);
        }
    }
    return nullptr;
}
} //namespace win
} //namespace zen
