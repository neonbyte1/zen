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
#include <zen/platform/common/memory_section.hpp>
#include <zen/platform/syscall.hpp>

using namespace zen::win;

memory_section::memory_section(
    const page_protection prot,
    const u64             size
) noexcept
{
    void* section_handle{};
    auto  large_size = static_cast<i64>(size);

    if (
        nt_create_section(
            &section_handle,
            std::to_underlying(base_access::generic_all),
            nullptr,
            &large_size,
            std::to_underlying(prot),
            0x08000000 /*SEC_COMMIT*/,
            nullptr
        )
    ) {
        handle_.reset(section_handle);
        protection_ = prot;
        size_       = size;
    }
}

auto
memory_section::map(
    const void* const     process_handle,
    const allocation_type allocation
) const noexcept -> u64
{
    return valid()
        ? nt_map_view_of_section(
            *handle_,
            process_handle,
            size_,
            allocation,
            protection_,
            rtl::section_inherit::view_unmap
        ) : 0;
}
