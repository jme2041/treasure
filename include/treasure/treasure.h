// treasure.h
// Treasure's Demonstration Library
// Copyright (C) 2026 Jeffrey M. Engelmann

#pragma once
#ifndef TREASURE_H
#define TREASURE_H

#include <Windows.h>
#include <wil/resource.h>
#include <string>
#include <cctype>
#include <cwctype>
#include <optional>
#include <type_traits>
#include <algorithm>
#include <concepts>

namespace treasure {

    // Concept that defines a Windows-style string (CHAR or WCHAR)

    template<typename T>
    concept WindowsStringLike =
        std::is_same_v<T, std::string> ||
        std::is_same_v<T, std::wstring>;

    // Get a message from the operating system

    template<WindowsStringLike R>
    std::optional<R> get_system_message(
        DWORD const message_id,
        DWORD const language_id = LANG_NEUTRAL)
    {
        auto const is_not_space = [](auto const c) -> bool
            {
                if constexpr (std::is_same_v<R, std::string>)
                    return !std::isspace(c);
                else return !std::iswspace(c);
            };

        R ret;
        wil::unique_hlocal buf;

        if constexpr (std::is_same_v<R, std::string>)
        {
            if (!FormatMessageA(
                FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_ALLOCATE_BUFFER |
                FORMAT_MESSAGE_MAX_WIDTH_MASK,
                nullptr,
                message_id,
                language_id,
                reinterpret_cast<LPSTR>(&buf),
                0,
                nullptr) || !buf) return std::nullopt;

            ret = reinterpret_cast<char const*>(buf.get());
        }
        else
        {
            if (!FormatMessageW(
                FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_ALLOCATE_BUFFER |
                FORMAT_MESSAGE_MAX_WIDTH_MASK,
                nullptr,
                message_id,
                language_id,
                reinterpret_cast<LPWSTR>(&buf),
                0,
                nullptr) || !buf) return std::nullopt;

            ret = reinterpret_cast<wchar_t const*>(buf.get());
        }

        ret.erase(std::find_if(
            ret.rbegin(),
            ret.rend(),
            is_not_space).base(),
            ret.end());

        return ret;
    }
}

#endif  // TREASURE_H

///////////////////////////////////////////////////////////////////////////////
