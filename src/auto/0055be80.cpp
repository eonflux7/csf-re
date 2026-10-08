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

// void sub_646D30(int, int, uint32_t, int) -- cdecl, caller cleans 0x10 bytes.
CSF_FUNC(void, __cdecl, Game_RaiseStatusNotice, 0x646D30, (int arg0, int arg1, uint32_t code, int arg3));

// Original: int __thiscall (this, int a2), ret 4.
// Reads a2->flags at +0x2C; if the low 22 bits are zero, raises a status
// notice and substitutes 0. Then dispatches virtual slot 6 (+0x18) on this
// with the resulting value and returns that result.
static int __fastcall CMenuAccount_DispatchWithFlags(C_008453a4* self, void* /*edx*/, int a2)
{
    uint32_t flags = *reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(a2) + 0x2C);
    int value = static_cast<int>(flags);

    if ((flags & 0x3FFFFF) == 0) {
        Game_RaiseStatusNotice(0, 0, 0xE000000Eu, 0);
        value = 0;
    }

    return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(CSF_AT(void**, self, 0)[6])(
        self, nullptr, value);
}

HOOK(0x0055BE80, CMenuAccount_DispatchWithFlags, "auto");
