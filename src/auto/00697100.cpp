#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee: sub_697070, __thiscall with one stack argument (ret 4), bound as __fastcall.
CSF_FUNC(char, __fastcall, sub_697070, 0x00697070, (void* self, void* edx, uint8_t* a2));

// Original: 0x00697100 FormatterSetPattern (__thiscall, ret 4)
static int __fastcall S_004f21d0_SetPattern(S_004f21d0* self, void* /*edx*/, uint8_t* a2)
{
    self->field_0014 = 0;
    self->field_0018 = 7;
    self->field_0004 = 0;
    sub_697070(self, nullptr, a2);
    return (int)(uintptr_t)self;
}

HOOK(0x00697100, S_004f21d0_SetPattern, "auto");
