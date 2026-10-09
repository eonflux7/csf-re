#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Element layout: 68-byte script records (ACCIONES/CONDICIONES/EVENTOS style entries).
static constexpr int kScriptRecordSize = 68;

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Copies records backward from sourceEnd down to rangeBegin into the block ending at destEnd.
// Returns the new destination start (or destEnd unchanged when the range is empty).
static int __cdecl MissionScript_CopyRecordsBackward(int rangeBegin, int sourceEnd, int destEnd, int /*unused*/)
{
    if (rangeBegin == sourceEnd)
        return destEnd;

    std::uint8_t* src = reinterpret_cast<std::uint8_t*>(sourceEnd);
    std::uint8_t* dst = reinterpret_cast<std::uint8_t*>(destEnd);

    do
    {
        src -= kScriptRecordSize;
        dst -= kScriptRecordSize;

        CSF_AT(std::uint32_t, dst, 0x00) = CSF_AT(std::uint32_t, src, 0x00);
        CSF_AT(std::uint32_t, dst, 0x04) = CSF_AT(std::uint32_t, src, 0x04);
        CSF_AT(std::uint8_t, dst, 0x08) = CSF_AT(std::uint8_t, src, 0x08);
        CSF_AT(std::uint16_t, dst, 0x09) = CSF_AT(std::uint16_t, src, 0x09);
        CSF_AT(std::uint32_t, dst, 0x0C) = CSF_AT(std::uint32_t, src, 0x0C);
        CSF_AT(std::uint32_t, dst, 0x10) = CSF_AT(std::uint32_t, src, 0x10);
        CSF_AT(std::uint8_t, dst, 0x14) = CSF_AT(std::uint8_t, src, 0x14);

        // String member at +0x18: AssignStringSubstring(dst+0x18, src+0x18, 0, -1)
        AssignStringSubstring(dst + 0x18, nullptr, src + 0x18, 0u, 0xFFFFFFFFu);

        CSF_AT(std::uint32_t, dst, 0x34) = CSF_AT(std::uint32_t, src, 0x34);
        CSF_AT(std::uint32_t, dst, 0x38) = CSF_AT(std::uint32_t, src, 0x38);
        CSF_AT(std::uint32_t, dst, 0x3C) = CSF_AT(std::uint32_t, src, 0x3C);
        CSF_AT(std::uint32_t, dst, 0x40) = CSF_AT(std::uint32_t, src, 0x40);
    } while (src != reinterpret_cast<std::uint8_t*>(rangeBegin));

    return reinterpret_cast<int>(dst);
}

HOOK(0x0046A050, MissionScript_CopyRecordsBackward, "auto");
