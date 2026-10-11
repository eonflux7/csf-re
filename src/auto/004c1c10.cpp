#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callee at 0x405450: __stdcall, one stack argument, pops 4.
CSF_FUNC(int, __stdcall, AudioSoundManager_ProcessArchiveContext, 0x00405450, (void* context));

// Tries to start an AI state request. Returns 1 when the request is still pending,
// 0 when the state accepted it and finished starting.
static int __fastcall AiStateFactory_TryStartStateRequest(void* self, void* /*edx*/, int a2) {
    void* request = reinterpret_cast<void*>(a2);
    int savedOwnerSlot = CSF_AT(int, request, 0x1C);

    // Pre-check: when the flag is clear, ask the owner (vtbl slot 32) whether it rejects the request.
    bool flagFromOwner = false;
    if (CSF_AT(uint8_t, request, 0) == 0) {
        void* owner = CSF_AT(void*, request, 4);
        uint8_t ownerRet =
            ((uint8_t(__fastcall*)(void*, void*, int, int))CSF_AT(void**, owner, 0)[32])(
                owner, nullptr, 0, a2);
        flagFromOwner = (ownerRet == 0);
    }
    CSF_AT(uint8_t, request, 0) = flagFromOwner ? 1 : 0;

    AudioSoundManager_ProcessArchiveContext(reinterpret_cast<void*>(a2));

    bool stillClear = CSF_AT(uint8_t, request, 0) == 0;
    CSF_AT(int, request, 0x1C) = 0;
    if (stillClear) {
        CSF_AT(uint8_t, request, 0x15) = 0;
        CSF_AT(uint8_t, request, 0x16) = 0;
        void* owner = CSF_AT(void*, request, 4);
        int startResult =
            ((int(__fastcall*)(void*, void*, void*, int))CSF_AT(void**, owner, 0)[21])(
                owner, nullptr, reinterpret_cast<uint8_t*>(self) + 4, a2);
        CSF_AT(uint8_t, request, 0) = (startResult == -1) ? 1 : 0;
    }

    stillClear = CSF_AT(uint8_t, request, 0) == 0;
    CSF_AT(int, request, 0x1C) = savedOwnerSlot;

    if (stillClear) {
        void* owner = CSF_AT(void*, request, 4);
        uint8_t finished =
            ((uint8_t(__fastcall*)(void*, void*, int))CSF_AT(void**, owner, 0)[31])(
                owner, nullptr, a2);
        if (finished) {
            CSF_AT(uint8_t, request, 0) = 0;
            return 0;
        }
    }

    CSF_AT(uint8_t, request, 0) = 1;
    return 1;
}

HOOK(0x004C1C10, AiStateFactory_TryStartStateRequest, "auto");
