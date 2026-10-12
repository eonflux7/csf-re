#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Serializes the base fields of the record (thiscall-style, one stack argument).
CSF_FUNC(int, __fastcall, Archive_SerializeBaseFields, 0x004DB4F0, (void* self, void* edx, int a2));

// Serializes a record: asks the owner whether it rejects the record, serializes base fields,
// lets the owner process the record payload, then asks the owner whether processing has finished.
// Returns 1 while the record is still pending, 0 when it finished.
static int __fastcall ArchiveRecord_SerializeAndProcess(S_0041f0a0* self, void* /*edx*/, S_0041f0a0* a2) {
    void* request = reinterpret_cast<void*>(a2);
    void* savedPendingPointer = CSF_AT(void*, request, 0x1C);

    // Pre-check: if the record flag is clear, ask the owner (vtbl slot 32) whether it rejects the record.
    bool rejectedByOwner = false;
    if (CSF_AT(uint8_t, request, 0) == 0) {
        void* owner = CSF_AT(void*, request, 4);
        uint8_t ownerRejects =
            ((uint8_t(__fastcall*)(void*, void*, int, void*))CSF_AT(void**, owner, 0)[32])(
                owner, nullptr, 0, request);
        rejectedByOwner = (ownerRejects == 0);
    }
    CSF_AT(uint8_t, request, 0) = rejectedByOwner ? 1 : 0;

    Archive_SerializeBaseFields(reinterpret_cast<void*>(self), nullptr, reinterpret_cast<int>(request));

    // Clear the pending pointer while the payload is processed; restored below.
    CSF_AT(void*, request, 0x1C) = nullptr;

    // If the record is still clear, let the owner (vtbl slot 17) process it with this object's sub-block at +0x10.
    if (CSF_AT(uint8_t, request, 0) == 0) {
        CSF_AT(uint8_t, request, 0x15) = 0;
        CSF_AT(uint8_t, request, 0x16) = 0;
        void* owner = CSF_AT(void*, request, 4);
        int processResult =
            ((int(__fastcall*)(void*, void*, void*, void*))CSF_AT(void**, owner, 0)[17])(
                owner, nullptr, reinterpret_cast<uint8_t*>(self) + 0x10, request);
        CSF_AT(uint8_t, request, 0) = (processResult == -1) ? 1 : 0;
    }

    bool stillClear = CSF_AT(uint8_t, request, 0) == 0;
    CSF_AT(void*, request, 0x1C) = savedPendingPointer;

    if (stillClear) {
        void* owner = CSF_AT(void*, request, 4);
        uint8_t finished =
            ((uint8_t(__fastcall*)(void*, void*, void*))CSF_AT(void**, owner, 0)[31])(
                owner, nullptr, request);
        if (finished) {
            CSF_AT(uint8_t, request, 0) = 0;
            return 0;
        }
    }

    CSF_AT(uint8_t, request, 0) = 1;
    return 1;
}

HOOK(0x00423F30, ArchiveRecord_SerializeAndProcess, "auto");
