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

// AssignStringRange: __thiscall, ret 12
CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830,
         (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));

// Copies the embedded sub-record at +0x34 of a record: __thiscall, ret 4
CSF_FUNC(int, __fastcall, CopySubRecord_514B10, 0x00514B10,
         (void* self, void* edx, int a4a));

// Copies a range [first, last) of 80-byte records into dest, returns end of the written destination
// (or dest itself when the range is empty).
static S_004f21d0* __cdecl CopyRecordRange_515160(uint8_t* first, uint8_t* last, S_004f21d0* dest)
{
    if (first == last)
        return dest;

    uint8_t* out = reinterpret_cast<uint8_t*>(dest);
    uint8_t* in = first;

    do
    {
        AssignStringRange_488830(out, nullptr, in, 0, 0xFFFFFFFF);

        CSF_AT(std::uint32_t, out, 0x1C) = CSF_AT(std::uint32_t, in, 0x1C);
        CSF_AT(std::uint32_t, out, 0x20) = CSF_AT(std::uint32_t, in, 0x20);
        CSF_AT(std::uint32_t, out, 0x24) = CSF_AT(std::uint32_t, in, 0x24);
        CSF_AT(std::uint32_t, out, 0x28) = CSF_AT(std::uint32_t, in, 0x28);
        CSF_AT(std::uint32_t, out, 0x2C) = CSF_AT(std::uint32_t, in, 0x2C);
        CSF_AT(std::uint32_t, out, 0x30) = CSF_AT(std::uint32_t, in, 0x30);

        CopySubRecord_514B10(out + 0x34, nullptr,
                             static_cast<int>(reinterpret_cast<std::uintptr_t>(in + 0x34)));

        CSF_AT(std::uint8_t, out, 0x44) = CSF_AT(std::uint8_t, in, 0x44);
        CSF_AT(std::uint32_t, out, 0x48) = CSF_AT(std::uint32_t, in, 0x48);
        CSF_AT(std::uint8_t, out, 0x4C) = CSF_AT(std::uint8_t, in, 0x4C);
        CSF_AT(std::uint8_t, out, 0x4D) = CSF_AT(std::uint8_t, in, 0x4D);

        in += 0x50;
        out += 0x50;
    } while (in != last);

    return reinterpret_cast<S_004f21d0*>(out);
}

HOOK(0x00515160, CopyRecordRange_515160, "auto");
