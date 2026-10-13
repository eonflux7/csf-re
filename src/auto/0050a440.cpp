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

CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

// Applies two option values to the options subsystem, then stores the
// field_0007 byte into the global options block at offset 0x21DC.
static int __fastcall ApplyOptionsFileValues(S_0050a440* self, void* /*edx*/)
{
    // First subsystem: call slot 4 (+0x10) with field_0008.
    uint8_t* managerA = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetGlobalValue_8B056C()));
    uint32_t subsystemA = CSF_AT(uint32_t, managerA, 0x1C);
    if (subsystemA) {
        void* subsystemObjA = reinterpret_cast<void*>(static_cast<uintptr_t>(subsystemA));
        ((void (__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, subsystemObjA, 0)[4])(
            subsystemObjA, nullptr, self->field_0008);
    }

    // Second subsystem: re-fetched; call slot 6 (+0x18) with field_000c.
    uint8_t* managerB = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetGlobalValue_8B056C()));
    uint32_t subsystemB = CSF_AT(uint32_t, managerB, 0x1C);
    if (subsystemB) {
        void* subsystemObjB = reinterpret_cast<void*>(static_cast<uintptr_t>(subsystemB));
        ((void (__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, subsystemObjB, 0)[6])(
            subsystemObjB, nullptr, self->field_000c);
    }

    // Store the flag byte into the global options block.
    int result = GetGlobalValue_8BC1E0();
    uint8_t* optionsBlock = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(result));
    CSF_AT(uint8_t, optionsBlock, 0x21DC) = self->field_0007;
    return result;
}

HOOK(0x0050A440, ApplyOptionsFileValues, "auto");
