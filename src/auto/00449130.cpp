#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(uint32_t*, __fastcall, CopyRecordTail, 0x00448B60, (void* self, void* edx, int a2));

static uint32_t* __cdecl CopyRecordsBackward(
    uint8_t* begin, uint8_t* end, uint32_t* destinationEnd, int /*unused*/)
{
    if (begin == end)
        return destinationEnd;

    uint8_t* source = end;
    uint8_t* destination = reinterpret_cast<uint8_t*>(destinationEnd);
    do
    {
        const uint32_t firstDword = CSF_AT(uint32_t, source - 20, 0);
        source -= 20;
        destination -= 20;
        CSF_AT(uint32_t, destination, 0) = firstDword;
        CopyRecordTail(destination + 4, nullptr,
            static_cast<int>(reinterpret_cast<uintptr_t>(source + 4)));
    } while (source != begin);

    return reinterpret_cast<uint32_t*>(destination);
}

HOOK(0x00449130, CopyRecordsBackward, "auto");
