#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, S_006fdfe0_FindWorldFromClump, 0x0058DF40, (int a1));
CSF_FUNC(int, __cdecl, RpWorldRemoveClump, 0x0058E310, (int a1, int a2));
CSF_FUNC(int, __cdecl, RpClumpDestroy, 0x0058F740, (int a1));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(char, __cdecl, GfxEnvironment_PrepareClumpResources, 0x006BCB50, (int a1));
CSF_FUNC(char, __cdecl, GfxEnvironment_IsClumpInHierarchy, 0x006BCE70, (int a1, int a2));
CSF_FUNC(char, __fastcall, GfxEnvironment_FinalizeClumpPreparation, 0x006BD130, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GfxEnvironment_CollectClumps, 0x006C0BC0, (int a1, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __stdcall, GfxSkin_ForwardWithSkinCallback, 0x006C5CB0, (int a1));
CSF_FUNC(char, __fastcall, GfxEnvironment_RegisterPreparedScene, 0x006ECBE0, (void* self, void* edx, void* a2, char a3));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

static char __fastcall C_00833708_PrepareClump(C_00833708* self, void* /*edx*/)
{
    const char prepared = CSF_AT(char, self, 0x91);
    if (prepared)
        return prepared;

    alignas(8) std::uint8_t clumpListBytes[0x18];
    CSF_AT(std::uint32_t, clumpListBytes, 0x00) = 0;
    CSF_AT(std::uint32_t, clumpListBytes, 0x04) = 0;
    CSF_AT(std::uint32_t, clumpListBytes, 0x08) = 0;
    CSF_AT(int, clumpListBytes, 0x0C) = 10;

    GfxEnvironment_CollectClumps(
        self->field_0004, reinterpret_cast<int>(clumpListBytes));

    for (int index = static_cast<int>(
             CSF_AT(std::uint32_t, clumpListBytes, 0x08) - 1);
         index >= 0; --index)
    {
        if (!GfxEnvironment_IsClumpInHierarchy(
                reinterpret_cast<int*>(CSF_AT(std::uint32_t, clumpListBytes, 0))[index],
                self->field_0004))
        {
            const int clump =
                reinterpret_cast<int*>(CSF_AT(std::uint32_t, clumpListBytes, 0))[index];
            GetGlobalValue_6C5760();
            GfxSkin_ForwardWithSkinCallback(clump);
            RpClumpDestroy(
                reinterpret_cast<int*>(CSF_AT(std::uint32_t, clumpListBytes, 0))[index]);
        }
    }

    ((void(__fastcall*)(void*, void*))
        CSF_AT(void**, self, 0)[0x38 / 4])(self, nullptr);

    for (std::uint32_t index = 0; index < self->field_0014; ++index)
    {
        GfxEnvironment_PrepareClumpResources(
            reinterpret_cast<int*>(self->field_000c)[index]);
        if (!GfxEnvironment_IsClumpInHierarchy(
                reinterpret_cast<int*>(self->field_000c)[index], self->field_0004))
        {
            const int world = S_006fdfe0_FindWorldFromClump(
                reinterpret_cast<int*>(self->field_000c)[index]);
            if (world)
                RpWorldRemoveClump(world, reinterpret_cast<int*>(self->field_000c)[index]);
        }
    }

    for (int index = static_cast<int>(CSF_AT(std::uint32_t, self, 0x24) - 1);
         index >= 0; --index)
    {
        GfxEnvironment_PrepareClumpResources(
            reinterpret_cast<int*>(CSF_AT(std::uint32_t, self, 0x1C))[index]);
        if (!GfxEnvironment_IsClumpInHierarchy(
                reinterpret_cast<int*>(CSF_AT(std::uint32_t, self, 0x1C))[index],
                self->field_0004))
        {
            const int world = S_006fdfe0_FindWorldFromClump(
                reinterpret_cast<int*>(CSF_AT(std::uint32_t, self, 0x1C))[index]);
            if (world)
                RpWorldRemoveClump(
                    world, reinterpret_cast<int*>(CSF_AT(std::uint32_t, self, 0x1C))[index]);

            const int clump =
                reinterpret_cast<int*>(CSF_AT(std::uint32_t, self, 0x1C))[index];
            GetGlobalValue_6C5760();
            GfxSkin_ForwardWithSkinCallback(clump);
            RpClumpDestroy(
                reinterpret_cast<int*>(CSF_AT(std::uint32_t, self, 0x1C))[index]);

            const std::uint32_t remaining = CSF_AT(std::uint32_t, self, 0x24) - 1;
            CSF_AT(std::uint32_t, self, 0x24) = remaining;
            if (static_cast<std::uint32_t>(index) != remaining)
            {
                int* destination =
                    reinterpret_cast<int*>(CSF_AT(std::uint32_t, self, 0x1C)) + index;
                _memcpy(destination, destination + 1,
                        4 * (remaining - static_cast<std::uint32_t>(index)));
            }
        }
    }

    std::uint8_t* group = reinterpret_cast<std::uint8_t*>(self) + 0x2C;
    for (int groupsRemaining = 3; groupsRemaining; --groupsRemaining, group += 0x10)
    {
        const std::uint32_t count = CSF_AT(std::uint32_t, group, 0x08);
        for (std::uint32_t index = 0; index < count; ++index)
        {
            GfxEnvironment_PrepareClumpResources(
                reinterpret_cast<int*>(CSF_AT(std::uint32_t, group, 0))[index]);
        }
    }

    const int globalManager = GetGlobalInt();
    void* clumpManager = reinterpret_cast<void*>(
        ResolveClumpManager(globalManager, nullptr));
    ((void(__fastcall*)(void*, void*, std::uint32_t))
        CSF_AT(void**, clumpManager, 0)[0x88 / 4])(
            clumpManager, nullptr, self->field_0004);

    GfxEnvironment_FinalizeClumpPreparation(self, nullptr);
    CSF_AT(char, self, 0x91) = 1;

    void* sceneController = reinterpret_cast<void*>(
        SceneLoader_GetSceneController(reinterpret_cast<void*>(GetGlobalInt()), nullptr));
    void* scene = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, sceneController, 0)[0x38 / 4])(sceneController, nullptr);
    GfxEnvironment_RegisterPreparedScene(scene, nullptr, self, 1);

    const std::uint32_t allocation = CSF_AT(std::uint32_t, clumpListBytes, 0);
    if (allocation)
        return static_cast<char>(Mem_Free(reinterpret_cast<void*>(allocation)));
    return 0;
}

HOOK(0x006BD3D0, C_00833708_PrepareClump, "auto");
