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

CSF_FUNC(int, __fastcall, CharTask_StartMovement, 0x004DB140, (void* self, void* edx, int a2));

// Sets the 'pending' flag on the owner's state word, stores the three-dword
// target position at +0x3C, then starts the character's movement task.
static int __fastcall C_0082f9f8_StartMoveToPosition(C_0082f9f8* self, void* /*edx*/, uint32_t* a2)
{
    *reinterpret_cast<uint32_t*>(self->field_0008 + 0x18) |= 1u;

    CSF_AT(uint32_t, self, 0x3C) = a2[0];
    CSF_AT(uint32_t, self, 0x40) = a2[1];
    CSF_AT(uint32_t, self, 0x44) = a2[2];

    return CharTask_StartMovement(self, nullptr, reinterpret_cast<int>(&self->field_0014));
}

HOOK(0x004D8770, C_0082f9f8_StartMoveToPosition, "auto");
