#include <cstdint>
#include <cfloat>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Starts the movement task on the sub-object at +0x164, then, on the object it
// returns, sets two fields to their reset values and starts a second movement.
// Also clears two flag bytes on this task.
CSF_FUNC(int, __fastcall, CharTask_StartMovement, 0x004DB140, (void* self, void* edx, int a2));

static int __fastcall C_0082f9f8_StartMovementAndResetFlags(C_0082f9f8* self, void* /*edx*/, int /*a2*/)
{
    // Sub-object at this+0x164 (pad_0020[324]).
    C_0082f9f8* started = reinterpret_cast<C_0082f9f8*>(
        CharTask_StartMovement(self, nullptr, (int)(uintptr_t)((uint8_t*)self + 0x164)));

    // started->field_0008 is a pointer to a secondary object; reset two of its fields.
    uint8_t* linked = reinterpret_cast<uint8_t*>(started->field_0008);
    CSF_AT(float, linked, 0x2B0) = FLT_MAX;
    CSF_AT(uint32_t, linked, 0x2AC) = 0;

    // Start movement again using the started object's field_0014 as the argument.
    int result = CharTask_StartMovement(started, nullptr, (int)(uintptr_t)&started->field_0014);

    CSF_AT(uint8_t, self, 0x2A8) = 0;
    CSF_AT(uint8_t, self, 0x2ED) = 0;
    return result;
}

HOOK(0x0047BF90, C_0082f9f8_StartMovementAndResetFlags, "auto");
