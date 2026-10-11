#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Hud_CheckFlagAgainstGlobal, 0x006B2800, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, C_00860af0_DelegateResolve, 0x006F54C0, (void* self, void* edx, uint32_t* a2, int* a3));

static int __fastcall C_00860bb8_ResolveOrDelegate(C_00860bb8* self, void* /*edx*/, uint32_t* a2, int* a3)
{
    if (Hud_CheckFlagAgainstGlobal(a2, nullptr, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BB4DC))))
    {
        *a3 = (int)(uintptr_t)self;
        return self != nullptr;
    }

    C_00860af0_DelegateResolve(self, nullptr, a2, a3);
    return *a3 != 0;
}

HOOK(0x006CA8E0, C_00860bb8_ResolveOrDelegate, "auto");
