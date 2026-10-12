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

// Callee: resolves a record from a handle stored at this+0x64; returns a pointer as int.
CSF_FUNC(int, __fastcall, ResolveRecordFromHandle, 0x00425B40, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_Const20, 0x0082E0F0);

// Recomputes the scale factor at +0x58 from the value at +0x50 and the record's +0x10 value,
// and stores the constant 20.0f at +0x5C. Returns the record pointer from the callee.
static int __fastcall RefreshScaleFactorFromSource(void* self, void* /*edx*/)
{
    // The callee receives this+0x24 as `this` and this+0x64 as its argument.
    int record = ResolveRecordFromHandle(
        static_cast<uint8_t*>(self) + 0x24, nullptr,
        CSF_AT(int, self, 0x64));

    float input = CSF_AT(float, self, 0x50);
    float product = (g_Const20 - input) * (input + g_Const20);

    void* recordObj = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(record)));
    float recordValue = CSF_AT(float, recordObj, 0x10);

    CSF_AT(float, self, 0x5C) = 20.0f;
    CSF_AT(float, self, 0x58) = product / (recordValue + recordValue);

    return record;
}

HOOK(0x004240A0, RefreshScaleFactorFromSource, "auto");
