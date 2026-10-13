#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, HudCheckFlagAgainstGlobal, 0x006B2800, (uint32_t* self, void* , uint32_t* other));
CSF_FUNC(int, __fastcall, C_00861e08_ResolveDelegate, 0x00707480, (void* self, void* edx, uint32_t* a2, int* a3));

static int __fastcall C_00863430_ResolveFlagOrDelegate(C_00863430* self, void* /*edx*/, uint32_t* a2, int* a3)
{
    if (HudCheckFlagAgainstGlobal(a2, nullptr, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BBEF8))))
    {
        *a3 = (int)self;
        return self != nullptr;
    }

    C_00861e08_ResolveDelegate(self, nullptr, a2, a3);
    return *a3 != 0;
}

HOOK(0x00708130, C_00863430_ResolveFlagOrDelegate, "auto");
