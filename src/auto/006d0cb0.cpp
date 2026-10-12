#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Hud_CheckFlagAgainstGlobal, 0x006B2800, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, C_00860de0_ResolveThroughBase, 0x006E3C80, (void* self, void* edx, uint32_t* a2, int* a3));

static int __fastcall C_00860e68_ResolveOrDelegate(C_00860e68* self, void* /*edx*/, uint32_t* a2, int* a3)
{
    if (Hud_CheckFlagAgainstGlobal(a2, nullptr, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BB4F8))))
    {
        *a3 = (int)self;
        return self != nullptr;
    }

    C_00860de0_ResolveThroughBase(self, nullptr, a2, a3);
    return *a3 != 0;
}

HOOK(0x006D0CB0, C_00860e68_ResolveOrDelegate, "auto");
