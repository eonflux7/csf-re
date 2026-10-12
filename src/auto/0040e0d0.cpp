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

CSF_FUNC(uint32_t*, __cdecl, AllocateSubobjectHandle, 0x0040A430, (int type));
CSF_FUNC(char, __fastcall, C_0082e520_PrepareInputForBinding, 0x0041E050, (C_0082e520* self, void* /*edx*/, int a2, int a3));

// __thiscall(this, a2, a3), ret 8
static char __fastcall C_0082dd78_InitSubobjectBinding(C_0082dd78* self, void* /*edx*/, int a2, int a3)
{
    uint32_t* subobjectHandle = AllocateSubobjectHandle(14);
    self->field_0030 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(subobjectHandle));

    // Virtual slot +0x8 of the handle object: bool Accepts(int)
    auto acceptsArg = reinterpret_cast<char(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, subobjectHandle, 0)[2]);
    if (!acceptsArg(subobjectHandle, nullptr, a2)) {
        return 0;
    }

    char result = C_0082e520_PrepareInputForBinding(static_cast<C_0082e520*>(self), nullptr, a2, a3);
    self->field_0039 = 1;
    self->field_0070 |= 0x20u;
    self->field_0034 = 0;
    self->field_0034 |= 1u;
    return result;
}

HOOK(0x0040E0D0, C_0082dd78_InitSubobjectBinding, "auto");
