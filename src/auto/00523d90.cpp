#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Vtable of the S_00523cf0 class (referenced by the original at 0x523DC5).
[[maybe_unused]] static void* const kSoundPlayWorkerVtable = CSF_VTBL(0x00523CF0);

// Callee at 0x523CF0: updates the request state held in `self`/`a2` before the start attempt.
CSF_FUNC(int, __fastcall, SoundPlayRequest_Evaluate, 0x00523CF0, (void* self, void* edx, int a2));

// Tries to start a queued sound play request. Returns 1 when the request is still pending
// (not started), 0 when the owner accepted it and it finished starting.
static int __fastcall SoundPlay_TryStartRequest(S_00523cf0* self, void* /*edx*/, int a2) {
    void* request = reinterpret_cast<void*>(a2);
    int savedOwnerSlot = CSF_AT(int, request, 0x1C);

    // Pre-check: if the request flag is clear, ask the owner (vtbl slot 32) whether it rejects the play.
    bool rejectedByOwner = false;
    if (CSF_AT(uint8_t, request, 0) == 0) {
        void* owner = CSF_AT(void*, request, 4);
        uint8_t ownerRejects =
            ((uint8_t(__fastcall*)(void*, void*, int, int))CSF_AT(void**, owner, 0)[32])(
                owner, nullptr, 0, a2);
        rejectedByOwner = (ownerRejects == 0) ? true : false;
    }
    CSF_AT(uint8_t, request, 0) = rejectedByOwner ? 1 : 0;

    SoundPlayRequest_Evaluate(self, nullptr, a2);

    // If the request is still clear after evaluation, try to start it (vtbl slot 21).
    if (CSF_AT(uint8_t, request, 0) == 0) {
        CSF_AT(uint8_t, request, 0x15) = 0;
        CSF_AT(uint8_t, request, 0x16) = 0;
        void* owner = CSF_AT(void*, request, 4);
        int startResult =
            ((int(__fastcall*)(void*, void*, void*, int))CSF_AT(void**, owner, 0)[21])(
                owner, nullptr, reinterpret_cast<uint8_t*>(self) + 8, a2);
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

HOOK(0x00523D90, SoundPlay_TryStartRequest, "auto");
