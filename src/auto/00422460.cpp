#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callees bound by address (names chosen here, signatures as given in the packet).
CSF_FUNC(void*, __fastcall, ResolveSubObject, 0x004DB190, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, ApplyScaledStep, 0x00421B30, (void* self, void* edx, float a2, char a3));

// __thiscall method: ecx = this, edx unused. Plain int return (eax is fully written).
static int __fastcall S_004223f0_RefreshSubStateIfFlagged(S_004223f0* self, void* /*edx*/)
{
    uint8_t* state = reinterpret_cast<uint8_t*>(self->field_0008);

    // Original reads the byte at +0x205 of the state block.
    if (CSF_AT(uint8_t, state, 0x205) != 0)
    {
        void* subObject = ResolveSubObject(self, nullptr, static_cast<int>(
            reinterpret_cast<uintptr_t>(state) + 0x1A8));

        // The original re-reads this->field_0008 after the call for the float parameter.
        uint8_t* stateAfter = reinterpret_cast<uint8_t*>(self->field_0008);
        float scale = CSF_AT(float, stateAfter, 0x208);

        return ApplyScaledStep(subObject, nullptr, scale, static_cast<char>(1));
    }

    return static_cast<int>(self->field_0008);
}

HOOK(0x00422460, S_004223f0_RefreshSubStateIfFlagged, "auto");
