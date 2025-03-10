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

namespace zen {
class status_code
{
public:
    constexpr
    status_code() noexcept = default;

    template<std::integral T>
    constexpr
    explicit
    status_code(
        const T status
    ) noexcept
        : code_{static_cast<long_t>(status)}
    {}

    constexpr
    status_code(
        const status_code& rhs
    ) noexcept = default;

    constexpr
    status_code(
        status_code&& rhs
    ) noexcept
        : code_{rhs.release()}
    {}

    constexpr
    ~status_code() noexcept = default;

    constexpr
    auto
    operator=(
        const status_code& rhs
    ) noexcept -> status_code& = default;

    constexpr
    auto
    operator=(
        status_code&& rhs
    ) noexcept -> status_code&
    {
        reset(rhs.release());

        return *this;
    }

    template<std::integral T>
    constexpr
    auto
    operator=(
        const T rhs
    ) noexcept -> status_code&
    {
        code_ = static_cast<long_t>(rhs);

        return *this;
    }

    NODISCARD
    constexpr
    auto
    operator==(
        const status_code& rhs
    ) const noexcept -> bool
    {
        return operator==(rhs.code_);
    }

    template<std::integral T>
    NODISCARD
    constexpr
    auto
    operator==(
        const T rhs
    ) const noexcept -> bool
    {
        return code_ == static_cast<long_t>(rhs);
    }

    NODISCARD
    constexpr
    auto
    operator!=(
        const status_code& rhs
    ) const noexcept -> bool
    {
        return !(*this == rhs);
    }

    template<std::integral T>
    NODISCARD
    constexpr
    auto
    operator!=(
        const T rhs
    ) const noexcept -> bool
    {
        return !(*this == rhs);
    }

    NODISCARD
    constexpr
    auto
    operator<=>(
        const status_code& rhs
    ) const noexcept -> std::strong_ordering
    {
        return operator<=><long_t>(rhs.code_);
    }

    template<std::integral T>
    NODISCARD
    constexpr
    auto
    operator<=>(
        const T rhs
    ) const noexcept -> std::strong_ordering
    {
        const auto other = static_cast<long_t>(rhs);

        return code_ == other
            ? std::strong_ordering::equal
            : code_ > other
                ? std::strong_ordering::greater
                : std::strong_ordering::less;
    }

    NODISCARD
    constexpr
    explicit
    operator
    bool() const noexcept
    {
        return success();
    }

    NODISCARD
    constexpr
    auto
    operator*() const noexcept -> long_t
    {
        return code_;
    }

    NODISCARD
    constexpr
    auto
    get() const noexcept -> long_t
    {
        return code_;
    }

    template<std::integral T>
    NODISCARD
    constexpr
    auto
    is(
        const T status
    ) const noexcept -> bool
    {
        return code_ == static_cast<long_t>(status);
    }

    constexpr
    auto
    release() noexcept -> long_t
    {
        const auto old = code_;

        code_ = 0;

        return old;
    }

    constexpr
    auto
    reset(
        const long_t value = 0
    ) noexcept -> status_code&
    {
        code_ = value;

        return *this;
    }

    NODISCARD
    constexpr
    auto
    success() const noexcept -> bool
    {
        return is(0);
    }

    NODISCARD
    constexpr
    auto
    info_length_mismatch() const noexcept -> bool
    {
        return is(0xC0000004);
    }

    NODISCARD
    constexpr
    auto
    access_violation() const noexcept -> bool
    {
        return is(0xC0000005);
    }

    NODISCARD
    constexpr
    auto
    invalid_parameter() const noexcept -> bool
    {
        return is(0xC000000D);
    }

    NODISCARD
    constexpr
    auto
    invalid_system_service() const noexcept -> bool
    {
        return is(0xC000001C);
    }

    NODISCARD
    constexpr
    auto
    access_denied() const noexcept -> bool
    {
        return is(0xC0000022);
    }

private:
    long_t code_{};
};
} //namespace zen
