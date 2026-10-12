#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callee at 0x0040ACC0: __thiscall-style, takes the destination sub-object (ecx) and a source pointer (pushed).
CSF_FUNC(int, __fastcall, CopyRecordPayload, 0x0040ACC0, (void* self, void* edx, int a2));

// Copies 28-byte records from [begin, end) into the destination ending at 'dest',
// walking backwards. Each record: dword 0, dword 1, dword 2, then a 16-byte payload
// handled by CopyRecordPayload. Returns the new destination cursor, or 'dest' if the
// range is empty.
static uint32_t* __cdecl ReverseCopyRecords28(int rangeBegin, int rangeEnd, uint32_t* dest, int /*unused*/)
{
    uint8_t* src = reinterpret_cast<uint8_t*>(rangeEnd);
    if (rangeBegin == rangeEnd)
        return dest;

    uint8_t* out = reinterpret_cast<uint8_t*>(dest);
    do
    {
        src -= 28;
        out -= 28;

        uint32_t* srcWords = reinterpret_cast<uint32_t*>(src);
        uint32_t* outWords = reinterpret_cast<uint32_t*>(out);

        outWords[0] = srcWords[0];
        outWords[1] = srcWords[1];
        outWords[2] = srcWords[2];

        CopyRecordPayload(out + 12, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(src + 12)));
    } while (reinterpret_cast<int>(src) != rangeBegin);

    return reinterpret_cast<uint32_t*>(out);
}

HOOK(0x0040AE60, ReverseCopyRecords28, "auto");
