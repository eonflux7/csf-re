#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original 0x007F0F00: __cdecl, two stack args (field pointer, value).
CSF_FUNC(uint32_t*, __cdecl, sub_7F0F00, 0x007F0F00, (uint32_t* a1, int a2));

// __thiscall, one stack arg, returns the byte 1 (ret 4).
static char __fastcall C_00863ac0_ForwardEffectParam(C_00863ac0* self, void* /*edx*/, int a2)
{
    sub_7F0F00(reinterpret_cast<uint32_t*>(self->field_0004), a2);
    return 1;
}

HOOK(0x0070ECF0, C_00863ac0_ForwardEffectParam, "auto");
