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

// Releases the currently playing sound handle (if any) and submits a new sound request
// built from the owner's data, then stores the new handle and switches to state 3.
static int __fastcall SceneObject_SubmitStateSoundRequest(void* self, void* /*edx*/)
{
    void* currentHandle = CSF_AT(void*, self, 0x28);
    if (currentHandle != nullptr)
    {
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, currentHandle, 0)[0x4C / 4])(currentHandle, nullptr);
        CSF_AT(void*, self, 0x28) = nullptr;
    }

    // Request descriptor (0x38 bytes on the original frame) with an embedded C_0082d448 at +0x24.
    alignas(8) uint8_t requestBytes[0x38];
    // Listener/position info (0x10 bytes on the original frame), also a C_0082d448-style object.
    alignas(8) uint8_t listenerBytes[0x10];

    CSF_AT(int, requestBytes, 0x28) = -1;
    CSF_AT(int, requestBytes, 0x2C) = -1;
    CSF_AT(int, requestBytes, 0x08) = -1;
    CSF_AT(int, requestBytes, 0x0C) = -1;

    void* owner = CSF_AT(void*, self, 0x14);

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

    CSF_AT(int, requestBytes, 0x28) = CSF_AT(int, owner, 0x20);
    CSF_AT(int, requestBytes, 0x2C) = CSF_AT(int, owner, 0x24);
    int stateCode = CSF_AT(int, owner, 0x50);

    void* slotTable = reinterpret_cast<uint8_t*>(GetGlobalValue_8BC1E0()) + 0x22A4;
    CSF_AT(int, requestBytes, 0x08) = ResolveSlotIndexForState(slotTable, nullptr, 3, stateCode);

    CSF_AT(int, listenerBytes, 0x04) = CSF_AT(int, requestBytes, 0x28);
    CSF_AT(uint8_t, requestBytes, 0x36) = 1;
    CSF_AT(void**, listenerBytes, 0x00) = CSF_VTBL(0x0082D448);
    CSF_AT(int, listenerBytes, 0x08) = CSF_AT(int, requestBytes, 0x2C);
    CSF_AT(float, listenerBytes, 0x0C) = 1000004.0f;

    S_00521df0* soundManager = reinterpret_cast<S_00521df0*>(GetGlobalValue_521BB0());
    int newHandle = S_00521df0_SubmitSoundRequest(soundManager, nullptr, requestBytes,
                                                  static_cast<int>(reinterpret_cast<uintptr_t>(listenerBytes)));

    CSF_AT(int, self, 0x28) = newHandle;
    CSF_AT(int, self, 0x20) = 3;
    return newHandle;
}

HOOK(0x00425710, SceneObject_SubmitStateSoundRequest, "auto");
