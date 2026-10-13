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

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(void, __fastcall, RpPrtStd_ResetBaseState, 0x006C9FC0, (void* self, void* edx));

// Releases the clump-manager-owned object at +0x20 and the owned object at +0x34,
// then resets the base render state.
static void __fastcall C_00862390_ReleaseClumpAndResetBase(C_00862390* self, void* /*edx*/)
{
    if (self->field_0020)
    {
        int globalInt = GetGlobalInt();
        int clumpManager = ResolveClumpManager(globalInt, nullptr);
        // Virtual slot +0x2C on the clump manager: release(objectAt+0x20)
        using ReleaseFn = void(__fastcall*)(void* self, void* edx, uint32_t object);
        ReleaseFn release = (ReleaseFn)CSF_AT(void**, (void*)clumpManager, 0)[0x2C / 4];
        release((void*)clumpManager, nullptr, self->field_0020);
        self->field_0020 = 0;
    }

    void** owned = reinterpret_cast<void**>(self->field_0034);
    if (owned)
    {
        // Virtual slot +0x00 on the owned object: destroy(1)
        using DestroyFn = void(__fastcall*)(void* self, void* edx, int flags);
        DestroyFn destroy = (DestroyFn)CSF_AT(void**, owned, 0)[0];
        destroy(owned, nullptr, 1);
        self->field_0034 = 0;
    }

    RpPrtStd_ResetBaseState(reinterpret_cast<void*>(self), nullptr);
}

HOOK(0x006F0CF0, C_00862390_ReleaseClumpAndResetBase, "auto");
