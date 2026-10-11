#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(char, __fastcall, TargetInRangeCheck_SubCall, 0x004E7E30, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_FarDistanceSqThreshold, 0x0083EC28);

static char __fastcall C_0083eb70_IsTargetInRange(C_0083eb70* self, void* /*edx*/, uint8_t* target)
{
    uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_0008);
    uint32_t mode = CSF_AT(uint32_t, owner, 0x1d8);
    float nearRange = CSF_AT(float, self, 0x34);
    float farRange = CSF_AT(float, self, 0x38);
    char result = 0;

    if (mode == 2 || mode == 3)
        farRange = farRange + farRange;

    if (CSF_AT(uint32_t, owner, 0x50) == 0) {
        float distSq = CSF_AT(float, target, 0x2c);
        return distSq < g_FarDistanceSqThreshold;
    }

    if (mode == 3) {
        return farRange * farRange > CSF_AT(float, target, 0x2c);
    }

    if (!(nearRange * nearRange > CSF_AT(float, target, 0x2c))) {
        if (CSF_AT(int, target, 4) != 3)
            return result;
        if (CSF_AT(int, target, 8) != 14)
            return result;

        int obj = CHandleTable_Resolve(g_pCurrentScene, nullptr, reinterpret_cast<int>(target + 0x14));
        if (!obj)
            return result;
        uint8_t* o = reinterpret_cast<uint8_t*>(obj);
        if ((CSF_AT(uint32_t, o, 0x170) & 0x2000) == 0)
            return result;
        if (CSF_AT(uint32_t, o, 0x50) != 0)
            return result;

        bool isMode2 = CSF_AT(uint32_t, owner, 0x1d8) == 2;
        bool flagOk = (CSF_AT(uint32_t, o, 0x1c4) == 0) && ((CSF_AT(uint32_t, o, 0x2e0) & 0x10000) == 0);

        if (!isMode2 && !flagOk)
            return result;
        if (!(farRange * farRange > CSF_AT(float, target, 0x2c)))
            return result;

        if (!TargetInRangeCheck_SubCall(self, nullptr, reinterpret_cast<int>(target + 0x14))) {
            CSF_AT(int, target, 4) = 2;
            CSF_AT(int, target, 8) = 7;
        }
    }
    return 1;
}

HOOK(0x004E7F10, C_0083eb70_IsTargetInRange, "auto");
