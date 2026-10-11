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

CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static double __fastcall C_0083eb70_GetModeScaledBaseValue(C_0083eb70* self, void* /*edx*/) {
    float baseValue = self->field_003c;

    uint8_t* modeOwner = reinterpret_cast<uint8_t*>(self->field_0008);
    int mode = CSF_AT(int, modeOwner, 0x1D8);

    if (mode == 0 || mode == 1) {
        uint8_t* globalBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetGlobalValue_8BC1E0()));
        float scale = CSF_AT(float, globalBase, 0xAC4);
        return baseValue * scale;
    }

    return baseValue;
}

HOOK(0x004E6D80, C_0083eb70_GetModeScaledBaseValue, "auto");
