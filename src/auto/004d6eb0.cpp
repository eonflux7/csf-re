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

CSF_FUNC(int, __fastcall, sub_4D1700, 0x004D1700, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_004D10F0_ResetFields, 0x004D10F0, (void* self, void*));
CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());

// Resets the autonomous task state of C_0083dbb0 and forwards to the
// reset of the sub-object embedded at +0x330.
static int __fastcall C_0083dbb0_ResetAutonomousState(C_0083dbb0* self, void* /*edx*/)
{
    sub_4D1700(self, nullptr);

    // Captured before the writes below; the writes do not touch field_0308.
    void* targetA = reinterpret_cast<void*>(self->field_0308);

    CSF_AT(uint32_t, self, 0x10) = 0;
    self->field_02f4 = 1;
    self->field_02f8 = 0;
    self->field_02fc = 0;
    self->field_0300 = 0;
    self->field_0304 = 0;

    // Virtual call, vtable slot 0x98 / 4 = 38, on the object stored at +0x308.
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, targetA, 0)[38])(targetA, nullptr);

    // Re-read +0x30C after the virtual call, then virtual slot 0x90 / 4 = 36.
    void* targetB = reinterpret_cast<void*>(self->field_030c);
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, targetB, 0)[36])(targetB, nullptr);

    // The original passes field_0310 in ecx; the bound signature takes no arguments.
    SceneLoader_NoOp();

    // Tail jump to C_004D10F0_ResetFields with this + 0x330 as its self.
    return C_004D10F0_ResetFields(reinterpret_cast<void*>((uint8_t*)self + 0x330), nullptr);
}

HOOK(0x4D6EB0, C_0083dbb0_ResetAutonomousState, "auto");
