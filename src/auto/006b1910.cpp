#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(void, __fastcall, SkinGeom_InitFromSource, 0x006C5A40, (void* self, void* , int sourceHandle, char flagByte));
CSF_FUNC(int, __fastcall, C_008656d0_SetOrClearFlagBits, 0x006C5BE0, (C_008656d0* self, void* , int bitMask, char enable));
CSF_FUNC(char, __fastcall, C_008656d0_ApplyEntryParameters, 0x006C63D0, (C_008656d0* self, void* , int entryAddress));
CSF_FUNC(char, __fastcall, C_008656d0_ProcessIfBit20Set, 0x006C6870, (C_008656d0* self, void* , void* a2));
CSF_FUNC(char, __fastcall, S_006c5cd0_ResetAndCommitState, 0x006C7A20, (S_006c5cd0* self, void*));
CSF_FUNC(int, __fastcall, GfxWater_GetLoadedScene, 0x006CC3A0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, S_006cc4f0_ApplySceneState, 0x006CCFF0, (S_006cc4f0* self, void* , char state));
CSF_FUNC(int, __fastcall, ClumpManager_SelectWorld, 0x006D0FB0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, GfxWater_PrepareSceneObjects, 0x006E01D0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, GfxWater_CommitCachedObjects, 0x006E3C10, (void* self, void* edx));

static char __fastcall GfxWater_RenderSceneWithCamera(
    void* self, void* /*edx*/, int sourceHandle, S_006cc4f0* camera)
{
    const char state = static_cast<char>(reinterpret_cast<uintptr_t>(camera));

    SkinGeom_InitFromSource(
        reinterpret_cast<void*>(GetGlobalValue_6C5760()),
        nullptr, sourceHandle, state);

    const int loadedScene = GfxWater_GetLoadedScene(
        CSF_AT(void*, self, 0x5C), nullptr);
    ClumpManager_SelectWorld(
        CSF_AT(void*, self, 0x64), nullptr, loadedScene);

    const uint32_t previousState = CSF_AT(
        uint32_t, reinterpret_cast<void*>(GetGlobalValue_6C5760()), 0x08);

    void* scene = CSF_AT(void*, self, 0x5C);
    void* sceneSettings = CSF_AT(void*, scene, 0xBC);
    if (CSF_AT(uint8_t, sceneSettings, 0x34))
    {
        C_008656d0_SetOrClearFlagBits(
            reinterpret_cast<C_008656d0*>(GetGlobalValue_6C5760()),
            nullptr, 4096, 0);
        C_008656d0_SetOrClearFlagBits(
            reinterpret_cast<C_008656d0*>(GetGlobalValue_6C5760()),
            nullptr, 0x4000, 0);
        C_008656d0_SetOrClearFlagBits(
            reinterpret_cast<C_008656d0*>(GetGlobalValue_6C5760()),
            nullptr, 2, 0);
    }

    const char applied = S_006cc4f0_ApplySceneState(
        CSF_AT(S_006cc4f0*, self, 0x5C), nullptr, state);

    if (applied)
    {
        GfxWater_PrepareSceneObjects(CSF_AT(void*, self, 0x54), nullptr);

        for (uint32_t i = 0; i < CSF_AT(uint32_t, self, 0x10); ++i)
        {
            void* entry = CSF_AT(void**, self, 0x08)[i];
            C_008656d0_ProcessIfBit20Set(
                reinterpret_cast<C_008656d0*>(GetGlobalValue_6C5760()),
                nullptr, entry);
        }

        void* provider = GetCachedObjectProvider(
            CSF_AT(void*, self, 0x64), nullptr);
        GfxWater_CommitCachedObjects(provider, nullptr);

        for (uint32_t i = 0; i < CSF_AT(uint32_t, self, 0x84); ++i)
        {
            const int entryAddress = CSF_AT(int*, self, 0x7C)[i];
            C_008656d0_ApplyEntryParameters(
                reinterpret_cast<C_008656d0*>(GetGlobalValue_6C5760()),
                nullptr, entryAddress);
        }
    }

    const char committed = S_006c5cd0_ResetAndCommitState(
        reinterpret_cast<S_006c5cd0*>(GetGlobalValue_6C5760()), nullptr);
    const char result = static_cast<char>(committed & applied);

    CSF_AT(uint32_t, reinterpret_cast<void*>(GetGlobalValue_6C5760()), 0x08)
        = previousState;
    return result;
}

HOOK(0x006B1910, GfxWater_RenderSceneWithCamera, "auto");
