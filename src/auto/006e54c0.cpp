#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Hud_CheckFlagAgainstGlobal, 0x006B2800, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, C_00861c08_ResolveOrDelegate, 0x00706070, (C_00861c08* self, void* /*edx*/, std::uint32_t* a2, int* a3));

static int __fastcall C_00861ce0_ResolveOrDelegate(C_00861ce0* self, void* /*edx*/, uint32_t* a2, int* a3)
{
    if (Hud_CheckFlagAgainstGlobal(a2, nullptr, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BBAB4))))
    {
        *a3 = (int)self;
        return self != nullptr;
    }

    C_00861c08_ResolveOrDelegate(reinterpret_cast<C_00861c08*>(self), nullptr, a2, a3);
    return *a3 != 0;
}

HOOK(0x006E54C0, C_00861ce0_ResolveOrDelegate, "auto");
