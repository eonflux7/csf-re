#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __fastcall, S_00521df0_SubmitSoundRequest, 0x005242B0, (S_00521df0* self, void* , void* request, int listenerInfo));
CSF_FUNC(int, __fastcall, ResolveSlotIndexForState, 0x00756810, (void* self, void* , int slotIndex, int stateCode));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

// Builds a sound request for the object attached at +0x14 (using its id pair and state),
// submits it to the sound manager and stores the resulting handle.
static int __fastcall StartAttachedObjectSoundRequest(void* self, void* /*edx*/)
{
    // Request descriptor (0x38 bytes of frame room).
    alignas(8) uint8_t requestBytes[0x38] = {};
    // Listener / source info (0x10 bytes of frame room).
    alignas(8) uint8_t sourceInfoBytes[0x10] = {};

    void* attached = CSF_AT(void*, self, 0x14);

    CSF_AT(int, requestBytes, 0x28) = -1;
    CSF_AT(int, requestBytes, 0x2C) = -1;
    CSF_AT(int, requestBytes, 0x08) = -1;
    CSF_AT(int, requestBytes, 0x0C) = -1;
    CSF_AT(int, requestBytes, 0x18) = 0;
    CSF_AT(int, requestBytes, 0x1C) = 0;
    CSF_AT(int, requestBytes, 0x20) = 0;
    CSF_AT(void**, requestBytes, 0x24) = CSF_VTBL(0x0082D448);
    CSF_AT(int, requestBytes, 0x04) = 1;
    CSF_AT(int, requestBytes, 0x10) = 0;
    CSF_AT(int, requestBytes, 0x14) = 0;
    CSF_AT(uint8_t, requestBytes, 0x34) = 0;
    CSF_AT(uint8_t, requestBytes, 0x35) = 1;
    CSF_AT(uint8_t, requestBytes, 0x36) = 1;
    CSF_AT(int, requestBytes, 0x30) = 0;
    CSF_AT(int, requestBytes, 0x00) = 2;

    CSF_AT(int, requestBytes, 0x28) = CSF_AT(int, attached, 0x20);
    CSF_AT(int, requestBytes, 0x2C) = CSF_AT(int, attached, 0x24);
    int stateCode = CSF_AT(int, attached, 0x50);

    uint8_t* globalBase = reinterpret_cast<uint8_t*>(GetGlobalValue_8BC1E0());
    CSF_AT(int, requestBytes, 0x08) = ResolveSlotIndexForState(globalBase + 0x22A4, nullptr, 2, stateCode);

    CSF_AT(int, sourceInfoBytes, 0x04) = CSF_AT(int, requestBytes, 0x28);
    CSF_AT(uint8_t, requestBytes, 0x36) = 0;
    CSF_AT(void**, sourceInfoBytes, 0x00) = CSF_VTBL(0x0082D448);
    CSF_AT(int, sourceInfoBytes, 0x08) = CSF_AT(int, requestBytes, 0x2C);
    CSF_AT(float, sourceInfoBytes, 0x0C) = 1000004.0f;

    S_00521df0* soundManager = reinterpret_cast<S_00521df0*>(GetGlobalValue_521BB0());
    int handle = S_00521df0_SubmitSoundRequest(soundManager, nullptr, requestBytes,
                                               reinterpret_cast<int>(sourceInfoBytes));

    CSF_AT(int, self, 0x20) = 1;
    CSF_AT(int, self, 0x28) = handle;
    return handle;
}

HOOK(0x00425640, StartAttachedObjectSoundRequest, "auto");
