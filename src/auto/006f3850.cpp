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

CSF_GLOBAL(int, g_Value_8BB738, 0x008BB738);

CSF_FUNC(void, __fastcall, sub_6C9FC0, 0x006C9FC0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue_8BB738, 0x006E04A0, ());
CSF_FUNC(int, __fastcall, sub_6E04C0, 0x006E04C0, (void* self, void* edx, int a1, int a2));

// Clears the emitter's per-particle state. If a handle is set and the global
// is live, notifies the owner via GetValue_8BB738 and sub_6E04C0 before
// zeroing the fields, then chains to the base reset sub_6C9FC0.
static void __fastcall C_00862748_ReleaseEmitterState(C_00862748* self, void* /*edx*/)
{
    if (self->field_0024 && g_Value_8BB738)
    {
        uint32_t handle = self->field_0024;
        uint32_t ownerArg = self->field_002c;
        // The original pushes handle and ownerArg, calls GetValue_8BB738 (cdecl,
        // no args), then passes its result as `this` to sub_6E04C0 with the
        // pushed values as the stack arguments.
        void* owner = reinterpret_cast<void*>(static_cast<uintptr_t>(GetValue_8BB738()));
        sub_6E04C0(owner, nullptr, static_cast<int>(ownerArg), static_cast<int>(handle));
    }

    bool clearPointer = (self->field_0034 == 0);
    self->field_0024 = 0;
    self->field_0038 = 0;
    self->field_0028 = 0;
    if (clearPointer)
        self->field_0030 = 0;

    sub_6C9FC0(self, nullptr);
}

HOOK(0x006F3850, C_00862748_ReleaseEmitterState, "auto");
