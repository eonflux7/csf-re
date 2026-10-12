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

CSF_FUNC(int, __cdecl, sub_7FEDC0, 0x007FEDC0, (uint32_t* a1, int a2, int a3));

// Fills a 6-dword block (position/heading data) from the object reached through
// field_0004 and stores it into the caller's output buffer.
static int __fastcall C_008630e0_FetchTransformInto(C_008630e0* self, void* /*edx*/, int a2)
{
    uint32_t v8[8];

    uint8_t* sourceBase = reinterpret_cast<uint8_t*>(self->field_0004);
    uint32_t sourceInner = CSF_AT(uint32_t, sourceBase, 0x24);

    sub_7FEDC0(reinterpret_cast<uint32_t*>(sourceInner),
               static_cast<int>(self->field_0004),
               static_cast<int>(reinterpret_cast<uintptr_t>(v8)));

    uint8_t* out = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a2));
    CSF_AT(uint32_t, out, 0x0C) = v8[4];
    CSF_AT(uint32_t, out, 0x10) = v8[5];
    CSF_AT(uint32_t, out, 0x14) = v8[6];
    CSF_AT(uint32_t, out, 0x00) = v8[0];
    CSF_AT(uint32_t, out, 0x04) = v8[1];
    CSF_AT(uint32_t, out, 0x08) = v8[2];

    return a2;
}

HOOK(0x006FF9A0, C_008630e0_FetchTransformInto, "auto");
