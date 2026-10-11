// Module: audio. Stores a 3-dword parameter block into the audio object's
// buffer at +0x58 and forwards the buffer's current handle (+0x30) to the
// audio dispatch routine when it is non-null.
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

// Audio dispatch routine (__cdecl, 4 args, no stack cleanup by callee).
CSF_FUNC(uint32_t*, __cdecl, Audio_DispatchEffectCommand, 0x007E1320,
         (uint32_t* a1, int a2, int a3, int a4));

static int __fastcall C_00863dd0_StoreEffectParamsAndDispatch(C_00863dd0* self, void* /*edx*/, int* a2)
{
    // Read the three parameters from the caller's block.
    int paramX = a2[0];
    int paramY = a2[1];
    int paramZ = a2[2];

    // Keep a local copy of the triple to pass by address to the dispatcher.
    int triple[3];
    triple[0] = paramX;
    triple[1] = paramY;
    triple[2] = paramZ;

    // Store the triple at field_0004 + 0x58 (slots 22..24 as dwords).
    void* block = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0004));
    CSF_AT(uint32_t, block, 0x58) = static_cast<uint32_t>(paramX);
    CSF_AT(uint32_t, block, 0x5C) = static_cast<uint32_t>(paramY);
    CSF_AT(uint32_t, block, 0x60) = static_cast<uint32_t>(paramZ);

    // Dispatch only if the buffer's handle at +0x30 is set.
    uint32_t handle = CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0004)), 0x30);
    if (handle != 0) {
        uint32_t* dispatchResult = Audio_DispatchEffectCommand(
            reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(handle)),
            0,
            1,
            static_cast<int>(reinterpret_cast<uintptr_t>(triple)));
        return static_cast<int>(reinterpret_cast<uintptr_t>(dispatchResult));
    }
    return 0;
}

HOOK(0x00712A50, C_00863dd0_StoreEffectParamsAndDispatch, "auto");
