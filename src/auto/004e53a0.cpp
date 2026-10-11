#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, CharActionFsm_IsFlag10000SetAndByteNonZero, 0x00482310, (void* self, void*));
CSF_FUNC(int, __fastcall, IsDistinctNonSentinelValue, 0x0048E0D0, (void* self, void* , std::int32_t candidate));
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneObject_PassesFirstCheck, 0x0048E120, (void* self, void* edx));
CSF_FUNC(int, __fastcall, IsStateCompatible, 0x0048EF90, (void* self, void* , int requestedState));
CSF_FUNC(int, __fastcall, Hud_GetAlternateSceneSelection, 0x004950B0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, IsSpecialTypeCode, 0x00755910, (int code));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __fastcall C_0083e9d0_MatchesSceneObjectCondition(
    C_0083e9d0* self, void* /*edx*/, int handle, int condition)
{
    void* object = reinterpret_cast<void*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr, handle));
    if (!object)
        return 0;

    switch (condition)
    {
    case 1:
        return CSF_AT(std::int32_t, object, 0x50) == 20;

    case 3:
    {
        if (CSF_AT(std::int32_t, object, 0x50))
            return 0;
        if (static_cast<std::uint8_t>(
                CharActionFsm_IsFlag10000SetAndByteNonZero(object, nullptr)))
            return 0;

        const int selection = Hud_GetAlternateSceneSelection(object, nullptr);
        if (!selection)
            return 0;

        return selection <= CSF_AT(
            std::int32_t, reinterpret_cast<void*>(self->field_0008), 0x7C);
    }

    case 5:
        if ((CSF_AT(std::uint32_t, object, 0x170) & 0x2000) == 0)
            return 0;
        if (!static_cast<std::uint8_t>(IsStateCompatible(
                reinterpret_cast<void*>(self->field_0008),
                nullptr, CSF_AT(std::int32_t, object, 0x78))))
            return 0;
        return CSF_AT(std::int32_t, object, 0x1D8) == 2;

    case 6:
        return static_cast<char>(
            IsSpecialTypeCode(CSF_AT(std::int32_t, object, 0x50)));

    case 10:
        if ((CSF_AT(std::uint32_t, object, 0x170) & 0x2000) == 0)
            return 0;
        if (!static_cast<std::uint8_t>(SceneObj_IsInactive(object, nullptr)))
            return 0;
        if (!static_cast<std::uint8_t>(IsStateCompatible(
                reinterpret_cast<void*>(self->field_0008),
                nullptr, CSF_AT(std::int32_t, object, 0x78))))
            return 0;
        return 1;

    case 11:
        if ((CSF_AT(std::uint32_t, object, 0x170) & 0x2000) == 0)
            return 0;
        if (!static_cast<std::uint8_t>(IsStateCompatible(
                reinterpret_cast<void*>(self->field_0008),
                nullptr, CSF_AT(std::int32_t, object, 0x78))))
            return 0;
        return CSF_AT(std::int32_t, object, 0x1D8) == 3;

    case 13:
        if (CSF_AT(std::int32_t, object, 0x50))
            return 0;
        return (CSF_AT(std::uint32_t, object, 0x2DC) & 0x80000000) == 0;

    case 14:
        if ((CSF_AT(std::uint32_t, object, 0x170) & 0x2000) == 0)
            return 0;
        if (!static_cast<std::uint8_t>(IsDistinctNonSentinelValue(
                reinterpret_cast<void*>(self->field_0008),
                nullptr, CSF_AT(std::int32_t, object, 0x78))))
            return 0;
        if (static_cast<std::uint8_t>(SceneObj_IsInactive(object, nullptr)))
            return 0;
        if (static_cast<std::uint8_t>(
                SceneObject_PassesFirstCheck(object, nullptr)))
            return 0;
        if (CSF_AT(std::int32_t, object, 0x50))
            return 1;
        if (static_cast<std::uint8_t>(
                CharActionFsm_IsFlag10000SetAndByteNonZero(object, nullptr)))
            return 0;
        return Hud_GetAlternateSceneSelection(object, nullptr) == 0;

    default:
        return 0;
    }
}

HOOK(0x004E53A0, C_0083e9d0_MatchesSceneObjectCondition, "auto");
