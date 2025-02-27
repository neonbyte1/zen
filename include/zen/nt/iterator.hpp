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

ZEN_WIN32_ALIGNMENT(zen::win)
template<class T, std::integral SizeType = u16>
class iterator
{
public:
    using value_type    = T;
    using pointer       = T*;
    using const_pointer = const T*;
    using size_type     = SizeType;

    constexpr
    iterator() noexcept = default;

    constexpr
    iterator(
        pointer const   base,
        const size_type count
    ) noexcept
        : base_{base}
        , count_{count}
    {}

    NODISCARD
    constexpr
    auto
    begin() noexcept -> pointer
    {
        return base_;
    }

    NODISCARD
    constexpr
    auto
    begin() const noexcept -> const_pointer
    {
        return const_cast<iterator*>(this)->begin();
    }

    NODISCARD
    constexpr
    auto
    end() noexcept -> pointer
    {
        return base_ + count_;
    }

    NODISCARD
    constexpr
    auto
    end() const noexcept -> const_pointer
    {
        return const_cast<iterator*>(this)->end();
    }

private:
    pointer   base_{};
    size_type count_{};
};
ZEN_RESTORE_ALIGNMENT() //namespace zen::win
