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

// Virtual slot 0x30 / 4 = 12: query returning a status code (no extra args, thiscall).
using StatusQueryFn = int(__fastcall*)(void* self, void* edx);
// Virtual slot 0x8C / 4 = 35: four stack args after this (thiscall, callee pops 16 bytes).
using StatusActionFn = int(__fastcall*)(void* self, void* edx, int a, int b, int c, int d);

static constexpr int kStatusCodeReset = 0x35; // 53

static int __fastcall C_0082e7c8_CheckStatusAndRunReset(C_0082e7c8* self, void* /*edx*/)
{
    // Query the status of the object referenced by field_0014.
    void* target = reinterpret_cast<void*>(self->field_0014);
    int result = reinterpret_cast<StatusQueryFn>(CSF_AT(void**, target, 0)[48 / 4])(target, nullptr);

    if (result == kStatusCodeReset) {
        // Re-read field_0014 and its vtable for the second call, as the original does.
        void* target2 = reinterpret_cast<void*>(self->field_0014);
        return reinterpret_cast<StatusActionFn>(CSF_AT(void**, target2, 0)[140 / 4])(
            target2, nullptr, 0, 0, -1, 0);
    }
    return result;
}

HOOK(0x004201F0, C_0082e7c8_CheckStatusAndRunReset, "auto");
