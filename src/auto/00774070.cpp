#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Tables_ClearRecordBuffers, 0x0051ABD0, (void* self, void*));
CSF_FUNC(void**, __fastcall, S_00773d00_EraseRecords, 0x00773D00, (void* self, void* edx, void** a2, char* a3, int a4));
CSF_FUNC(int, __fastcall, S_00773d00_InsertCopies, 0x00773D60, (void* self, void* edx, char* a1, unsigned int a3, int arg8));

// The packet also lists this address as a vtable, although the instruction
// referencing it is a direct call.
[[maybe_unused]] static const auto g_RecordEraseVtable = CSF_VTBL(0x00773D00);

// Preserve the complete 96-byte incoming record, including argument padding.
// Together with requestedCount this occupies the original 100 stack bytes.
struct alignas(4) AnimationDefs_RecordArgument
{
    uint8_t bytes[96];
};
static_assert(sizeof(AnimationDefs_RecordArgument) == 96);

static int __fastcall S_00773d00_Resize(
    S_00773d00* self,
    void* /*edx*/,
    unsigned int requestedCount,
    AnimationDefs_RecordArgument record)
{
    const uint32_t begin = self->field_0004;
    const uint32_t count = begin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(self->field_0008 - begin) / 96)
        : 0;

    if (count >= requestedCount)
    {
        if (begin)
        {
            const uint32_t end = self->field_0008;
            const uint32_t currentCount = static_cast<uint32_t>(
                static_cast<int32_t>(end - begin) / 96);

            if (requestedCount < currentCount)
            {
                S_00773d00_EraseRecords(
                    self, nullptr,
                    reinterpret_cast<void**>(&requestedCount),
                    reinterpret_cast<char*>(begin + 96 * requestedCount),
                    static_cast<int>(end));
            }
        }
    }
    else
    {
        const int32_t currentCount = begin
            ? static_cast<int32_t>(self->field_0008 - begin) / 96
            : 0;

        S_00773d00_InsertCopies(
            self, nullptr,
            reinterpret_cast<char*>(self->field_0008),
            requestedCount - static_cast<uint32_t>(currentCount),
            static_cast<int>(reinterpret_cast<uintptr_t>(record.bytes)));
    }

    return Tables_ClearRecordBuffers(record.bytes, nullptr);
}

HOOK(0x00774070, S_00773d00_Resize, "auto");
