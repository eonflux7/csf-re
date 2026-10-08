#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, ResetWideStringsInRecordRange, 0x007233D0,
         (int begin, int end));
CSF_FUNC(std::uint32_t*, __cdecl, CopyStringRecordsForward, 0x00723580,
         (std::uint32_t* first, std::uint32_t* last,
          std::uint32_t* destination));

static std::uint32_t* __fastcall EraseWideStringRecords(
    int* self,
    void* /*edx*/,
    std::uint32_t* resultIterator,
    std::uint32_t* first,
    std::uint32_t* last)
{
    if (first != last)
    {
        std::uint32_t* newEnd = CopyStringRecordsForward(
            last,
            reinterpret_cast<std::uint32_t*>(
                static_cast<std::uintptr_t>(CSF_AT(std::uint32_t, self, 0x08))),
            first);

        ResetWideStringsInRecordRange(
            static_cast<int>(reinterpret_cast<std::intptr_t>(newEnd)),
            CSF_AT(int, self, 0x08));

        CSF_AT(std::uint32_t, self, 0x08) =
            static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(newEnd));
    }

    *resultIterator =
        static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(first));
    return resultIterator;
}

HOOK(0x007238E0, EraseWideStringRecords, "auto");
