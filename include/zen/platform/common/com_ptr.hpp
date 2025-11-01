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

namespace zen::win {
template<class T>
#if defined(__IUnknown_FWD_DEFINED__)
requires(std::is_base_of_v<IUnknown, T>)
#endif
class com_ptr
{
public:
    using value_type    = T;
    using pointer       = T*;

private:
    NODISCARD
    static
    auto
    add_with_ref(
        pointer const ptr
    ) noexcept -> pointer
    {
        if (ptr) {
            ptr->AddRef();
        }

        return ptr;
    }

public:
    constexpr
    com_ptr() noexcept = default;

    explicit
    com_ptr(
        pointer const ptr,
        const bool    ref = false
    ) noexcept
        : data_{ref ? add_with_ref(ptr) : ptr}
    {}

    com_ptr(
        const com_ptr& rhs
    ) noexcept
        : data_{add_with_ref(rhs.get())}
    {}

    com_ptr(
        com_ptr&& rhs
    ) noexcept
        : data_{rhs.get()}
    {
        rhs.data_ = nullptr;
    }

    ~com_ptr() noexcept
    {
        release();
    }

    auto
    operator=(
        const com_ptr& rhs
    ) noexcept -> com_ptr&
    {
        if (this != std::addressof(rhs)) {
            reset(add_with_ref(rhs.get()));
        }

        return *this;
    }

    auto
    operator=(
        com_ptr&& rhs
    ) noexcept -> com_ptr&
    {
        if (this != std::addressof(rhs)) {
            reset(rhs.get());

            rhs.data_ = nullptr;
        }

        return *this;
    }

    NODISCARD
    constexpr
    explicit
    operator
    bool() const noexcept
    {
        return valid();
    }

    NODISCARD
    constexpr
    auto
    operator->() const noexcept -> pointer
    {
        return get();
    }

    NODISCARD
    constexpr
    auto
    valid() const noexcept -> bool
    {
        return data_ != nullptr;
    }

    NODISCARD
    constexpr
    auto
    get() const noexcept -> pointer
    {
        return data_;
    }

    NODISCARD
    auto
    address_of() noexcept -> pointer*
    {
        return &data_;
    }

    template<class Ret>
    NODISCARD
    auto
    address_of() noexcept -> Ret
    {
        return reinterpret_cast<Ret>(&data_);
    }

    auto
    release() noexcept -> pointer
    {
        auto* const old = get();

        if (old) {
            data_ = nullptr;

            old->Release();
        }

        return old;
    }

    auto
    reset(
        pointer const value = nullptr
    ) noexcept -> com_ptr&
    {
        if (value != data_) {
            release();

            data_ = value;
        }

        return *this;
    }

private:
    pointer data_{};
};
} //namespace zen::win
