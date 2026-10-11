#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Base-class handler at 0x65E710, bound as given in the packet.
CSF_FUNC(void, __stdcall, sub_65E710, 0x0065E710, (int a1, int a2));

// Calls the object's virtual slot 7 (byte offset 0x1C) with the given state index.
// Written out per call so the slot is a fixed compile-time index.
static int __stdcall C_00867800_SetStates1To4AndCallBase(int a1, int a2)
{
    void* self = reinterpret_cast<void*>(a1);

    if (a2 == 0)
    {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[7])(self, nullptr, 1);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[7])(self, nullptr, 2);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[7])(self, nullptr, 3);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[7])(self, nullptr, 4);
        sub_65E710(a1, a2);
        return 0;
    }

    if (a2 == 1)
    {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[7])(self, nullptr, 1);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[7])(self, nullptr, 2);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[7])(self, nullptr, 3);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[7])(self, nullptr, 4);
    }

    sub_65E710(a1, a2);
    return 0;
}

HOOK(0x007422E0, C_00867800_SetStates1To4AndCallBase, "auto");
