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

#include <zen/platform/rtl/version_info.hpp>

namespace zen {
namespace detail {
enum struct build_threshold : u32
{
    // https://en.wikipedia.org/wiki/Windows_11_version_history
    undefined = 0,
    rs0       = 10586,
    rs1       = 14393,
    rs2       = 15063,
    rs3       = 16299,
    rs4       = 17134,
    rs5       = 17763,
    h1_19     = 18362,
    h2_19     = 18363,
    h1_20     = 19041,
    h1_21     = 19044,
    h2_21     = 22000,
    h2_22     = 22621,
    h2_23     = 22631,
    h2_24     = 26100,
    h2_25     = 26200,
    h1_26     = 28000,
    h2_26     = 26300,
    max       = 99999,
};

enum struct nt_version : u16
{
    nt_4      = 0x0400,
    win_2k    = 0x0500,
    win_xp    = 0x0501,
    ws_03     = 0x0502,
    win_vista = 0x0600,  // also Win6 / WS08 / Longhorn
    win_7     = 0x0601,
    win_8     = 0x0602,
    win_8_1   = 0x0603,
    win_10    = 0x0A00,
};
} //namespace detail

namespace win {
enum struct version_id : u8
{
    unsupported,
    win_xp,
    win_7,
    win_8,
    win_8_1,
    win_10,
    win_10_rs1,
    win_10_rs2,
    win_10_rs3,
    win_10_rs4,
    win_10_rs5,
    win_10_19h1,
    win_10_19h2,
    win_10_20h1,
    win_10_21h1,
    win_11_21h2,
    win_11_22h2,
    win_11_23h2,
    win_11_24h2,
};

class version_info_t
{
    using build_threshold = detail::build_threshold;
    using nt_version      = detail::nt_version;

    struct entry_t
    {
        version_id      id;
        build_threshold threshold;
    };

    static constexpr entry_t entries[] =
    {
        {version_id::win_11_24h2, build_threshold::h2_24},
        {version_id::win_11_23h2, build_threshold::h2_23},
        {version_id::win_11_22h2, build_threshold::h2_22},
        {version_id::win_11_21h2, build_threshold::h2_21},
        {version_id::win_10_21h1, build_threshold::h1_21},
        {version_id::win_10_20h1, build_threshold::h1_20},
        {version_id::win_10_19h2, build_threshold::h2_19},
        {version_id::win_10_19h1, build_threshold::h1_19},
        {version_id::win_10_rs5,  build_threshold::rs5},
        {version_id::win_10_rs4,  build_threshold::rs4},
        {version_id::win_10_rs3,  build_threshold::rs3},
        {version_id::win_10_rs2,  build_threshold::rs2},
        {version_id::win_10_rs1,  build_threshold::rs1},
        {version_id::win_10,      build_threshold::rs0},
    };

    NODISCARD
    constexpr
    auto
    is_at_least(
        const detail::nt_version native_version,
        const u16                service_pack_major,
        const build_threshold    build = build_threshold::undefined
    ) const noexcept -> bool
    {
        const auto major = native.version.major();

        if (!major) {
            return false;
        }

        const auto minor = native.version.minor();
        const auto spm   = native.service_pack.major();



        if (major != bit::u16_hi(native_version)) {
            return major > bit::u16_hi(native_version);
        }
        if (minor != bit::u16_lo(native_version)) {
            return minor > bit::u16_lo(native_version);
        }
        if (service_pack_major != spm) {
            return service_pack_major > spm;
        }

        return native.build_number >= std::to_underlying(build);
    }

public:
    version_id                id{};
    rtl::os_version_info_ex_w native{};

    auto
    classify() noexcept -> void
    {
        id = version_id::unsupported;

        const auto full_version
            = static_cast<detail::nt_version>(
                (native.version.major() << 8) | native.version.minor()
            );

        switch (full_version) {
        case nt_version::win_10:
            for (const auto& entry : entries) {
                if (native.build_number >= std::to_underlying(entry.threshold)) {
                    id = entry.id;

                    return;
                }
            }
            break;

        case nt_version::win_8_1: id = version_id::win_8_1; break;
        case nt_version::win_8:   id = version_id::win_8;   break;
        case nt_version::win_7:   id = version_id::win_7;   break;
        case nt_version::win_xp:  id = version_id::win_xp;  break;
        default:                                            break;
        }
    }

    NODISCARD
    constexpr
    auto
    is_at_least_win_xp() const noexcept -> bool
    {
        return is_at_least(nt_version::win_xp, 0);
    }

    NODISCARD
    constexpr
    auto
    is_at_least_win_xp_sp1() const noexcept -> bool
    {
        return is_at_least(nt_version::win_xp, 1);
    }

    NODISCARD
    constexpr
    auto
    is_at_least_win_xp_sp2() const noexcept -> bool
    {
        return is_at_least(nt_version::win_xp, 2);
    }

    NODISCARD
    constexpr
    auto
    is_at_least_win_xp_sp3() const noexcept -> bool
    {
        return is_at_least(nt_version::win_xp, 3);
    }

    NODISCARD
    constexpr
    auto
    is_at_least_win_vista() const noexcept -> bool
    {
        return is_at_least(nt_version::win_vista, 0);
    }

    NODISCARD
    constexpr
    auto
    is_at_least_win_vista_sp1() const noexcept -> bool
    {
        return is_at_least(nt_version::win_vista, 1);
    }

    NODISCARD
    constexpr
    auto
    is_at_least_win_vista_sp2() const noexcept -> bool
    {
        return is_at_least(nt_version::win_vista, 2);
    }

    NODISCARD
    constexpr
    auto
    is_at_least_win_7() const noexcept -> bool
    {
        return is_at_least(nt_version::win_7, 0);
    }

    NODISCARD
    constexpr
    auto
    is_at_least_win_7_sp1() const noexcept -> bool
    {
        return is_at_least(nt_version::win_7, 1);
    }

    NODISCARD
    constexpr
    auto
    is_at_least_win_8() const noexcept -> bool
    {
        return is_at_least(nt_version::win_8, 0);
    }

    NODISCARD
    constexpr
    auto
    is_at_least_win_8_1() const noexcept -> bool
    {
        return is_at_least(nt_version::win_8_1, 0);
    }

    NODISCARD
    constexpr
    auto
    is_at_least_win_10() const noexcept -> bool
    {
        return is_at_least(nt_version::win_10, 0);
    }

    NODISCARD
    constexpr
    auto
    is_at_least_win_10_rs1() const noexcept -> bool
    {
        return is_at_least(nt_version::win_10, 0, build_threshold::rs1);
    }

    NODISCARD
    constexpr
    auto
    is_at_least_win_10_rs2() const noexcept -> bool
    {
        return is_at_least(nt_version::win_10, 0, build_threshold::rs2);
    }

    NODISCARD
    constexpr
    auto
    is_at_least_win_10_rs3() const noexcept -> bool
    {
        return is_at_least(nt_version::win_10, 0, build_threshold::rs3);
    }

    NODISCARD
    constexpr
    auto
    is_at_least_win_10_rs4() const noexcept -> bool
    {
        return is_at_least(nt_version::win_10, 0, build_threshold::rs4);
    }

    NODISCARD
    constexpr
    auto
    is_at_least_win_10_rs5() const noexcept -> bool
    {
        return is_at_least(nt_version::win_10, 0, build_threshold::rs5);
    }

    NODISCARD
    constexpr
    auto
    is_at_least_win_10_19h1() const noexcept -> bool
    {
        return is_at_least(nt_version::win_10, 0, build_threshold::h1_19);
    }

    NODISCARD
    constexpr
    auto
    is_at_least_win_10_19h2() const noexcept -> bool
    {
        return is_at_least(nt_version::win_10, 0, build_threshold::h2_19);
    }

    NODISCARD
    constexpr
    auto
    is_at_least_win_10_20h1() const noexcept -> bool
    {
        return is_at_least(nt_version::win_10, 0, build_threshold::h1_20);
    }

    NODISCARD
    constexpr
    auto
    is_at_least_win_10_21h1() const noexcept -> bool
    {
        return is_at_least(nt_version::win_10, 0, build_threshold::h1_21);
    }

    NODISCARD
    constexpr
    auto
    is_at_least_win_11_21h2() const noexcept -> bool
    {
        return is_at_least(nt_version::win_10, 0, build_threshold::h2_21);
    }

    NODISCARD
    constexpr
    auto
    is_at_least_win_11_22h2() const noexcept -> bool
    {
        return is_at_least(nt_version::win_10, 0, build_threshold::h2_22);
    }

    NODISCARD
    constexpr
    auto
    is_at_least_win_11_23h2() const noexcept -> bool
    {
        return is_at_least(nt_version::win_10, 0, build_threshold::h2_23);
    }

    NODISCARD
    constexpr
    auto
    is_at_least_win_11_24h2() const noexcept -> bool
    {
        return is_at_least(nt_version::win_10, 0, build_threshold::h2_24);
    }

    NODISCARD
    constexpr
    auto
    is_at_least_win_11_25h2() const noexcept -> bool
    {
        return is_at_least(nt_version::win_10, 0, build_threshold::h2_25);
    }

    NODISCARD
    constexpr
    auto
    is_at_least_win_11_26h1() const noexcept -> bool
    {
        return is_at_least(nt_version::win_10, 0, build_threshold::h1_26);
    }

    NODISCARD
    constexpr
    auto
    is_at_least_win_11_26h2() const noexcept -> bool
    {
        return is_at_least(nt_version::win_10, 0, build_threshold::h2_26);
    }
};
} //namespace win
} //namespace zen
