#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Hud_CheckFlagAgainstGlobal, 0x006B2800, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, C_00861f94_DelegateResolve, 0x006B6A80, (void* self, void* edx, uint32_t* a2, uint32_t* a3));

static int __fastcall C_00861f94_ResolveOrDelegate(
    C_00861f94* self, void* /*edx*/, uint32_t* query, uint32_t* result)
{
    if (static_cast<uint8_t>(Hud_CheckFlagAgainstGlobal(
            query, nullptr,
            reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BBAD8)))))
    {
        *result = reinterpret_cast<uint32_t>(self);
        return self != nullptr;
    }

    C_00861f94_DelegateResolve(self, nullptr, query, result);
    return *result != 0;
}

HOOK(0x006EB8A0, C_00861f94_ResolveOrDelegate, "auto");
