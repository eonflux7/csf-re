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

// Callees of the original (bound by address, signatures per packet).
CSF_FUNC(uint16_t*, __cdecl, sub_6A45A0, 0x006A45A0, (int a1));
CSF_FUNC(int, __fastcall, sub_6A08B0, 0x006A08B0, (int a1, int a2));

// Resets an inflate stream (z_stream at `strm`) and its internal state block.
// The z_stream layout: +0x08 total_in, +0x14 total_out, +0x18 msg, +0x1C state,
// +0x20 zalloc, +0x24 zfree, +0x2C data_type, +0x30 adler.
static int __cdecl InflateResetStream(uint8_t* strm)
{
    if (!strm)
        return -2;

    uint8_t* state = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, strm, 0x1C)));
    if (!state || !CSF_AT(int, strm, 0x20) || !CSF_AT(int, strm, 0x24))
        return -2;

    CSF_AT(int, strm, 0x14) = 0;
    CSF_AT(int, strm, 0x08) = 0;
    CSF_AT(int, strm, 0x18) = 0;
    CSF_AT(int, strm, 0x2C) = 2;

    CSF_AT(int, state, 0x10) = CSF_AT(int, state, 0x08);

    const bool hasNegativeTotal = CSF_AT(int, state, 0x18) < 0;
    CSF_AT(int, state, 0x14) = 0;
    if (hasNegativeTotal)
        CSF_AT(int, state, 0x18) = 0;

    CSF_AT(int, state, 0x04) = CSF_AT(int, state, 0x18) != 0 ? 113 : 42;

    CSF_AT(int, strm, 0x30) = 1;

    CSF_AT(int, state, 0x20) = 0;
    sub_6A45A0(static_cast<int>(reinterpret_cast<uintptr_t>(state)));

    // The original passes an uninitialized ecx (garbage); pass 0 instead.
    sub_6A08B0(0, static_cast<int>(reinterpret_cast<uintptr_t>(state)));

    return 0;
}

HOOK(0x006A15C0, InflateResetStream, "auto");
