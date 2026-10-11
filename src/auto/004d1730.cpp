// Reimplementation of AiCore_UpdateTargetData (0x004D1730).
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

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(void*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall AiCore_UpdateTargetData(S_0048c9f0* self, void* /*edx*/, int a2)
{
    uint8_t* handle = reinterpret_cast<uint8_t*>(a2);

    if (CSF_AT(int, handle, 8) < 0)
    {
        CSF_AT(uint8_t, self, 0) = 0;
        CSF_AT(int, self, 0x14) = -1;
        CSF_AT(int, self, 0x18) = -1;
        return -1;
    }

    int resolved = CHandleTable_Resolve(g_pCurrentScene, nullptr, a2);
    if (resolved == 0)
        return resolved;

    CSF_AT(uint8_t, self, 0) = 1;
    CSF_AT(int, self, 0x14) = CSF_AT(int, handle, 4);
    CSF_AT(int, self, 0x18) = CSF_AT(int, handle, 8);

    uint8_t* positionVector = reinterpret_cast<uint8_t*>(
        GetObjectPositionVector(reinterpret_cast<void*>(resolved), nullptr));
    CSF_AT(int, self, 4) = CSF_AT(int, positionVector, 0);
    CSF_AT(int, self, 8) = CSF_AT(int, positionVector, 4);
    CSF_AT(int, self, 0xC) = CSF_AT(int, positionVector, 8);
    return CSF_AT(int, positionVector, 8);
}

HOOK(0x004D1730, AiCore_UpdateTargetData, "auto");
