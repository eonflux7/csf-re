#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Hud_CheckFlagAgainstGlobal, 0x006B2800, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, C_00861a60_ResolveOrDelegate, 0x006B1FB0, (void* self, void* edx, void* a2, uint32_t* a3));

static int __fastcall C_00861a78_ResolveOrDelegate(C_00861a78* self, void* /*edx*/, uint32_t* a2, uint32_t* a3)
{
    if (Hud_CheckFlagAgainstGlobal(a2, nullptr, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BBAA0))))
    {
        *a3 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(self));
        return self != nullptr;
    }

    C_00861a60_ResolveOrDelegate(self, nullptr, a2, a3);
    return *a3 != 0;
}

HOOK(0x006E3D20, C_00861a78_ResolveOrDelegate, "auto");
