#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_0082d58c_ResetField0008, 0x00409290, (C_0082d58c* self, void*));
CSF_FUNC(int, __cdecl, RwFreeListPurgeAllFreeLists, 0x006265F0, ());
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, RenderHandle_FromObject, 0x006B2D50, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, SceneLoader_ResetRenderWorld, 0x006B70D0, (void* self, void* edx));
CSF_FUNC(char, __cdecl, GfxScene_ResetStreamingState, 0x006BA250, ());
CSF_FUNC(int, __fastcall, SceneLoader_ClearRenderResources, 0x006BDAE0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, SceneLoader_FinalizeRenderResources, 0x006BDCB0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __fastcall, C_008608e8_ReleaseChildrenAndClear, 0x006C7020, (C_008608e8* self, void*));
CSF_FUNC(char, __fastcall, GfxScene_ReleaseSceneData, 0x006CB0E0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, SceneLoader_SetRenderResetFlag, 0x006CB3A0, (void* self, void* edx, char a2));
CSF_FUNC(char*, __fastcall, GfxScene_RemoveSceneEntry, 0x006CD390, (void* self, void* edx, char* a2));
CSF_FUNC(char, __fastcall, GfxScene_ClearClumpManager, 0x006D11B0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxScene_ResetRenderHandle, 0x006D6760, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxScene_ClearObjectRegistry, 0x006DFAE0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxScene_ReleaseAuxiliarySceneData, 0x006F6060, (void* self, void* edx));
CSF_FUNC(char, __cdecl, GfxScene_ResetSceneCaches, 0x006F7D80, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));
CSF_FUNC(void, __fastcall, SceneLoader_ClearWorldResources, 0x007512A0, (void* self, void* edx));

static int __fastcall C_00860bb8_ReleaseSceneResources(
    C_00860bb8* self, void* /*edx*/)
{
    SceneLoader_SetRenderResetFlag(self, nullptr, 1);

    void* sceneData = reinterpret_cast<void*>(self->field_00bc);
    if (sceneData)
    {
        GfxScene_ReleaseSceneData(
            reinterpret_cast<void*>(self->field_00bc), nullptr);
        Mem_Free(sceneData);
        self->field_00bc = 0;
    }

    void* auxiliaryData = reinterpret_cast<void*>(self->field_00c4);
    if (auxiliaryData)
    {
        GfxScene_ReleaseAuxiliarySceneData(
            reinterpret_cast<void*>(self->field_00c4), nullptr);
        Mem_Free(auxiliaryData);
        self->field_00c4 = 0;
    }

    if (self->field_0069)
        self->vSlot30();

    uint32_t entryIndex = 0;
    if (self->field_007c)
    {
        do
        {
            auto* entries = reinterpret_cast<uint32_t*>(self->field_0074);
            void* entry = reinterpret_cast<void*>(entries[2 * entryIndex]);
            if (entry)
            {
                C_0082d58c_ResetField0008(
                    reinterpret_cast<C_0082d58c*>(entries[2 * entryIndex]),
                    nullptr);
                if (CSF_AT(uint32_t, entry, 0))
                {
                    Mem_Free(reinterpret_cast<void*>(CSF_AT(uint32_t, entry, 0)));
                    CSF_AT(uint32_t, entry, 0) = 0;
                }
                Mem_Free(entry);
                reinterpret_cast<uint32_t*>(self->field_0074)[2 * entryIndex] = 0;
            }
            ++entryIndex;
        } while (entryIndex < self->field_007c);
    }

    void* entries = reinterpret_cast<void*>(self->field_0074);
    self->field_0078 = 0;
    self->field_007c = 0;
    if (entries)
    {
        Mem_Free(entries);
        self->field_0074 = 0;
    }

    void* auxiliaryEntries = reinterpret_cast<void*>(self->field_0084);
    self->field_0088 = 0;
    self->field_008c = 0;
    if (auxiliaryEntries)
    {
        Mem_Free(auxiliaryEntries);
        self->field_0084 = 0;
    }

    void* ownedObject = reinterpret_cast<void*>(self->field_0064);
    if (ownedObject)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, ownedObject, 0)[1])(ownedObject, nullptr, 1);
        self->field_0064 = 0;
    }

    for (int32_t index = static_cast<int32_t>(self->field_00a8 - 1);
         index >= 0; --index)
    {
        GfxScene_RemoveSceneEntry(self, nullptr, reinterpret_cast<char*>(index));
    }

    void* sceneEntries = reinterpret_cast<void*>(self->field_00a0);
    self->field_00a4 = 0;
    self->field_00a8 = 0;
    if (sceneEntries)
    {
        Mem_Free(sceneEntries);
        self->field_00a0 = 0;
    }

    void* registry = reinterpret_cast<void*>(SceneLoader_GetObjectRegistry(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr));
    GfxScene_ClearObjectRegistry(registry, nullptr);
    self->field_0068 = 0;

    int clumpManager = ResolveClumpManager(GetGlobalInt(), nullptr);
    if (clumpManager)
        GfxScene_ClearClumpManager(reinterpret_cast<void*>(clumpManager), nullptr);

    for (uint32_t index = 0; index < self->field_001c; ++index)
    {
        void* object = reinterpret_cast<void*>(
            reinterpret_cast<uint32_t*>(self->field_0014)[index]);
        if (object)
        {
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, object, 0)[1])(object, nullptr, 1);
            reinterpret_cast<uint32_t*>(self->field_0014)[index] = 0;
        }
    }

    void* objects = reinterpret_cast<void*>(self->field_0014);
    self->field_0018 = 0;
    self->field_001c = 0;
    if (objects)
    {
        Mem_Free(objects);
        self->field_0014 = 0;
    }

    void* finalEntries = reinterpret_cast<void*>(self->field_00c8);
    self->field_00cc = 0;
    self->field_00d0 = 0;
    if (finalEntries)
    {
        Mem_Free(finalEntries);
        self->field_00c8 = 0;
    }

    SceneLoader_SetRenderResetFlag(self, nullptr, 0);
    if (self->field_006a)
    {
        void* renderHandle = reinterpret_cast<void*>(RenderHandle_FromObject(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr));
        if (renderHandle)
            GfxScene_ResetRenderHandle(renderHandle, nullptr);
    }

    SceneLoader_ClearRenderResources(
        reinterpret_cast<void*>(self->field_0008), nullptr);
    SceneLoader_ClearWorldResources(
        reinterpret_cast<void*>(self->field_000c), nullptr);
    SceneLoader_FinalizeRenderResources(
        reinterpret_cast<void*>(self->field_0008), nullptr);
    SceneLoader_ResetRenderWorld(
        reinterpret_cast<void*>(self->field_000c), nullptr);

    void* extraRenderResources = reinterpret_cast<void*>(self->field_0098);
    if (extraRenderResources)
        SceneLoader_FinalizeRenderResources(extraRenderResources, nullptr);

    void* extraWorldResources = reinterpret_cast<void*>(self->field_009c);
    if (extraWorldResources)
        SceneLoader_ResetRenderWorld(extraWorldResources, nullptr);

    void* sceneObject = reinterpret_cast<void*>(self->field_0010);
    if (sceneObject)
    {
        ((void(__fastcall*)(void*, void*))
            CSF_AT(void**, sceneObject, 0)[2])(sceneObject, nullptr);
    }

    void* accessor = reinterpret_cast<void*>(GlobalInt_ResolveAccessor(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr));
    ((void(__fastcall*)(void*, void*))
        CSF_AT(void**, accessor, 0)[7])(accessor, nullptr);

    GfxScene_ResetSceneCaches();
    GfxScene_ResetStreamingState();
    C_008608e8_ReleaseChildrenAndClear(
        reinterpret_cast<C_008608e8*>(GetGlobalValue_6C5760()), nullptr);
    return RwFreeListPurgeAllFreeLists();
}

HOOK(0x006CD4B0, C_00860bb8_ReleaseSceneResources, "auto");
