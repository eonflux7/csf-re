#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Copies a range of 0x3C-byte records from [srcBegin, srcEnd) to dstBegin.
// Each record: dword at +0x00, dwords at +0x04..+0x0C, dwords at +0x10..+0x1C,
// and a string object at +0x20 that is assigned as a substring (pos 0, count -1).
static void* __cdecl CopyRecordRangeWithNames(void* srcBegin, void* srcEnd, void* dstBegin, int /*unused*/)
{
    std::uint8_t* src = static_cast<std::uint8_t*>(srcBegin);
    std::uint8_t* dst = static_cast<std::uint8_t*>(dstBegin);

    if (src != static_cast<std::uint8_t*>(srcEnd))
    {
        do
        {
            CSF_AT(std::uint32_t, dst, 0x00) = CSF_AT(std::uint32_t, src, 0x00);
            CSF_AT(std::uint32_t, dst, 0x04) = CSF_AT(std::uint32_t, src, 0x04);
            CSF_AT(std::uint32_t, dst, 0x08) = CSF_AT(std::uint32_t, src, 0x08);
            CSF_AT(std::uint32_t, dst, 0x0C) = CSF_AT(std::uint32_t, src, 0x0C);

            CSF_AT(std::uint32_t, dst, 0x10) = CSF_AT(std::uint32_t, src, 0x10);
            CSF_AT(std::uint32_t, dst, 0x14) = CSF_AT(std::uint32_t, src, 0x14);
            CSF_AT(std::uint32_t, dst, 0x18) = CSF_AT(std::uint32_t, src, 0x18);
            CSF_AT(std::uint32_t, dst, 0x1C) = CSF_AT(std::uint32_t, src, 0x1C);

            AssignStringSubstring(dst + 0x20, nullptr, src + 0x20, 0u, 0xFFFFFFFFu);

            src += 0x3C;
            dst += 0x3C;
        } while (src != static_cast<std::uint8_t*>(srcEnd));
    }

    return dst;
}

HOOK(0x00775600, CopyRecordRangeWithNames, "auto");
