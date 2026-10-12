#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Serializes the base fields of the archive node (thiscall, one stack argument).
CSF_FUNC(int, __fastcall, Archive_SerializeBaseFields, 0x004DB4F0, (void* self, void* edx, int a2));

// Runs one archive pass for a request: asks the owner whether it rejects the request, serializes the
// base fields, tries to start the request through the owner (vtbl slot 21), then asks the owner
// whether the request has finished (vtbl slot 31). Returns 1 while pending, 0 when finished.
static int __fastcall ArchiveRequest_RunPass(void* self, void* /*edx*/, int a2) {
    void* request = reinterpret_cast<void*>(a2);
    int savedOwnerSlot = CSF_AT(int, request, 0x1C);

    // Pre-check: if the request flag is clear, ask the owner (vtbl slot 32) whether it rejects the request.
    bool rejectedByOwner = false;
    if (CSF_AT(uint8_t, request, 0) == 0) {
        void* owner = CSF_AT(void*, request, 4);
        uint8_t ownerRejects =
            ((uint8_t(__fastcall*)(void*, void*, int, int))CSF_AT(void**, owner, 0)[32])(
                owner, nullptr, 0, a2);
        rejectedByOwner = (ownerRejects == 0);
    }
    CSF_AT(uint8_t, request, 0) = rejectedByOwner ? 1 : 0;

    Archive_SerializeBaseFields(self, nullptr, a2);

    // Clear the owner slot while the start attempt runs; restored below.
    CSF_AT(int, request, 0x1C) = 0;

    // If the request is still clear, try to start it through the owner (vtbl slot 21).
    if (CSF_AT(uint8_t, request, 0) == 0) {
        void* owner = CSF_AT(void*, request, 4);
        CSF_AT(uint8_t, request, 0x15) = 0;
        CSF_AT(uint8_t, request, 0x16) = 0;
        int startResult =
            ((int(__fastcall*)(void*, void*, void*, int))CSF_AT(void**, owner, 0)[21])(
                owner, nullptr, reinterpret_cast<uint8_t*>(self) + 0x10, a2);
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

HOOK(0x00423CA0, ArchiveRequest_RunPass, "auto");
