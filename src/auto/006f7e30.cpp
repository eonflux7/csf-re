#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, GfxEnvironment_WriteToFile, 0x006BDDC0, (void* self, void* , const char* filename));
CSF_FUNC(int, __fastcall, GetSceneEntrySlotValue, 0x006CC7F0, (void* self, void* , int index));
CSF_FUNC(bool, __fastcall, C_00863ac0_ConsumeWorldSectorFlag, 0x00708870, (C_00863ac0* self, void* , C_00863ac0* a2));
CSF_FUNC(bool, __stdcall, SceneLoader_SaveWorldStream, 0x007088D0, (int a1, C_00863ac0* a2));
CSF_FUNC(char, __stdcall, GfxAnimInterpolator_WriteSectorClumpsWithExtension, 0x00708980, (char* extension));

static char __fastcall C_00862944_SaveSceneResources(
    C_00862944* self, void* /*edx*/, C_0085e7b8* providerOwner, int outputFiles)
{
    void* global = reinterpret_cast<void*>(GetGlobalInt());
    void* scene = reinterpret_cast<void*>(
        SceneLoader_GetSceneController(global, nullptr));

    global = reinterpret_cast<void*>(GetGlobalInt());
    void* accessor = reinterpret_cast<void*>(
        GlobalInt_ResolveAccessor(global, nullptr));
    void* textureList = ((void* (__fastcall*)(void*, void*))
        CSF_AT(void**, accessor, 0)[0x10 / 4])(accessor, nullptr);
    ((void (__fastcall*)(void*, void*, const char*))
        CSF_AT(void**, textureList, 0)[0x30 / 4])(
            textureList, nullptr, CSF_STR(0x00862954, "TexList.txt"));

    void* environment = ((void* (__fastcall*)(void*, void*))
        CSF_AT(void**, scene, 0)[0x40 / 4])(scene, nullptr);
    void** environmentVtable = CSF_AT(void**, environment, 0);
    void* provider = GetCachedObjectProvider(providerOwner, nullptr);
    if (!((bool (__fastcall*)(void*, void*, void*))
        environmentVtable[0x20 / 4])(environment, nullptr, provider))
        return 0;

    const char* environmentFilename =
        CSF_AT(const char*, reinterpret_cast<void*>(outputFiles), 0x4);
    environment = ((void* (__fastcall*)(void*, void*))
        CSF_AT(void**, scene, 0)[0x40 / 4])(scene, nullptr);
    if (!GfxEnvironment_WriteToFile(environment, nullptr, environmentFilename))
        return 0;

    C_00863ac0* world = ((C_00863ac0* (__fastcall*)(void*, void*))
        CSF_AT(void**, scene, 0)[0x44 / 4])(scene, nullptr);
    if (C_00863ac0_ConsumeWorldSectorFlag(
            reinterpret_cast<C_00863ac0*>(self), nullptr, world))
    {
        world = ((C_00863ac0* (__fastcall*)(void*, void*))
            CSF_AT(void**, scene, 0)[0x44 / 4])(scene, nullptr);
        int worldFilename =
            CSF_AT(int, reinterpret_cast<void*>(outputFiles), 0x8);
        if (!SceneLoader_SaveWorldStream(worldFilename, world))
            return 0;
    }

    if (!GfxAnimInterpolator_WriteSectorClumpsWithExtension(
            const_cast<char*>(CSF_STR(0x0086294C, ".rpc"))))
        return 0;

    const uint32_t entryCount = ((uint32_t (__fastcall*)(void*, void*))
        CSF_AT(void**, scene, 0)[0x88 / 4])(scene, nullptr);
    for (uint32_t index = 0; index < entryCount; ++index)
    {
        C_00863ac0* sector =
            ((C_00863ac0* (__fastcall*)(void*, void*, uint32_t))
                CSF_AT(void**, scene, 0)[0x90 / 4])(scene, nullptr, index);
        if (C_00863ac0_ConsumeWorldSectorFlag(
                reinterpret_cast<C_00863ac0*>(self), nullptr, sector))
        {
            sector = ((C_00863ac0* (__fastcall*)(void*, void*, uint32_t))
                CSF_AT(void**, scene, 0)[0x90 / 4])(scene, nullptr, index);
            int filename = GetSceneEntrySlotValue(
                scene, nullptr, static_cast<int>(index));
            if (!SceneLoader_SaveWorldStream(filename, sector))
                return 0;
            GetSceneEntrySlotValue(scene, nullptr, static_cast<int>(index));
        }
    }
    return 1;
}

HOOK(0x006F7E30, C_00862944_SaveSceneResources, "auto");
