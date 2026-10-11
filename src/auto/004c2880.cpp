#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Referenced by the original at 0x4C28B5 (`call 0x4c2380`), bound as the packet specifies.
[[maybe_unused]] static void* const kCoverAttackRecordVtable = CSF_VTBL(0x004C2380);

// Callee at 0x4C2380: processes the cover-attack record held by `self`/`a2`.
CSF_FUNC(int, __fastcall, CoverAttack_ProcessRecord, 0x004C2380, (void* self, void* edx, int a2));

// Updates the AI state request `a2` for the S_004c2380 state object. Re-evaluates the owner's
// accept/reject check, runs the cover-attack record processing, retries the owner start
// (vtbl slot 14) and finally asks the owner whether the state has finished (vtbl slot 31).
// Returns 0 while the state is still active (flag clear), 1 when it is done.
static int __fastcall S_004c2380_UpdateRequestState(S_004c2380* self, void* /*edx*/, int a2) {
    void* request = reinterpret_cast<void*>(a2);
    int savedOwnerSlot = CSF_AT(int, request, 0x1C);

    // Pre-check: if the flag is clear, ask the owner (vtbl slot 32) whether it rejects the request.
    bool rejectedByOwner = false;
    if (CSF_AT(uint8_t, request, 0) == 0) {
        void* owner = CSF_AT(void*, request, 4);
        uint8_t ownerRejects =
            ((uint8_t(__fastcall*)(void*, void*, int, int))CSF_AT(void**, owner, 0)[32])(
                owner, nullptr, 0, a2);
        rejectedByOwner = (ownerRejects == 0);
    }
    CSF_AT(uint8_t, request, 0) = rejectedByOwner ? 1 : 0;

    CoverAttack_ProcessRecord(self, nullptr, a2);

    // If still clear, retry the owner start (vtbl slot 14, owner at this+0x14).
    if (CSF_AT(uint8_t, request, 0) == 0) {
        CSF_AT(uint8_t, request, 0x15) = 0;
        CSF_AT(uint8_t, request, 0x16) = 0;
        void* owner = CSF_AT(void*, request, 4);
        int startResult =
            ((int(__fastcall*)(void*, void*, void*, int))CSF_AT(void**, owner, 0)[14])(
                owner, nullptr, reinterpret_cast<uint8_t*>(self) + 0x14, a2);
        CSF_AT(uint8_t, request, 0) = (startResult == -1) ? 1 : 0;
    }

    bool stillClear = CSF_AT(uint8_t, request, 0) == 0;
    CSF_AT(int, request, 0x1C) = savedOwnerSlot;

    // If still clear, ask the owner (vtbl slot 31) whether the state has finished.
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

HOOK(0x004C2880, S_004c2380_UpdateRequestState, "auto");
