#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, HudCheckFlagAgainstGlobal, 0x006B2800, (uint32_t* self, void* , uint32_t* other));
CSF_FUNC(int, __fastcall, RenderEngine_DelegateResolve, 0x006DB750, (void* self, void* edx, uint32_t* a2, uint32_t* a3));

static int __fastcall C_00861460_ResolveOrDelegate(C_00861460* self, void* /*edx*/, uint32_t* a2, uint32_t* a3)
{
    if (HudCheckFlagAgainstGlobal(a2, nullptr, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BB728))))
    {
        *a3 = reinterpret_cast<uint32_t>(self);
        return self != nullptr;
    }

    RenderEngine_DelegateResolve(self, nullptr, a2, a3);
    return *a3 != 0;
}

HOOK(0x006DC180, C_00861460_ResolveOrDelegate, "auto");
