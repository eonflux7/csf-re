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

// Callee: bound by address, signature as given in the packet.
CSF_FUNC(char, __fastcall, WaterFx_RunPassCore, 0x0070CE50, (void* self, void* edx, void* a2, int a3));

// Original 0x7097B0: __thiscall with two stack args (a2, a3), callee pops 8.
// Calls this->vtbl[0x24/4] (reset/notify hook), zeroes two state dwords,
// runs the pass core, marks the object as done, and calls the vtable hook again on failure.
static char __fastcall WaterFx_ProcessAndRelease(S_006fe0c0* self, void* /*edx*/, void* a2, int a3)
{
    // vtable slot at byte offset 0x24 -> index 9
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x24 / 4])(self, nullptr);

    self->field_0100 = 0;
    self->field_0104 = 0;

    char succeeded = WaterFx_RunPassCore(self, nullptr, a2, a3);

    self->field_00d4 = 1;

    if (!succeeded)
    {
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x24 / 4])(self, nullptr);
    }

    return succeeded;
}

HOOK(0x7097B0, WaterFx_ProcessAndRelease, "auto");
