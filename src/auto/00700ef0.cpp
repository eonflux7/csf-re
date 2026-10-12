#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, GfxObject_ApplyToRecordRange, 0x006B3E10, (int firstRecord, std::uint32_t recordCount, const void** context, int , int));
CSF_FUNC(int, __fastcall, SkinListInsertRange, 0x006B4F70, (void* self, void* edx, void** a1, unsigned int a3, void** arg8));

static int __fastcall C_008630e0_AppendTeamPlayerRecords(
    C_008630e0* self, void* /*edx*/, void* records)
{
    int remaining = CSF_AT(
        std::uint16_t, reinterpret_cast<void*>(self->field_0004), 0x0C);
    if (remaining == 0)
        return remaining;

    alignas(8) std::uint8_t contextBytes[0x0C];
    CSF_AT(std::uint8_t, contextBytes, 0x04) = 0;
    CSF_AT(C_008630e0*, contextBytes, 0x08) = self;

    C_008630e0* source = self;
    std::uint32_t recordOffset = 0;

    do
    {
        void* sourceRecords = CSF_AT(
            void*, reinterpret_cast<void*>(source->field_0004), 0x10);
        const std::uint32_t begin = CSF_AT(std::uint32_t, records, 0x04);

        CSF_AT(void*, contextBytes, 0) = CSF_AT(
            void*, static_cast<std::uint8_t*>(sourceRecords) + recordOffset, 0x08);

        std::uint32_t size = 0;
        if (begin != 0)
        {
            size = static_cast<std::uint32_t>(
                static_cast<std::int32_t>(
                    CSF_AT(std::uint32_t, records, 0x08) - begin) >> 3);
        }

        if (begin != 0 &&
            size < static_cast<std::uint32_t>(
                static_cast<std::int32_t>(
                    CSF_AT(std::uint32_t, records, 0x0C) - begin) >> 3))
        {
            const std::uint32_t end = CSF_AT(std::uint32_t, records, 0x08);
            GfxObject_ApplyToRecordRange(
                static_cast<int>(end), 1,
                reinterpret_cast<const void**>(contextBytes),
                reinterpret_cast<int>(records), remaining);
            CSF_AT(std::uint32_t, records, 0x08) = end + 8;
            source = CSF_AT(C_008630e0*, contextBytes, 0x08);
        }
        else
        {
            SkinListInsertRange(
                records, nullptr, CSF_AT(void**, records, 0x08), 1,
                reinterpret_cast<void**>(contextBytes));
        }

        recordOffset += 12;
        --remaining;
    }
    while (remaining != 0);

    return remaining;
}

HOOK(0x00700EF0, C_008630e0_AppendTeamPlayerRecords, "auto");
