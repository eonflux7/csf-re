#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// Refreshes the object's derived state; the original discards its result.
CSF_FUNC(uint8_t*, __fastcall, S_004f0cd0_RefreshState, 0x004F0F50, (void* self, void* edx));
// Copies the resolved slot pointer into the caller's output block.
CSF_FUNC(uint32_t*, __fastcall, S_004f0cd0_CopyFromSlot, 0x004F10F0, (void* self, void* edx, int a2));
// Returns whether the requested slot index is valid for this object.
CSF_FUNC(bool, __fastcall, S_004f0cd0_IsSlotValid, 0x004F1340, (void* self, void* edx, int a2));

// Resolves slot a2 (0, 1 or 2) inside the object's pad block, copies it to a3 and reports success.
static bool __fastcall S_004f0cd0_ResolveSlotIntoOutput(S_004f0cd0* self, void* /*edx*/, int a2, uint32_t* a3)
{
    // Pending fixup: a negative value below 0.0 (or NaN, as the unordered compare) resets to 0.5 and refreshes.
    if (self->field_0040 != 0 && !(self->field_003c >= g_fZero))
    {
        self->field_003c = 0.5f;
        S_004f0cd0_RefreshState(self, nullptr);
    }

    a3[2] = 0;

    bool isValid = S_004f0cd0_IsSlotValid(self, nullptr, a2);
    if (!isValid)
        return false;

    uint8_t* slot = nullptr;
    if (a2 == 0)
    {
        slot = &self->pad_000c[32];
    }
    else if (a2 == 1)
    {
        slot = &self->pad_000c[16];
    }
    else if (a2 == 2)
    {
        slot = self->pad_000c;
    }

    bool found = (slot != nullptr);
    if (found)
    {
        S_004f0cd0_CopyFromSlot(a3, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(slot)));
    }
    return found;
}

HOOK(0x004F1530, S_004f0cd0_ResolveSlotIntoOutput, "auto");
