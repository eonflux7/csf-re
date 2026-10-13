#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpClumpForAllLights, 0x0058EF90,
         (int a1, int (__cdecl *a2)(char*, int), int a3));
CSF_FUNC(int, __cdecl, RpClumpRemoveLight, 0x0058F1D0, (int a1, int a2));
CSF_FUNC(int, __cdecl, RpLightDestroy, 0x0059D000, (int a1));
CSF_FUNC(int, __cdecl, RwFrameTransform, 0x00623860, (int a1, int a2, int a3));
CSF_FUNC(void*, __cdecl, RwFrameSetIdentity, 0x006238C0, (void* frame));
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __cdecl, GfxObjectProps_UpdateLightType, 0x006B86A0, (int objectAddress));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectPropertyWord, 0x006B8800, (int objectIndex));
CSF_FUNC(int, __fastcall, SceneLoader_SetClumpContext, 0x006D1030, (void* self, void* , int newContext));

static int __fastcall C_00833708_FinalizeClumpPreparation(
    C_00833708* self, void* /*edx*/)
{
    void* manager = reinterpret_cast<void*>(
        ResolveClumpManager(GetGlobalInt(), nullptr));

    alignas(8) uint8_t lightArrayBytes[0x28];
    CSF_AT(uint32_t, lightArrayBytes, 0x00) = 0;
    CSF_AT(uint32_t, lightArrayBytes, 0x04) = 0;
    CSF_AT(uint32_t, lightArrayBytes, 0x08) = 0;
    CSF_AT(int, lightArrayBytes, 0x0C) = 10;

    SceneLoader_SetClumpContext(
        manager, nullptr, static_cast<int>(self->field_0004));

    const uint32_t clumpCount = CSF_AT(uint32_t, self, 0x64);
    for (uint32_t clumpIndex = 0; clumpIndex < clumpCount; ++clumpIndex)
    {
        const int clump = reinterpret_cast<int*>(
            CSF_AT(uint32_t, self, 0x5C))[clumpIndex];

        CSF_AT(uint32_t, lightArrayBytes, 0x08) = 0;
        RpClumpForAllLights(
            clump,
            reinterpret_cast<int (__cdecl *)(char*, int)>(
                CSF_ADDR_FN(0x006BD100)),
            reinterpret_cast<int>(lightArrayBytes));

        const uint32_t lightCount =
            CSF_AT(uint32_t, lightArrayBytes, 0x08);
        for (uint32_t lightIndex = 0; lightIndex < lightCount; ++lightIndex)
        {
            const int light = reinterpret_cast<int*>(
                CSF_AT(uint32_t, lightArrayBytes, 0x00))[lightIndex];
            void* lightObject = reinterpret_cast<void*>(light);

            if (CSF_AT(uint8_t, lightObject, 0x01) == 2)
            {
                RpClumpRemoveLight(clump, light);
                RpLightDestroy(light);
            }
            else
            {
                const int lightFrame = CSF_AT(int, lightObject, 0x04);
                const int clumpFrame =
                    CSF_AT(int, reinterpret_cast<void*>(clump), 0x04);
                if (lightFrame != clumpFrame)
                {
                    RwFrameTransform(
                        lightFrame,
                        reinterpret_cast<int>(
                            reinterpret_cast<uint8_t*>(clumpFrame) + 0x10),
                        2);
                    RwFrameSetIdentity(reinterpret_cast<void*>(clumpFrame));
                }

                GfxObjectProps_UpdateLightType(light);
                const int propertyAddress =
                    GfxObjectProps_GetObjectPropertyWord(light);
                if (CSF_AT(int, reinterpret_cast<void*>(propertyAddress), 0) == 2)
                {
                    reinterpret_cast<void (__fastcall *)(void*, void*, int)>(
                        CSF_AT(void**, manager, 0)[0x30 / 4])(
                            manager, nullptr, light);
                }
                else
                {
                    reinterpret_cast<void (__fastcall *)(void*, void*, int)>(
                        CSF_AT(void**, manager, 0)[0x24 / 4])(
                            manager, nullptr, light);
                }
            }
        }
    }

    SceneLoader_SetClumpContext(manager, nullptr, 0);
    const uint32_t lightStorage = CSF_AT(uint32_t, lightArrayBytes, 0x00);
    if (lightStorage)
        return Mem_Free(reinterpret_cast<void*>(lightStorage));
    return 0;
}

HOOK(0x006BD130, C_00833708_FinalizeClumpPreparation, "auto");
