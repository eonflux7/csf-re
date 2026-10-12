#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void**, __fastcall, S_00772940_EraseRecordRange, 0x00772940, (void* self, void* edx, void** a4, void* arg4, void* arg8));
CSF_FUNC(char, __fastcall, S_00772940_AppendCopies, 0x007729E0, (void* self, void* edx, void* a1, unsigned int a3, void* arg8));

static int RecordCountBetween(uint32_t firstRecord, uint32_t endRecord) {
    return static_cast<int>(endRecord - firstRecord) / 44;
}

static void* AsAddress(uint32_t address) {
    return reinterpret_cast<void*>(static_cast<uintptr_t>(address));
}

static char __fastcall S_00772940_ResizeRecords(
        S_00772940* self, void* /*edx*/,
        unsigned int newCount,
        legacy::string fillValue,
        [[maybe_unused]] int unusedArg8,
        void* blockToFree,
        [[maybe_unused]] int unusedArg10,
        [[maybe_unused]] int unusedArg11)
{
    static_assert(sizeof(legacy::string) == 28);
    (void)CSF_VTBL(0x00772940);

    uint32_t recordsBegin = self->field_0004;
    unsigned int currentCount = 0;
    if (recordsBegin)
        currentCount = RecordCountBetween(recordsBegin, self->field_0008);

    if (currentCount >= newCount) {
        if (recordsBegin &&
            newCount < static_cast<unsigned int>(
                RecordCountBetween(recordsBegin, self->field_0008))) {
            S_00772940_EraseRecordRange(
                self, nullptr, reinterpret_cast<void**>(&newCount),
                AsAddress(recordsBegin + 44u * newCount),
                AsAddress(self->field_0008));
        }
    } else {
        int existingCount = 0;
        if (recordsBegin)
            existingCount = RecordCountBetween(recordsBegin, self->field_0008);

        S_00772940_AppendCopies(
            self, nullptr, AsAddress(self->field_0008),
            newCount - static_cast<unsigned int>(existingCount),
            static_cast<void*>(&fillValue));
    }

    char result = 0;
    if (blockToFree)
        result = static_cast<char>(Mem_FreeDebug(blockToFree));

    if (fillValue.res >= 0x10) {
        uint32_t fillBuffer = reinterpret_cast<uint32_t>(fillValue.bx.ptr);
        result = static_cast<char>(fillBuffer);
        if (fillBuffer)
            return static_cast<char>(Mem_FreeDebug(AsAddress(fillBuffer)));
    }
    return result;
}

HOOK(0x00772FA0, S_00772940_ResizeRecords, "auto");
