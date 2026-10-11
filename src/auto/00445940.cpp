#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, IsDistinctNonSentinelValue, 0x0048E0D0, (void* self, void* , std::int32_t candidate));
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(int, __fastcall, IsStateCompatible, 0x0048EF90, (void* self, void* , int requestedState));
CSF_FUNC(int, __cdecl, IsSpecialTypeCode, 0x00755910, (int code));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __stdcall SceneObj_CheckStateEligibility(int requestedState, int handle)
{
    auto* object = reinterpret_cast<C_00833b88*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr, handle));
    if (!object)
        return 0;

    const bool hasStateFlag = (object->field_0170 & 0x2000) == 0x2000;

    if (requestedState == 1)
    {
        if (!hasStateFlag)
        {
            if (static_cast<std::uint8_t>(IsSpecialTypeCode(object->field_0050)) != 0)
                return 1;
            return object->field_0050 == 20 ? 1 : 0;
        }

        const char distinct = static_cast<char>(
            IsDistinctNonSentinelValue(object, nullptr, 1));
        if (distinct != 0)
            return distinct;

        if (static_cast<std::uint8_t>(
                IsStateCompatible(object, nullptr, 1)) != 0)
        {
            const std::uint32_t state = object->field_01d8;
            if (state == 2 || state == 3)
                return 1;
        }

        return static_cast<std::uint8_t>(
            SceneObj_IsInactive(object, nullptr)) != 0 ? 1 : 0;
    }

    if (requestedState == 0 && hasStateFlag)
        return static_cast<char>(
            IsDistinctNonSentinelValue(object, nullptr, 0));

    return 0;
}

HOOK(0x00445940, SceneObj_CheckStateEligibility, "auto");
