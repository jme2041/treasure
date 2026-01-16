// test_treasure.cpp
// Treasure's Demonstration Library
// Copyright (C) 2026 Jeffrey M. Engelmann

#include "treasure/treasure.h"

int main()
{
    using namespace treasure;

    auto const s = get_system_message<std::string>(0, 0x0409);
    if (!s || *s != "The operation completed successfully.") return 1;

    auto const w = get_system_message<std::wstring>(0, 0x0409);
    if (!w || *w != L"The operation completed successfully.") return 1;

    if (get_system_message<std::string>(0xFFFFFFFF, 0x0409)) return 1;

    if (get_system_message<std::wstring>(0xFFFFFFFF, 0x0409)) return 1;

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
