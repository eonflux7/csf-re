#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callee at 0x405450: processes the archive context for the request (stdcall, one stack argument).
CSF_FUNC(int, __stdcall, AudioSoundManager_ProcessArchiveContext, 0x00405450, (void* context));

// Advances an AI state request: asks the owner whether it rejects the request, processes the
// archive context, tries to start the request through the owner, and finally asks the owner
// whether the request has finished. Returns 1 while the request is still pending, 0 when it finished.
static int __fastcall AiState_AdvanceRequest(void* self, void* /*edx*/, int a2) {
    void* request = reinterpret_cast<void*>(a2);
    int savedOwnerSlot = CSF_AT(int, request, 0x1C);

    // Pre-check: if the request flag is clear, ask the owner (vtbl slot 32) whether it rejects the request.
    bool rejectedByOwner = false;
    if (CSF_AT(uint8_t, request, 0) == 0) {
        void* owner = CSF_AT(void*, request, 4);
        uint8_t ownerRejects =
            ((uint8_t(__fastcall*)(void*, void*, int, int))CSF_AT(void**, owner, 0)[32])(
                owner, nullptr, 0, a2);
        rejectedByOwner = (ownerRejects == 0) ? true : false;
    }
    CSF_AT(uint8_t, request, 0) = rejectedByOwner ? 1 : 0;

    AudioSoundManager_ProcessArchiveContext(reinterpret_cast<void*>(a2));

    // Clear the owner slot while the start attempt runs; restored below.
    if (CSF_AT(uint8_t, request, 0) == 0) {
        CSF_AT(int, request, 0x1C) = 0;
    } else {
        CSF_AT(int, request, 0x1C) = 0;
    }

    // If the request is still clear, try to start it through the owner (vtbl slot 17).
    if (CSF_AT(uint8_t, request, 0) == 0) {
        CSF_AT(uint8_t, request, 0x15) = 0;
        CSF_AT(uint8_t, request, 0x16) = 0;
        void* owner = CSF_AT(void*, request, 4);
        int startResult =
            ((int(__fastcall*)(void*, void*, void*, int))CSF_AT(void**, owner, 0)[17])(
                owner, nullptr, reinterpret_cast<uint8_t*>(self) + 4, a2);
        CSF_AT(uint8_t, request, 0) = (startResult == -1) ? 1 : 0;
    }

    bool stillClear = CSF_AT(uint8_t, request, 0) == 0;
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

HOOK(0x004C17F0, AiState_AdvanceRequest, "auto");
