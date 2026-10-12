#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Hud_CheckFlagAgainstGlobal, 0x006B2800, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, sub_6D0CB0, 0x006D0CB0, (void* self, void* edx, uint32_t* a2, uint32_t* a3));

static int __fastcall C_008631c8_ResolveOrDelegate(C_008631c8* self, void* /*edx*/, uint32_t* a2, uint32_t* a3)
{
    if (Hud_CheckFlagAgainstGlobal(a2, nullptr, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BBED8))))
    {
        *a3 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(self));
        return self != nullptr;
    }

    sub_6D0CB0(self, nullptr, a2, a3);
    return *a3 != 0;
}

HOOK(0x00705F10, C_008631c8_ResolveOrDelegate, "auto");
