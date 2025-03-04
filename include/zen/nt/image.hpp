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
#pragma once

#include <zen/nt/dos_header.hpp>

#if defined(ZEN_IMAGE_IMPORT_INFO_COLLECTION)
#   include <zen/nt/directories/imports.hpp>
#   include <zen/nt/directories/iat.hpp>
#   include <unordered_map>
#endif //ZEN_IMAGE_IMPORT_INFO_COLLECTION

#if defined(ZEN_IMAGE_EXPORT_INFO_COLLECTION)
#   include <zen/nt/directories/exports.hpp>
#   include <vector>
#endif //ZEN_IMAGE_EXPORT_INFO_COLLECTION

#if defined(ZEN_IMAGE_RELOC_INFO_COLLECTION)
#   include <zen/nt/directories/relocs.hpp>
#endif //ZEN_IMAGE_RELOC_INFO_COLLECTION

ZEN_WIN32_ALIGNMENT(zen::win)
#if defined(ZEN_IMAGE_IMPORT_INFO_COLLECTION)
struct import_info
{
    constexpr
        explicit
        import_info(
            const u32 index
        ) noexcept
        : index{ index }
    {}

    std::string name;
    u32         index{};
    u16         ordinal{};
    u64         rva{};
};
#endif //ZEN_IMAGE_IMPORT_INFO_COLLECTION

#if defined(ZEN_IMAGE_EXPORT_INFO_COLLECTION)
struct export_info
{
    struct forward_info
    {
        std::string library;
        std::string function;
    };

    std::string  name;
    u32          rva{};
    u16          ordinal{};
    forward_info forward{};

    NODISCARD
        constexpr
        auto
        forwarded() const noexcept -> bool
    {
        return !forward.library.empty()
            && !forward.function.empty();
    }
};
#endif //ZEN_IMAGE_EXPORT_INFO_COLLECTION

#if defined(ZEN_IMAGE_RELOC_INFO_COLLECTION)
struct reloc_info
{
    u32        base_rva{};
    u16        offset{};
    reloc_type type{};
};
#endif //ZEN_IMAGE_RELOC_INFO_COLLECTION

template<bool X64 = detail::is_64_bit>
class image
{
public:
    constexpr
    image() noexcept = default;

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    valid() const noexcept -> bool
    {
        return dos_hdr_.valid() && nt_hdr()->valid();
    }

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    is_32_bit() const noexcept -> bool
    {
        return nt_hdr()->is_32_bit();
    }

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    is_64_bit() const noexcept -> bool
    {
        return nt_hdr()->is_64_bit();
    }

    NODISCARD
    constexpr
    auto
    dos_hdr() noexcept -> dos_header*
    {
        return &dos_hdr_;
    }

    NODISCARD
    constexpr
    auto
    dos_hdr() const noexcept -> const dos_header*
    {
        return const_cast<image*>(this)->dos_hdr();
    }

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    nt_hdr() noexcept -> nt_headers<X64>*
    {
        return dos_hdr_.nt_hdr<X64>();
    }

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    nt_hdr() const noexcept -> const nt_headers<X64>*
    {
        return const_cast<image*>(this)->nt_hdr();
    }

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    optional_hdr() noexcept -> optional_header<X64>*
    {
        return &nt_hdr()->optional_hdr();
    }

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    optional_hdr() const noexcept -> const optional_header<X64>*
    {
        return const_cast<image*>(this)->optional_hdr();
    }

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    file_hdr() noexcept -> file_header*
    {
        return &nt_hdr()->file_hdr();
    }

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    file_hdr() const noexcept -> const file_header*
    {
        return const_cast<image*>(this)->file_hdr();
    }

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    directory(
        const win::directory id
    ) noexcept -> data_directory*
    {
        auto* const opt = optional_hdr();

        if (opt->num_data_directories() <= static_cast<u16>(id)) {
            return nullptr;
        }

        auto* const dir = &opt->data_directories().at(id);

        return dir->present() ? dir : nullptr;
    }

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    directory(
        const win::directory id
    ) const noexcept -> const data_directory*
    {
        return const_cast<image*>(this)->directory(id);
    }

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    raw_limit() const noexcept -> szt
    {
        const auto* const nt  = nt_hdr();
        const auto* const scn = nt->template sections<false>();

        szt max_raw = nt->optional_hdr().size_headers();

        for (szt i{}; i < nt->file_hdr().num_sections(); ++i) {
            max_raw = std::max<szt>(scn[i].ptr_raw_data() + scn[i].size_raw_data(), max_raw);
        }

        if (const auto* const dir = directory(win::directory::security)) {
            max_raw = std::max<szt>(dir->rva() + dir->size(), max_raw);
        }

        return max_raw;
    }

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    rva_to_section(
        const u32 rva
    ) noexcept -> coff::section_header*
    {
        auto* const nt = nt_hdr();

        for (szt i{}; i < nt->file_hdr().num_sections(); ++i) {
            auto* const section = nt->section(i);

            if (
                section->virtual_address() <= rva
                && rva < (section->virtual_address() + section->virtual_size())
            ) {
                return section;
            }
        }

        return nullptr;
    }

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    rva_to_section(
        const u32 rva
    ) const noexcept -> const coff::section_header*
    {
        return const_cast<image*>(this)->rva_to_section(rva);
    }

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    fo_to_section(
        const u32 offset
    ) noexcept -> coff::section_header*
    {
        auto* const nt = nt_hdr();

        for (szt i{}; i < nt->file_hdr().num_sections(); ++i) {
            auto* const section = nt->section(i);

            if (
                section->ptr_raw_data() <= offset
                && offset < (section->ptr_raw_data() + section->size_raw_data())
            ) {
                return section;
            }
        }

        return nullptr;
    }

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    fo_to_section(
        const u32 offset
    ) const noexcept -> const coff::section_header*
    {
        return const_cast<image*>(this)->fo_to_section(offset);
    }

    template<class T = u8>
    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    rva_to_ptr(
        const u32 rva,
        const szt length = 1
    ) noexcept -> T*
    {
        const auto* const scn = rva_to_section(rva);

        if (!scn) {
            const auto rva_hdr_end = nt_hdr()->optional_hdr().size_headers();

            if (rva < rva_hdr_end && (rva + length) <= rva_hdr_end) {
                return reinterpret_cast<T*>(
                    reinterpret_cast<std::byte*>(&dos_hdr_) + rva
                );
            }

            return nullptr;
        }

        // Apply the boundary check.
        const szt offset = rva - scn->virtual_address();

        if ((offset + length) > scn->size_raw_data()) {
            return nullptr;
        }


        // Return the final pointer.
        return reinterpret_cast<T*>(
            reinterpret_cast<std::byte*>(&dos_hdr_)
            + scn->ptr_raw_data()
            + offset
        );
    }

    template<class T = u8>
    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    rva_to_ptr(
        const u32 rva,
        const szt length = 1
    ) const noexcept -> const T*
    {
        return const_cast<image*>(this)->template rva_to_ptr<T>(rva, length);
    }

    template<class T = u8>
    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    fo_to_ptr(
        const u32 offset,
        const szt length = 1
    ) noexcept -> T*
    {
        const auto* const scn = fo_to_section(offset);

        if (!scn) {
            const auto rva_hdr_end = nt_hdr()->optional_hdr().size_headers();

            if (offset < rva_hdr_end && (offset + length) <= rva_hdr_end) {
                return reinterpret_cast<T*>(
                    reinterpret_cast<std::byte*>(&dos_hdr_) + offset
                );
            }

            return nullptr;
        }

        // Apply the boundary check.
        const szt soffset = offset - scn->ptr_raw_data();
        if ((soffset + length) > scn->virtual_size()) {
            return nullptr;
        }

        // Return the final pointer.
        return reinterpret_cast<T*>(
            reinterpret_cast<std::byte*>(&dos_hdr_)
            + scn->virtual_address()
            + soffset
        );
    }

    template<class T = u8>
    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    fo_to_ptr(
        const u32 offset,
        const szt length = 1
    ) const noexcept -> const T*
    {
        return const_cast<image*>(this)->template fo_to_ptr<T>(offset, length);
    }

    template<class T = u8>
    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    raw_to_ptr(
        const u32 offset,
        const szt length = 0
    ) noexcept -> T*
    {
        if (length != 0 && (offset + length) > raw_limit()) {
            return nullptr;
        }

        return reinterpret_cast<T*>(
            reinterpret_cast<std::byte*>(&dos_hdr_) + offset
        );
    }

    template<class T = u8>
    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    raw_to_ptr(
        const u32 offset,
        const szt length = 0
    ) const noexcept -> const T*
    {
        return const_cast<image*>(this)->template raw_to_ptr<T>(offset, length);
    }

#if defined(ZEN_IMAGE_IMPORT_INFO_COLLECTION)
    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    collect_imports() const -> std::unordered_map<std::string, std::vector<import_info>>
    {
        const auto* const data_directory = directory(win::directory::imports);

        if (!data_directory) {
            return {};
        }

        const auto* import_table = rva_to_ptr<import_directory>(data_directory->rva());

        std::unordered_map<std::string, std::vector<import_info>> result;

        for (
            u32 prev_name{};
            prev_name < import_table->rva_name();
            prev_name = import_table->rva_name(), ++import_table
        ) {
            const auto* const module_name_raw
                = rva_to_ptr<const char>(import_table->rva_name());

            if (!module_name_raw || *module_name_raw == 0) {
                continue;
            }

            const auto* entry = rva_to_ptr<image_thunk_data<X64>>(import_table->rva_original_first_thunk());
            std::vector<import_info> module_info;

            for (va_t<X64> i{}; i < entry->address(); i += sizeof(va_t<X64>), ++entry) {
                const auto* const import_by_name
                    = rva_to_ptr<image_named_import>(entry->address());

                import_info info{static_cast<u32>(i / sizeof(va_t<X64>))};

                info.rva = i + import_table->rva_first_thunk();

                if (!entry->is_ordinal() && import_by_name->name()[0]) {
                    info.name.assign(import_by_name->name());
                } else {
                    info.ordinal = entry->ordinal();
                }

                module_info.emplace_back(std::move(info));
            }

            result.insert(std::make_pair(module_name_raw, std::move(module_info)));
        }

        return result;
    }
#endif //ZEN_IMAGE_IMPORT_INFO_COLLECTION

#if defined(ZEN_IMAGE_EXPORT_INFO_COLLECTION)
    NODISCARD
    ZEN_CXX23_CONSTEXPR
    auto
    collect_exports() const -> std::vector<export_info>
    {
        std::vector<export_info> result{};
        const auto* const        data_directory
            = directory(win::directory::exports);

        if (data_directory) {
            const auto* const export_dir
                = rva_to_ptr<export_directory>(data_directory->rva());

            const auto num_functions = export_dir->num_functions();
            const auto num_names     = export_dir->num_names();

            if (num_functions > 0) {
                const auto* delta     = reinterpret_cast<const u8*>(export_dir) - data_directory->rva();
                const auto  names     = rva_to_ptr<u32>(export_dir->rva_names());
                const auto  functions = rva_to_ptr<u32>(export_dir->rva_functions());
                const auto  ordinals  = rva_to_ptr<u16>(export_dir->rva_name_ordinals());

                for (u32 i{}; i < num_functions; ++i) {
                    export_info info{};
                    const auto  ordinal = ordinals[i];

                    if (i < num_names) {
                        info.name.assign(reinterpret_cast<const char*>(delta + names[i]));
                    }

                    const auto function_pointer = delta + functions[ordinal];
                    const auto directory_begin  = delta + data_directory->rva();
                    const auto directory_end    = directory_begin + data_directory->size();

                    if (function_pointer >= directory_begin && function_pointer <= directory_end) {
                        // e.g. "api-ms-win-core-processthreads-l1-1-6.SetProcessDynamicEnforcedCetCompatibleRanges"
                        const auto* const      forwarded_name_raw = reinterpret_cast<const char*>(function_pointer);
                        const std::string_view forwarded_name{ forwarded_name_raw };
                        const auto             dot_pos{ forwarded_name.find('.') };

                        if (dot_pos == std::string_view::npos) {
                            continue;
                        }

                        info.forward.library = forwarded_name.substr(0, dot_pos);
                        info.forward.function = forwarded_name.substr(dot_pos + 1);
                    } else {
                        info.rva = functions[ordinal];
                    }

                    result.push_back(std::move(info));
                }
            }
        }

        return result;
    }
#endif //ZEN_IMAGE_EXPORT_INFO_COLLECTION

#if defined(ZEN_IMAGE_RELOC_INFO_COLLECTION)
    NODISCARD
    auto
    collect_relocations() const noexcept -> std::vector<reloc_info>
    {
        std::vector<reloc_info> result;

        if (const auto* const data_directory = directory(win::directory::basereloc)) {
            const auto* const reloc_dir = rva_to_ptr<reloc_directory>(data_directory->rva());
            const auto* const reloc_end
                = reinterpret_cast<const reloc_directory*>(
                    reinterpret_cast<const u8*>(reloc_dir)
                    + data_directory->size()
                );

            for (
                const auto* reloc = &reloc_dir->first_block;
                reloc < &reloc_end->first_block;
                reloc = reloc->next()
            ) {
                for (const auto* item = reloc->begin(); item != reloc->end(); ++item) {
                    result.emplace_back(reloc->base_rva(), item->offset(), item->type());
                }
            }
        }

        return result;
    }
#endif //ZEN_IMAGE_RELOC_INFO_COLLECTION

    NODISCARD
    ZEN_CXX23_CONSTEXPR
    static
    auto
    make(
        va_t<X64> handle
    ) noexcept -> image*
    {
        return reinterpret_cast<image*>(handle);
    }

private:

    dos_header dos_hdr_;
};
ZEN_RESTORE_ALIGNMENT() //namespace zen::win
