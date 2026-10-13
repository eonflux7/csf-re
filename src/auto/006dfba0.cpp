#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(unsigned int, __fastcall, SceneEntryList_RemoveValueAndNotify, 0x006B1840, (void* self, void* , int item));
CSF_FUNC(int, __fastcall, C_008611d4_ClearActiveRenderObject, 0x006D81D0, (C_008611d4* self, void*));
CSF_FUNC(int, __cdecl, GetValue_8BB738, 0x006E04A0, ());
CSF_FUNC(int*, __fastcall, GfxParticleEmitters_ClearEmitterResources, 0x006E22E0, (void* self, void*));
CSF_FUNC(char, __fastcall, AtomicEmitter_InitAndApplyRegion, 0x00702840, (void* self, void* , int arg0));
CSF_FUNC(int, __fastcall, C_00863180_ReleaseEmitterObjects, 0x007028C0, (C_00863180* self, void*));

static int* __fastcall GfxScene_ResetObjectRegistry(void* self, void* /*edx*/)
{
    C_00863180_ReleaseEmitterObjects(
        CSF_AT(C_00863180*, self, 0x44), nullptr);
    AtomicEmitter_InitAndApplyRegion(
        CSF_AT(void*, self, 0x44), nullptr, CSF_AT(int, self, 0x08));

    for (std::uint32_t i = 0; i < CSF_AT(std::uint32_t, self, 0x1C); ++i)
    {
        void* object = CSF_AT(void**, self, 0x14)[i];
        if (object)
        {
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, object, 0)[1])(object, nullptr, 1);
            CSF_AT(void**, self, 0x14)[i] = nullptr;
        }
    }

    void* firstRegistry = CSF_AT(void*, self, 0x14);
    CSF_AT(std::uint32_t, self, 0x18) = 0;
    CSF_AT(std::uint32_t, self, 0x1C) = 0;
    if (firstRegistry)
    {
        CoreMemory_Free(firstRegistry);
        CSF_AT(void*, self, 0x14) = nullptr;
    }

    for (std::uint32_t i = 0; i < CSF_AT(std::uint32_t, self, 0x2C); ++i)
    {
        void* object = CSF_AT(void**, self, 0x24)[i];
        if (object)
        {
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, object, 0)[1])(object, nullptr, 1);
            CSF_AT(void**, self, 0x24)[i] = nullptr;
        }
    }

    void* secondRegistry = CSF_AT(void*, self, 0x24);
    CSF_AT(std::uint32_t, self, 0x28) = 0;
    CSF_AT(std::uint32_t, self, 0x2C) = 0;
    if (secondRegistry)
    {
        CoreMemory_Free(secondRegistry);
        CSF_AT(void*, self, 0x24) = nullptr;
    }

    C_008611d4* renderObject = CSF_AT(C_008611d4*, self, 0x48);
    if (renderObject)
        C_008611d4_ClearActiveRenderObject(renderObject, nullptr);

    int sceneEntry = CSF_AT(int, self, 0x5C);
    if (sceneEntry)
    {
        void* entryList = reinterpret_cast<void*>(GetGlobalInt());
        SceneEntryList_RemoveValueAndNotify(entryList, nullptr, sceneEntry);
        CSF_AT(int, self, 0x5C) = 0;
    }

    void* emitterManager = reinterpret_cast<void*>(GetValue_8BB738());
    return GfxParticleEmitters_ClearEmitterResources(emitterManager, nullptr);
}

HOOK(0x006DFBA0, GfxScene_ResetObjectRegistry, "auto");
