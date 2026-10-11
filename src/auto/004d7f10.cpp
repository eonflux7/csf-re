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

// Callee: C_0082f9f8::Start (thiscall, 2 stack args -> ret 8)
CSF_FUNC(int, __fastcall, C_0082f9f8_Start, 0x004226C0, (C_0082f9f8* self, void* , float a2, char a3));

// Forwards to C_0082f9f8::Start after marking the owner's flags word with bit 0.
// Original is a tail jmp, so the callee pops the 8 stack bytes (float + char).
static int __fastcall AutonomoTask_SetFlagAndStart(void* self, void* /*edx*/, float a2, char a3)
{
    void* owner = CSF_AT(void*, self, 0x08);
    CSF_AT(uint32_t, owner, 0x18) |= 1u;

    void* inner = CSF_AT(void*, owner, 0x14);
    C_0082f9f8* target = reinterpret_cast<C_0082f9f8*>(CSF_AT(void*, inner, 0x30));
    return C_0082f9f8_Start(target, nullptr, a2, a3);
}

HOOK(0x004D7F10, AutonomoTask_SetFlagAndStart, "auto");
