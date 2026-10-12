#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(double, __fastcall, C_0086c288_GetFacingAngleRadians, 0x0075D0A0, (C_0086c288* self, void* , uint32_t* a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static double __fastcall ComputeLinkedSubObjectRatio(void* self, void* /*edx*/)
{
    if (CSF_AT(int, self, 0) == -1 || CSF_AT(int, self, 4) == -1)
        return static_cast<double>(g_fZero);

    int managerValue = GetValue8B0554();
    uint8_t* managerBase = reinterpret_cast<uint8_t*>(managerValue);
    C_0086c288* subObject = reinterpret_cast<C_0086c288*>(CSF_AT(uint32_t, managerBase, 0x1FD8));
    return C_0086c288_GetFacingAngleRadians(subObject, nullptr, reinterpret_cast<uint32_t*>(self));
}

HOOK(0x004A9CD0, ComputeLinkedSubObjectRatio, "auto");
