#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, HudCheckFlagAgainstGlobal, 0x006B2800, (uint32_t* self, void* edx, uint32_t* other));
CSF_FUNC(int, __fastcall, Light_DelegateResolve, 0x007096D0, (void* self, void* edx, uint32_t* a2, int* a3));

static int __fastcall CLightObject_ResolveOrDelegate(CLightObject* self, void* /*edx*/, uint32_t* a2, int* a3)
{
    if (HudCheckFlagAgainstGlobal(a2, nullptr, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BBB38))))
    {
        *a3 = (int)self;
        return self != nullptr;
    }

    Light_DelegateResolve(self, nullptr, a2, a3);
    return *a3 != 0;
}

HOOK(0x006FCCE0, CLightObject_ResolveOrDelegate, "auto");
