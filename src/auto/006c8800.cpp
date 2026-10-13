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

// Vtable slots (byte offset / 4): +0x3C -> 15, +0x34 -> 13, +0x2C -> 11.
// Object types are not declared, so the calls go through the raw vtable.

static int __fastcall S_006c8800_ProcessWeaponContext(S_006c8800* self, void* /*edx*/)
{
    int result = 0;

    // Stage 1: field +0x08, slot 15 (+0x3C), argument 0.
    uint32_t firstObj = self->field_0008;
    self->field_0024 = 0.0f;
    if (firstObj) {
        void* obj = reinterpret_cast<void*>(firstObj);
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(CSF_AT(void**, obj, 0)[15])(obj, nullptr, 0);
    }

    // Stage 2: field +0x04, slot 13 (+0x34), argument 0.
    uint32_t secondObj = self->field_0004;
    if (secondObj) {
        void* obj = reinterpret_cast<void*>(secondObj);
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(CSF_AT(void**, obj, 0)[13])(obj, nullptr, 0);
    }

    // Stage 3: field +0x0C, slot 11 (+0x2C), argument 0. Its result is returned directly.
    uint32_t thirdObj = self->field_000c;
    if (thirdObj) {
        void* obj = reinterpret_cast<void*>(thirdObj);
        return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(CSF_AT(void**, obj, 0)[11])(obj, nullptr, 0);
    }

    return result;
}

HOOK(0x006C8800, S_006c8800_ProcessWeaponContext, "auto");
