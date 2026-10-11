#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callee bindings (by address)
CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830,
         (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __fastcall, CopyProfileSubRecord_514B10, 0x00514B10,
         (void* self, void* edx, int a4a));

// Copies a block of 80-byte profile records from [sourceEnd - 80 .. stopAddress) downward
// into the destination block ending at destEnd. Each record is copied field by field
// (string via AssignStringRange, dwords at +0x1C..+0x30, a sub-record at +0x34,
// then bytes/dword at +0x44..+0x4D). Returns the destination record pointer reached.
static int __cdecl CopyProfileRecordsBackward(int stopAddress, int sourceEnd, int destEnd, int unusedArg4)
{
    if (stopAddress == sourceEnd)
        return destEnd;

    int srcCursor = sourceEnd;
    int dstCursor = destEnd;
    do
    {
        srcCursor -= 80;
        dstCursor -= 80;

        std::uint8_t* src = reinterpret_cast<std::uint8_t*>(srcCursor);
        std::uint8_t* dst = reinterpret_cast<std::uint8_t*>(dstCursor);

        AssignStringRange_488830(reinterpret_cast<void*>(dstCursor), nullptr,
                                 reinterpret_cast<void*>(srcCursor), 0, 0xFFFFFFFF);

        CSF_AT(std::uint32_t, dst, 0x1C) = CSF_AT(std::uint32_t, src, 0x1C);
        CSF_AT(std::uint32_t, dst, 0x20) = CSF_AT(std::uint32_t, src, 0x20);
        CSF_AT(std::uint32_t, dst, 0x24) = CSF_AT(std::uint32_t, src, 0x24);
        CSF_AT(std::uint32_t, dst, 0x28) = CSF_AT(std::uint32_t, src, 0x28);
        CSF_AT(std::uint32_t, dst, 0x2C) = CSF_AT(std::uint32_t, src, 0x2C);
        CSF_AT(std::uint32_t, dst, 0x30) = CSF_AT(std::uint32_t, src, 0x30);

        CopyProfileSubRecord_514B10(reinterpret_cast<void*>(dstCursor + 0x34), nullptr,
                                    srcCursor + 0x34);

        CSF_AT(std::uint8_t, dst, 0x44) = CSF_AT(std::uint8_t, src, 0x44);
        CSF_AT(std::uint32_t, dst, 0x48) = CSF_AT(std::uint32_t, src, 0x48);
        CSF_AT(std::uint8_t, dst, 0x4C) = CSF_AT(std::uint8_t, src, 0x4C);
        CSF_AT(std::uint8_t, dst, 0x4D) = CSF_AT(std::uint8_t, src, 0x4D);
    } while (srcCursor != stopAddress);

    return dstCursor;
}

HOOK(0x005168D0, CopyProfileRecordsBackward, "auto");
