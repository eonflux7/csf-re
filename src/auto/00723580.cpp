#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, AssignStringRange, 0x00488830,
         (void* self, void* edx, void* source,
          std::uint32_t position, std::uint32_t count));

static std::uint32_t* __cdecl CopyStringRecordsForward(
    std::uint32_t* first,
    std::uint32_t* last,
    std::uint32_t* destination)
{
    auto* sourceRecord = reinterpret_cast<std::uint8_t*>(first);
    auto* destinationRecord = reinterpret_cast<std::uint8_t*>(destination);
    auto* sourceEnd = reinterpret_cast<std::uint8_t*>(last);

    if (sourceRecord != sourceEnd)
    {
        do
        {
            CSF_AT(std::uint32_t, destinationRecord, 0x00) =
                CSF_AT(std::uint32_t, sourceRecord, 0x00);
            CSF_AT(std::uint32_t, destinationRecord, 0x04) =
                CSF_AT(std::uint32_t, sourceRecord, 0x04);
            CSF_AT(std::uint32_t, destinationRecord, 0x08) =
                CSF_AT(std::uint32_t, sourceRecord, 0x08);
            CSF_AT(std::uint32_t, destinationRecord, 0x0C) =
                CSF_AT(std::uint32_t, sourceRecord, 0x0C);
            CSF_AT(std::uint32_t, destinationRecord, 0x10) =
                CSF_AT(std::uint32_t, sourceRecord, 0x10);
            CSF_AT(std::uint32_t, destinationRecord, 0x14) =
                CSF_AT(std::uint32_t, sourceRecord, 0x14);

            AssignStringRange(destinationRecord + 0x18, nullptr,
                              sourceRecord + 0x18, 0, UINT32_MAX);

            CSF_AT(std::uint32_t, destinationRecord, 0x34) =
                CSF_AT(std::uint32_t, sourceRecord, 0x34);
            CSF_AT(std::uint32_t, destinationRecord, 0x38) =
                CSF_AT(std::uint32_t, sourceRecord, 0x38);

            sourceRecord += 0x3C;
            destinationRecord += 0x3C;
        }
        while (sourceRecord != sourceEnd);
    }

    return reinterpret_cast<std::uint32_t*>(destinationRecord);
}

HOOK(0x00723580, CopyStringRecordsForward, "auto");
