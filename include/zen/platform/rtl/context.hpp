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

#include <zen/core/requirements.hpp>

namespace zen::rtl {
struct ZEN_DECLSPEC_ALIGN(16) m128a
{
    u64 low{};
    i64 high{};
};

template<bool X64>
struct ZEN_DECLSPEC_ALIGN(16) save_format
{
private:
    NODISCARD
    constexpr
    static
    auto
    num_xmm_registers() noexcept -> szt
    {
        return X64 ? 16 : 8;
    }

    NODISCARD
    constexpr
    static
    auto
    num_reserved_bytes() noexcept -> szt
    {
        return X64 ? 96 : 224;
    }

public:
    u16   control_word{};
    u16   status_word{};
    u8    tag_word{};
    u8    reserved1{};
    u16   error_opcode{};
    u32   error_offset{};
    u16   error_selector{};
    u16   reserved2{};
    u32   data_offset{};
    u16   data_selector{};
    u16   reserved3{};
    u32   mx_csr{};
    u32   mx_csr_mask{};
    m128a float_registers[8]{};
    m128a xmm_registers[num_xmm_registers()]{};
    u8    reserved4[num_reserved_bytes()]{};
};

struct ZEN_DECLSPEC_ALIGN(16) context64
{
    enum struct flags : u32
    {
        amd                 = 0x100000,
        control             = amd | 0x00000001L,
        integer             = amd | 0x00000002L,
        segments            = amd | 0x00000004L,
        floating_point      = amd | 0x00000008L,
        debug_registers     = amd | 0x00000010L,
        xstate              = amd | 0x00000040L,
        kernel_cet          = amd | 0x00000080L,
        kernel_debugger     = amd | 0x04000000L,
        exception_active    = amd | 0x08000000L,
        service_active      = amd | 0x10000000L,
        exception_request   = amd | 0x40000000L,
        exception_reporting = amd | 0x80000000L,
        full                = control | integer | segments,
        all                 = full | floating_point | debug_registers,
    };
    union floating_state
    {
        save_format<true> save;

        struct
        {
            m128a header[2];
            m128a legacy[8];
            m128a xmm0;
            m128a xmm1;
            m128a xmm2;
            m128a xmm3;
            m128a xmm4;
            m128a xmm5;
            m128a xmm6;
            m128a xmm7;
            m128a xmm8;
            m128a xmm9;
            m128a xmm10;
            m128a xmm11;
            m128a xmm12;
            m128a xmm13;
            m128a xmm14;
            m128a xmm15;
        };
    };

    u64            p1_home{};
    u64            p2_home{};
    u64            p3_home{};
    u64            p4_home{};
    u64            p5_home{};
    u64            p6_home{};
    u32            context_flags{};
    u32            mx_csr{};
    u16            seg_cs{};
    u16            seg_ds{};
    u16            seg_es{};
    u16            seg_fs{};
    u16            seg_gs{};
    u16            seg_ss{};
    u32            e_flags{};
    u64            dr0{};
    u64            dr1{};
    u64            dr2{};
    u64            dr3{};
    u64            dr6{};
    u64            dr7{};
    u64            rax{};
    u64            rcx{};
    u64            rdx{};
    u64            rbx{};
    u64            rsp{};
    u64            rbp{};
    u64            rsi{};
    u64            rdi{};
    u64            r8{};
    u64            r9{};
    u64            r10{};
    u64            r11{};
    u64            r12{};
    u64            r13{};
    u64            r14{};
    u64            r15{};
    u64            rip{};
    floating_state flt{};
    m128a          vector_register[26]{};
    u64            vector_control{};
    u64            debug_control{};
    u64            last_branch_to_rip{};
    u64            last_branch_from_rip{};
    u64            last_exception_to_rip{};
    u64            last_exception_from_rip{};
};
ZEN_ENUM_OPERATORS(context64::flags);

struct context32
{
    enum struct flags : u32
    {
        i386                = 0x10000,
        control             = i386 | 0x01,
        integer             = i386 | 0x02,
        segments            = i386 | 0x04,
        floating_point      = i386 | 0x08,
        debug_registers     = i386 | 0x10,
        extended_registers  = i386 | 0x20,
        xstate              = i386 | 0x40,
        full                = control | integer | segments,
        all                 = full | floating_point | debug_registers | extended_registers,
    };

    struct floating_save_area
    {
        u32 control_word{};
        u32 status_word{};
        u32 tag_word{};
        u32 error_offset{};
        u32 error_selector{};
        u32 data_offset{};
        u32 data_selector{};
        u8  register_area[80 /*SIZE_OF_80387_REGISTERS*/]{};
        u32 spare0{};
    };

    u32                context_flags{};
    u32                dr0{};
    u32                dr1{};
    u32                dr2{};
    u32                dr3{};
    u32                dr6{};
    u32                dr7{};
    floating_save_area float_save{};
    u32                seg_gs{};
    u32                seg_fs{};
    u32                seg_es{};
    u32                seg_ds{};
    u32                edi{};
    u32                esi{};
    u32                ebx{};
    u32                edx{};
    u32                ecx{};
    u32                eax{};
    u32                ebp{};
    u32                eip{};
    u32                seg_cs{};              // MUST BE SANITIZED
    u32                e_flags{};             // MUST BE SANITIZED
    u32                esp{};
    u32                seg_ss{};
    u8                 extended_registers[512 /*MAXIMUM_SUPPORTED_EXTENSION*/]{};
};
ZEN_ENUM_OPERATORS(context32::flags);

template<bool X64 = detail::is_64_bit>
using context = std::conditional_t<X64, context64, context32>;
} //namespace zen::rtl
