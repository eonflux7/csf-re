#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, HudCheckFlagAgainstGlobal, 0x006B2800, (uint32_t* self, void* , uint32_t* other));
CSF_FUNC(int, __fastcall, C_00861260_ResolveOrDelegate, 0x00701E70, (C_00861260* self, void* , uint32_t* a2, int* a3));

static int __fastcall CRenderEngine_ResolveOrDelegate(CRenderEngine* self, void* /*edx*/, uint32_t* a2, int* a3)
{
    if (HudCheckFlagAgainstGlobal(a2, nullptr, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BB724))))
    {
        *a3 = (int)self;
        return self != nullptr;
    }

    C_00861260_ResolveOrDelegate(reinterpret_cast<C_00861260*>(self), nullptr, a2, a3);
    return *a3 != 0;
}

HOOK(0x006DB750, CRenderEngine_ResolveOrDelegate, "auto");
