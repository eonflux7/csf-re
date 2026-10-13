#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original 0x0044BA20: __fastcall-style, self in ecx, unused edx, one stack arg, ret 4.
CSF_FUNC(uint32_t*, __fastcall, AiAttackTasks_AppendTangent, 0x0044BA20, (void* self, void* edx, uint32_t* a3));

// Original forwards `this + 0x14` (sub-object) and the stack argument to AppendTangent (tail jump).
static uint32_t* __fastcall C_008311a0_AppendTangentToSubobject(C_008311a0* self, void* /*edx*/, uint32_t* a2)
{
    void* tangentSubobject = reinterpret_cast<uint8_t*>(self) + 0x14;
    return AiAttackTasks_AppendTangent(tangentSubobject, nullptr, a2);
}

HOOK(0x0044BAB0, C_008311a0_AppendTangentToSubobject, "auto");
