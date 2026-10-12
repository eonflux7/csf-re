#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Hud_CheckFlagAgainstGlobal, 0x006B2800, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, CCamera_ResolveProjectionDelegate, 0x006D0490, (void* self, void* edx, uint32_t* a2, int* a3));

static int __fastcall CCamera_ResolveProjectionOrDelegate(CCamera* self, void* /*edx*/, uint32_t* a2, int* a3)
{
    if (Hud_CheckFlagAgainstGlobal(a2, nullptr, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BB4FC))))
    {
        *a3 = (int)self;
        return self != nullptr;
    }

    CCamera_ResolveProjectionDelegate(self, nullptr, a2, a3);
    return *a3 != 0;
}

HOOK(0x006D2960, CCamera_ResolveProjectionOrDelegate, "auto");
