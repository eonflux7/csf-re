#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, S_00413570_AppendRecord, 0x00413C10, (S_00413570* self, void* , int a2, int a3));
CSF_FUNC(char, __stdcall, SceneSelection_IsHandleEligible, 0x00445940, (int a1, int a2));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, IsDistinctNonSentinelValue, 0x0048E0D0, (void* self, void* , std::int32_t candidate));
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, IsSpecialObjectType, 0x00755940, (int objectType));

static void __fastcall S_00445b40_UpdateObjectLists(
    S_00445b40* self, void* /*edx*/, float elapsedTime)
{
    const float remainingTime = self->field_003c - elapsedTime;
    self->field_003c = remainingTime;
    if (remainingTime > g_fZero)
        return;

    self->field_003c = 0.5f;
    self->field_0048 = 0;
    self->field_0058 = 0;

    const std::uint32_t objectCount =
        CSF_AT(std::uint32_t, g_pCurrentScene, 0x18);

    for (std::uint32_t index = 0; index < objectCount; ++index)
    {
        void** objects = CSF_AT(void**, g_pCurrentScene, 0x10);
        void* object = objects[index];
        if (!object)
            continue;

        if (((bool(__fastcall*)(void*, void*))
                 CSF_AT(void**, object, 0)[0x7C / 4])(object, nullptr))
            continue;

        const int objectType = CSF_AT(int, object, 0x50);
        if (objectType == 12 || objectType == 13 ||
            objectType == 26 || objectType == 27 ||
            objectType == 28 || objectType == 32)
            continue;

        if (static_cast<std::uint8_t>(IsSpecialObjectType(objectType)) != 0)
            continue;

        void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
        if (!((CSF_AT(std::uint8_t, globalState, 0xD0) == 0 &&
               static_cast<std::uint8_t>(
                   SceneObj_IsInactive(object, nullptr)) != 0 &&
               (CSF_AT(std::uint8_t, object, 0x94) & 0x12) == 0) ||
              (CSF_AT(std::uint8_t, object, 0x94) & 5) != 0))
            continue;

        const int handle =
            reinterpret_cast<int>(static_cast<std::uint8_t*>(object) + 0x1C);

        if (SceneSelection_IsHandleEligible(1, handle))
        {
            S_00413570_AppendRecord(
                reinterpret_cast<S_00413570*>(
                    reinterpret_cast<std::uint8_t*>(self) + 0x40),
                nullptr, handle, 0);
        }

        void* resolvedObject = reinterpret_cast<void*>(
            CHandleTable_Resolve(g_pCurrentScene, nullptr, handle));

        if (resolvedObject &&
            (CSF_AT(std::uint32_t, resolvedObject, 0x170) & 0x2000) != 0)
        {
            if (static_cast<std::uint8_t>(
                    IsDistinctNonSentinelValue(resolvedObject, nullptr, 0)) != 0)
            {
                S_00413570_AppendRecord(
                    reinterpret_cast<S_00413570*>(
                        reinterpret_cast<std::uint8_t*>(self) + 0x50),
                    nullptr, handle, 0);
            }
        }
    }
}

HOOK(0x00447800, S_00445b40_UpdateObjectLists, "auto");
