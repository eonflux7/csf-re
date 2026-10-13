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

CSF_FUNC(int, __fastcall, RefreshOwnerState, 0x004D4BB0, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_MaxPercent, 0x0082E120);

static int __fastcall C_00834fa8_SelectLookupByProximity(C_00834fa8* self, void* /*edx*/)
{
    // Refresh the owner object first; the original reloads [this+8] afterwards.
    RefreshOwnerState(CSF_AT(void*, self, 0x08), nullptr);

    void* owner = CSF_AT(void*, self, 0x08);
    float* position = GetObjectPositionVector(CSF_AT(void*, owner, 0x14), nullptr);

    float dx = position[0] - CSF_AT(float, owner, 0x3d8);
    float dy = position[1] - CSF_AT(float, owner, 0x3dc);
    float dz = position[2] - CSF_AT(float, owner, 0x3e0);
    float squaredDistance = dx * dx + dy * dy + dz * dz;

    if (squaredDistance <= g_MaxPercent)
        return CharTask_LookupByOffset(self, nullptr, CSF_AT(int, self, 0x0c) + 100);
    else
        return CharTask_LookupByOffset(self, nullptr, CSF_AT(int, self, 0x0c) + 80);
}

HOOK(0x004D5040, C_00834fa8_SelectLookupByProximity, "auto");
