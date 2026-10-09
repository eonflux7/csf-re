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

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void**, __fastcall, S_00778050_EraseRecordRange, 0x00778050, (void* self, void* edx, void** a4, void* arg4, void* arg8));
CSF_FUNC(char, __fastcall, S_00778050_AppendCopies, 0x007784C0, (void* self, void* edx, void* a1, unsigned int a3, void* arg8));

// Number of 44-byte records between two record addresses (signed division, as in the original).
static int RecordCountBetween(uint32_t firstRecord, uint32_t endRecord) {
    return static_cast<int>(endRecord - firstRecord) / 44;
}

static void* AsAddress(uint32_t address) {
    return reinterpret_cast<void*>(static_cast<uintptr_t>(address));
}

// Resizes the 44-byte record vector to newCount. Shrinking erases the tail;
// growing appends copies of fillValue at the end.
static char __fastcall S_00778050_ResizeRecords(
        S_00778050* self, void* /*edx*/,
        unsigned int newCount,
        C_0082d410 fillValue,
        [[maybe_unused]] int unusedArg4,
        [[maybe_unused]] int unusedArg5,
        [[maybe_unused]] int unusedArg6,
        unsigned int fillValueCapacity,
        [[maybe_unused]] int unusedArg8,
        void* blockToFree,
        [[maybe_unused]] int unusedArg10,
        [[maybe_unused]] int unusedArg11)
{
    // Per the packet: referenced in the original's call chain; no memory access.
    (void)CSF_VTBL(0x00778050);

    uint32_t recordsBegin = self->field_0004;
    unsigned int currentCount = 0;
    if (recordsBegin)
        currentCount = RecordCountBetween(recordsBegin, self->field_0008);

    if (currentCount >= newCount) {
        if (recordsBegin &&
            newCount < static_cast<unsigned int>(RecordCountBetween(recordsBegin, self->field_0008))) {
            S_00778050_EraseRecordRange(self, nullptr,
                                        reinterpret_cast<void**>(&newCount),
                                        AsAddress(recordsBegin + 44u * newCount),
                                        AsAddress(self->field_0008));
        }
    } else {
        int existingCount = 0;
        if (recordsBegin)
            existingCount = RecordCountBetween(recordsBegin, self->field_0008);
        S_00778050_AppendCopies(self, nullptr,
                                AsAddress(self->field_0008),
                                newCount - static_cast<unsigned int>(existingCount),
                                static_cast<void*>(&fillValue));
    }

    char result = 0;
    if (blockToFree)
        result = static_cast<char>(Mem_FreeDebug(blockToFree));

    if (fillValueCapacity >= 0x10) {
        uint32_t fillBuffer = fillValue.field_0004;
        result = static_cast<char>(fillBuffer);
        if (fillBuffer)
            return static_cast<char>(Mem_FreeDebug(AsAddress(fillBuffer)));
    }
    return result;
}

HOOK(0x00778860, S_00778050_ResizeRecords, "auto");
