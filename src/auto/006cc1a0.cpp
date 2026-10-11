#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, MissionScript_ProcessEntry_4419F0, 0x004419F0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, RwFreeListPurgeAllFreeLists, 0x006265F0, ());
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(int, __cdecl, SetAllocationMode, 0x006910B0, (int mode));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetFpsStatsFromGlobal, 0x006B2B50, (void* self, void* edx));
CSF_FUNC(int, __fastcall, RenderHandle_FromObject, 0x006B2D50, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetWorldObjectCollection, 0x006B6AF0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, ConsumeFlagByte26, 0x006B6BD0, (void* self, void*));
CSF_FUNC(char, __fastcall, GfxScene_ResetPrimaryObject, 0x006BC7F0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __fastcall, C_008608e8_ClearBuffers, 0x006C6E60, (C_008608e8* self, void*));
CSF_FUNC(int, __fastcall, CharacterDefs_AssignEmbeddedMember, 0x006CAF70, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SceneLoader_EndRenderReset, 0x006CBC10, (void* self, void* edx));
CSF_FUNC(char, __fastcall, GfxScene_ResetRenderHandle, 0x006D7260, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxScene_ResetEntryLookup, 0x006DAD70, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxScene_ResetObjectRegistry, 0x006DFBA0, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, GfxScene_ResetStreamingObject, 0x006EF440, (void* self, void* edx));
CSF_FUNC(uint32_t*, __cdecl, GfxScene_ResetGlobalSceneState, 0x006F7DB0, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));

static void* __fastcall C_00860bb8_ResetScene(C_00860bb8* self, void* /*edx*/)
{
    alignas(8) uint8_t entryBytes[0x10];

    void* streamingObject = reinterpret_cast<void*>(self->field_0064);
    if (streamingObject)
        GfxScene_ResetStreamingObject(streamingObject, nullptr);

    uint32_t allocation = self->field_0084;
    self->field_0088 = 0;
    self->field_008c = 0;
    if (allocation)
    {
        Mem_Free(reinterpret_cast<void*>(allocation));
        self->field_0084 = 0;
    }

    SceneLoader_EndRenderReset(self, nullptr);
    if (self->field_006a)
    {
        void* globalObject = reinterpret_cast<void*>(GetGlobalInt());
        int renderHandle = RenderHandle_FromObject(globalObject, nullptr);
        if (renderHandle)
            GfxScene_ResetRenderHandle(reinterpret_cast<void*>(renderHandle), nullptr);
    }

    void* globalObject = reinterpret_cast<void*>(GetGlobalInt());
    void* accessor = reinterpret_cast<void*>(GlobalInt_ResolveAccessor(globalObject, nullptr));
    SceneLoader_GetWorldObjectCollection(accessor, nullptr);
    SceneLoader_NoOp();

    void* firstCollection = reinterpret_cast<void*>(self->vSlot38());
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, firstCollection, 0)[1])(
        firstCollection, nullptr);

    void* secondCollection = reinterpret_cast<void*>(self->vSlot3C());
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, secondCollection, 0)[1])(
        secondCollection, nullptr);

    GfxScene_ResetPrimaryObject(reinterpret_cast<void*>(self->field_0008), nullptr);
    ConsumeFlagByte26(reinterpret_cast<void*>(self->field_000c), nullptr);

    globalObject = reinterpret_cast<void*>(GetGlobalInt());
    void* registry = reinterpret_cast<void*>(SceneLoader_GetObjectRegistry(globalObject, nullptr));
    GfxScene_ResetObjectRegistry(registry, nullptr);
    GfxScene_ResetGlobalSceneState();

    int managerId = GetGlobalInt();
    void* clumpManager = reinterpret_cast<void*>(ResolveClumpManager(managerId, nullptr));
    void* entryLookup = reinterpret_cast<void*>(ResolveEntryLookupValue(clumpManager, nullptr));
    GfxScene_ResetEntryLookup(entryLookup, nullptr);

    globalObject = reinterpret_cast<void*>(GetGlobalInt());
    void* fpsStats = reinterpret_cast<void*>(GetFpsStatsFromGlobal(globalObject, nullptr));
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, fpsStats, 0)[3])(fpsStats, nullptr);

    C_008608e8_ClearBuffers(
        reinterpret_cast<C_008608e8*>(GetGlobalValue_6C5760()), nullptr);
    RwFreeListPurgeAllFreeLists();

    MissionScript_ProcessEntry_4419F0(entryBytes, nullptr, &self->field_0014);
    allocation = self->field_0014;
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x1C) = 0;
    if (allocation)
    {
        Mem_Free(reinterpret_cast<void*>(allocation));
        self->field_0014 = 0;
    }
    SetAllocationMode(2);
    CharacterDefs_AssignEmbeddedMember(
        &self->field_0014, nullptr, reinterpret_cast<int>(entryBytes));
    SetAllocationMode(0);
    allocation = CSF_AT(uint32_t, entryBytes, 0);
    if (allocation)
        Mem_Free(reinterpret_cast<void*>(allocation));

    MissionScript_ProcessEntry_4419F0(entryBytes, nullptr, &self->field_0054);
    allocation = self->field_0054;
    CSF_AT(uint32_t, self, 0x58) = 0;
    CSF_AT(uint32_t, self, 0x5C) = 0;
    if (allocation)
    {
        Mem_Free(reinterpret_cast<void*>(allocation));
        self->field_0054 = 0;
    }
    SetAllocationMode(2);
    CharacterDefs_AssignEmbeddedMember(
        &self->field_0054, nullptr, reinterpret_cast<int>(entryBytes));
    SetAllocationMode(0);
    allocation = CSF_AT(uint32_t, entryBytes, 0);
    if (allocation)
        Mem_Free(reinterpret_cast<void*>(allocation));

    MissionScript_ProcessEntry_4419F0(entryBytes, nullptr, &self->field_00c8);
    allocation = self->field_00c8;
    CSF_AT(uint32_t, self, 0xCC) = 0;
    CSF_AT(uint32_t, self, 0xD0) = 0;
    if (allocation)
    {
        Mem_Free(reinterpret_cast<void*>(allocation));
        self->field_00c8 = 0;
    }
    SetAllocationMode(2);
    CharacterDefs_AssignEmbeddedMember(
        &self->field_00c8, nullptr, reinterpret_cast<int>(entryBytes));
    SetAllocationMode(0);

    void* result = reinterpret_cast<void*>(CSF_AT(uint32_t, entryBytes, 0));
    if (result)
        result = reinterpret_cast<void*>(Mem_Free(result));
    self->field_0069 = 0;
    return result;
}

HOOK(0x006CC1A0, C_00860bb8_ResetScene, "auto");
