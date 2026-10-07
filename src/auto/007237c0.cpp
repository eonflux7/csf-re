#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, AssignStringRange, 0x00488830,
         (void* self, void* edx, void* source,
          std::uint32_t position, std::uint32_t count));

static std::uint32_t* __cdecl FillStringRecords(
    std::uint32_t* first,
    std::uint32_t* last,
    std::uint32_t* value)
{
    auto* destinationRecord = reinterpret_cast<std::uint8_t*>(first);
    auto* destinationEnd = reinterpret_cast<std::uint8_t*>(last);

    if (destinationRecord != destinationEnd)
    {
        do
        {
            CSF_AT(std::uint32_t, destinationRecord, 0x00) =
                CSF_AT(std::uint32_t, value, 0x00);
            CSF_AT(std::uint32_t, destinationRecord, 0x04) =
                CSF_AT(std::uint32_t, value, 0x04);
            CSF_AT(std::uint32_t, destinationRecord, 0x08) =
                CSF_AT(std::uint32_t, value, 0x08);
            CSF_AT(std::uint32_t, destinationRecord, 0x0C) =
                CSF_AT(std::uint32_t, value, 0x0C);
            CSF_AT(std::uint32_t, destinationRecord, 0x10) =
                CSF_AT(std::uint32_t, value, 0x10);
            CSF_AT(std::uint32_t, destinationRecord, 0x14) =
                CSF_AT(std::uint32_t, value, 0x14);

            AssignStringRange(
                destinationRecord + 0x18, nullptr,
                reinterpret_cast<std::uint8_t*>(value) + 0x18,
                0, UINT32_MAX);

            CSF_AT(std::uint32_t, destinationRecord, 0x34) =
                CSF_AT(std::uint32_t, value, 0x34);
            CSF_AT(std::uint32_t, destinationRecord, 0x38) =
                CSF_AT(std::uint32_t, value, 0x38);

            destinationRecord += 0x3C;
        }
        while (destinationRecord != destinationEnd);
    }

    return last;
}

HOOK(0x007237C0, FillStringRecords, "auto");
