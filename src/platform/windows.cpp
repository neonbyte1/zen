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
#include <zen/platform/windows.hpp>
#include <zen/platform/rtl/peb.hpp>
#include <zen/nt/directories/exports.hpp>
#include <zen/nt/image.hpp>
#include <intrin.h>

using namespace zen;

namespace {
NODISCARD
auto
find_proc_address(
    const uptr handle,
    const u32  proc_hash,
    const i32  proc_ordinal,
    const bool lowercase
) noexcept -> uptr
{
    const auto* const dll = win::image<>::make(handle);

    if (!dll || !dll->valid()) {
        return 0;
    }

    const auto* const data_dir = dll->directory(win::directory::exports);

    if (!data_dir) {
        return 0;
    }

    const auto* const exp_dir = dll->rva_to_ptr<win::export_directory>(data_dir->rva());

    if (!exp_dir) {
        return 0;
    }

    const auto num_functions = exp_dir->num_functions();
    const auto num_names     = exp_dir->num_names();

    if (!num_functions) {
        return 0;
    }

    const auto        delta     = reinterpret_cast<const u8*>(exp_dir) - data_dir->rva();
    const auto* const names     = dll->rva_to_ptr<u32>(exp_dir->rva_names());
    const auto* const functions = dll->rva_to_ptr<u32>(exp_dir->rva_functions());
    const auto* const ordinals  = dll->rva_to_ptr<u16>(exp_dir->rva_name_ordinals());

    for (u32 i{}; i < num_functions; ++i) {
        const auto ordinal = proc_ordinal != -1
            ? static_cast<u16>(i)
            : ordinals[i];

        const auto found_by_ordinal
            = proc_ordinal != -1
            && static_cast<u16>(proc_ordinal) == exp_dir->base() + ordinal;

        bool found_by_name{};

        if (!found_by_ordinal && i < num_names) {
            const std::string_view function_name{
                reinterpret_cast<const char*>(delta + names[i])
            };

            const auto hash = lowercase
                ? fnv<>::get<true>(function_name)
                : fnv<>::get(function_name);

            found_by_name = hash == proc_hash;
        }

        if (!found_by_ordinal && !found_by_name) {
            continue;
        }

        const auto function_pointer = delta + functions[ordinal];
        const auto directory_begin  = delta + data_dir->rva();
        const auto directory_end    = directory_begin + data_dir->size();

        if (function_pointer >= directory_begin && function_pointer <= directory_end) {
            // e.g. "api-ms-win-core-processthreads-l1-1-6.SetProcessDynamicEnforcedCetCompatibleRanges"
            const auto* const      forwarded_name_raw = reinterpret_cast<const char*>(function_pointer);
            const std::string_view forwarded_name{forwarded_name_raw};
            const auto             dot_pos{forwarded_name.find('.')};

            if (dot_pos == std::string_view::npos) {
                return 0;
            }

            // e.g. "api-ms-win-core-processthreads-l1-1-6" + ".dll"
            const auto lib_hash         = fnv<>::hash(".dll", fnv<>::get<true>(forwarded_name.substr(0, dot_pos)));
            const auto forwarded_handle = win::get_module_handle(lib_hash);

            if (!forwarded_handle) {
                return 0;
            }

            return forwarded_handle
                ? find_proc_address(
                    forwarded_handle,
                    // e.g. "SetProcessDynamicEnforcedCetCompatibleRanges"
                    fnv<>::get<true>(forwarded_name.substr(dot_pos + 1)),
                    -1,
                    true
                ) : 0;
        }

        return reinterpret_cast<const uptr>(function_pointer);
    }

    return 0;
}

template<typename T, typename View, typename Fn>
auto
convert(
    const View in,
    Fn         fn
) noexcept -> std::basic_string<T>
{
    size_t               bytes_needed{};
    T                    stack_buffer[512]{};
    std::unique_ptr<T[]> heap_buffer;
    std::basic_string<T> result;

    if (!fn(&bytes_needed, nullptr, 0, in.data(), in.length())) {
        auto* ptr{ stack_buffer };
        size_t bytes_converted{};

        if (bytes_needed > sizeof(stack_buffer)) {
            heap_buffer = std::make_unique<T[]>(bytes_needed);
            ptr = heap_buffer.get();
        }

        if (!fn(&bytes_converted, ptr, bytes_needed, in.data(), in.length())) {
            result.assign(ptr);
        }
    }

    return result;
}
}

auto
win::get_peb() noexcept -> uptr
{
    // https://en.wikipedia.org/wiki/Win32_Thread_Information_Block#Contents_of_the_TIB_on_Windows
    // Type     Offset (x86)    Offset (x64)    Description
    // pointer 	FS:[0x30] 	    GS:[0x60] 	    Linear address of Process Environment Block (PEB)
#if defined(ZEN_TARGET_64_BIT)
    return __readgsqword(0x60);
#else
    return __readfsdword(0x30);
#endif
}

auto
win::get_module_handle(
    const u32  name,
    const bool lowercase
) noexcept -> uptr
{
    const auto validate_entry = [&](
        const rtl::ldr_data_table_entry<>* const entry
    ) noexcept -> bool
    {
        if (!entry->base_dll_name.buffer()) {
            return false;
        }

        if (!fnv<>::valid(name)) {
            return true;
        }

        const auto hash = lowercase
            ? fnv<>::get<true>(entry->base_dll_name.get())
            : fnv<>::get(entry->base_dll_name.get());

        return hash == name;
    };


    const auto* const peb = reinterpret_cast<const rtl::peb<>*>(get_peb());

    if (const auto* const ldr = peb->loader()) {
        auto current_entry = ldr->in_load_order_module_list.next;

        while (
            current_entry
            && current_entry != reinterpret_cast<const uptr>(&ldr->in_load_order_module_list)
        ) {
            const auto* const current
                = ZEN_WIN32_CONTAINING_RECORD(
                    current_entry,
                    rtl::ldr_data_table_entry<>,
                    in_load_order_links
                );

            if (validate_entry(current)) {
                return current->dll_base;
            }

            current_entry = reinterpret_cast<const rtl::list_entry<>*>(current)->next;
        }
    }

    /** @todo: ApiSchemaSet support */

    return 0;
}

auto
win::get_module_handle(
    const std::string_view name,
    const bool             lowercase
) noexcept -> uptr
{
    const u32 name_hash = !name.empty()
        ? (lowercase ? fnv<>::get<true>(name) : fnv<>::get(name))
        : 0;

    return get_module_handle(name_hash, lowercase);
}

auto
win::get_module_handle(
    const std::wstring_view name,
    const bool              lowercase
) noexcept -> uptr
{
    const u32 name_hash = !name.empty()
        ? (lowercase ? fnv<>::get<true>(name) : fnv<>::get(name))
        : 0;

    return get_module_handle(name_hash, lowercase);
}

auto
win::get_proc_address(
    const uptr             handle,
    const std::string_view name,
    const bool             lowercase
) noexcept -> uptr
{
    if (!handle || name.empty()) {
        return 0;
    }

    const auto name_ptr   = reinterpret_cast<const uptr>(name.data());
    const auto is_ordinal = name_ptr <= 0xFFFF;

    return find_proc_address(
        handle,
        !is_ordinal
            ? (lowercase ? fnv<>::get<true>(name) : fnv<>::get(name))
            : 0,
        is_ordinal ? static_cast<i32>(name_ptr) : -1,
        lowercase
    );
}

auto
win::get_proc_address(
    const uptr handle,
    const u32  name,
    const bool lowercase
) noexcept -> uptr
{
    if (!handle || !fnv<>::valid(name)) {
        return 0;
    }

    return find_proc_address(handle, name, -1, lowercase);
}

auto
win::to_wide(
    const std::string_view input
) noexcept -> std::wstring
{
    return ::convert<wchar_t>(
        input,
        [](
            size_t* const  num_chars_converted,
            wchar_t* const dst,
            const szt      size,
            const char*    src,
            const szt      src_size
        ) noexcept -> errno_t
        {
            return mbstowcs_s(num_chars_converted, dst, size, src, src_size);
        }
    );
}

auto
win::to_ansi(
    const std::wstring_view input
) noexcept -> std::string
{
    return ::convert<char>(
        input,
        [](
            szt* const     num_chars_converted,
            char* const    dst,
            const szt      size,
            const wchar_t* src,
            const szt      src_size
        ) noexcept -> errno_t
        {
            return wcstombs_s(num_chars_converted, dst, size, src, src_size);
        }
    );
}
