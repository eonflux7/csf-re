#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Hud_CheckFlagAgainstGlobal, 0x006B2800, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(bool, __fastcall, Skin_DelegateResolve, 0x006B2840, (int a1, int a2, uint32_t* a3, int* a4));

static int __fastcall C_00860378_ResolveOrDelegate(
    C_00860378* self, void* /*edx*/, uint32_t* query, int* result)
{
    if (static_cast<uint8_t>(Hud_CheckFlagAgainstGlobal(
            query, nullptr,
            reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BBAD4)))))
    {
        *result = reinterpret_cast<int>(self);
        return self != nullptr;
    }

    Skin_DelegateResolve(reinterpret_cast<int>(self), 0, query, result);
    return *result != 0;
}

HOOK(0x006EB820, C_00860378_ResolveOrDelegate, "auto");
