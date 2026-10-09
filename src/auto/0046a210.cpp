#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Callee: C_0082d410::AssignStringSubstring (__thiscall, ret 12)
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Copies one 68-byte mission-script record (CLASSID/ACCION/CONDICION/EVENTO entry)
// from `source` into every 68-byte slot in [rangeBegin, rangeEnd).
// Returns rangeEnd. __cdecl, no stack cleanup.
static int __cdecl MissionScript_CopyRecordRange(int rangeBegin, int rangeEnd, int source)
{
    int result = rangeEnd;
    int cursor = rangeBegin;

    while (cursor != rangeEnd)
    {
        std::uint8_t* dst = reinterpret_cast<std::uint8_t*>(cursor);
        std::uint8_t* src = reinterpret_cast<std::uint8_t*>(source);

        CSF_AT(std::uint32_t, dst, 0x00) = CSF_AT(std::uint32_t, src, 0x00);
        CSF_AT(std::uint32_t, dst, 0x04) = CSF_AT(std::uint32_t, src, 0x04);
        CSF_AT(std::uint8_t,  dst, 0x08) = CSF_AT(std::uint8_t,  src, 0x08);
        CSF_AT(std::uint16_t, dst, 0x09) = CSF_AT(std::uint16_t, src, 0x09);
        CSF_AT(std::uint32_t, dst, 0x0C) = CSF_AT(std::uint32_t, src, 0x0C);
        CSF_AT(std::uint32_t, dst, 0x10) = CSF_AT(std::uint32_t, src, 0x10);
        CSF_AT(std::uint8_t,  dst, 0x14) = CSF_AT(std::uint8_t,  src, 0x14);

        // Name string at +0x18: AssignStringSubstring(this=dst+0x18, source=src+0x18, pos 0, count -1)
        AssignStringSubstring(reinterpret_cast<void*>(dst + 0x18),
                              nullptr,
                              reinterpret_cast<const void*>(src + 0x18),
                              0u,
                              0xFFFFFFFFu);

        CSF_AT(std::uint32_t, dst, 0x34) = CSF_AT(std::uint32_t, src, 0x34);
        CSF_AT(std::uint32_t, dst, 0x38) = CSF_AT(std::uint32_t, src, 0x38);
        CSF_AT(std::uint32_t, dst, 0x3C) = CSF_AT(std::uint32_t, src, 0x3C);
        CSF_AT(std::uint32_t, dst, 0x40) = CSF_AT(std::uint32_t, src, 0x40);

        result = rangeEnd;
        cursor += 68;
    }

    return result;
}

HOOK(0x46A210, MissionScript_CopyRecordRange, "auto");
