#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, DestroySceneObjectOptionsStorage, 0x004018E0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(uint32_t*, __fastcall, InitializeSceneObjectOptions, 0x004777D0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_00833908_RegisterSceneResource, 0x0048E2B0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(unsigned int, __fastcall, C_00833908_SetDoorStateFlags, 0x00490D20, (C_00833908* self, void* edx, uint32_t a2, char a3));
CSF_FUNC(uint32_t, __fastcall, C_00833908_SetSceneMode, 0x00490D70, (void* self, void* edx, int mode));
CSF_FUNC(int, __fastcall, C_00833908_ConfigureSceneParameters, 0x00490EE0, (void* self, void* edx, int a2, int a3, int a4, int a5, int a6));
CSF_FUNC(int, __fastcall, C_00833908_ConfigureSceneTiming, 0x00490F60, (void* self, void* edx, int a2, float a3, int a4, int a5));
CSF_FUNC(int, __fastcall, C_00833908_SetPrimarySceneOption, 0x0049EBA0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_00833908_SetSecondarySceneOption, 0x0049EBC0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_00833908_SetTertiarySceneOption, 0x0049EBD0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_00833908_SetQuaternarySceneOption, 0x0049EBE0, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, C_00833908_SetSceneCallback, 0x0049F560, (void* self, void* edx, void* a2));
CSF_FUNC(uint32_t*, __fastcall, C_00833908_ConfigureMissionInterface, 0x0049F5A0, (void* self, void* edx, int a2, int a3, int a4, int a5));
CSF_FUNC(uint32_t*, __fastcall, C_00834790_ConstructForScene, 0x004A6C80, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, S_004e9090_LoadMissionScene, 0x004E9B80, (void* self, void* edx, int a2, char a3));
CSF_FUNC(void, __fastcall, C_00833908_SetSceneContext, 0x0055D860, (void* self, void* edx, void* a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

static int __fastcall S_004e9090_InitializeMissionScene(
    S_004e9090* self, void* /*edx*/,
    C_00833908* scene, C_00833908* missionKey)
{
    void* registry = reinterpret_cast<void*>(self->field_1fc8);
    auto* mission = ((std::uint8_t* (__fastcall*)(void*, void*, C_00833908*))
        CSF_AT(void**, registry, 0)[2])(registry, nullptr, missionKey);

    int loadedScene = S_004e9090_LoadMissionScene(
        self, nullptr, reinterpret_cast<int>(missionKey), 0);
    C_00833908_SetDoorStateFlags(scene, nullptr, loadedScene, 1);

    // All options, including their embedded strings, share one contiguous object.
    alignas(4) std::uint8_t options[0xB8] = {};
    InitializeSceneObjectOptions(options, nullptr);

    CSF_AT(uint32_t, options, 0) |= 0x40002;
    if (CSF_AT(uint8_t, mission, 0x159))
        CSF_AT(uint32_t, options, 0) |= 0x400;
    else
        CSF_AT(uint32_t, options, 0) &= 0xFFFFFBFF;

    if (CSF_AT(uint8_t, mission, 0x15A))
        CSF_AT(uint32_t, options, 0) &= 0xFFFFFFDF;
    else
        CSF_AT(uint32_t, options, 0) |= 0x20;

    if (CSF_AT(uint8_t, mission, 0x15B))
        CSF_AT(uint32_t, options, 0) |= 0x4000;
    else
        CSF_AT(uint32_t, options, 0) &= 0xFFFFBFFF;

    CSF_AT(int, options, 0x04) = CSF_AT(int, mission, 0x1F0);
    AssignStringSubstring(options + 0x08, nullptr, mission + 0x1F4, 0, -1);
    AssignStringSubstring(options + 0x24, nullptr, mission + 0x3C, 0, -1);

    CSF_AT(int, options, 0x58) = CSF_AT(int, mission, 0x220);
    CSF_AT(int, options, 0x5C) = CSF_AT(int, mission, 0x224);
    CSF_AT(int, options, 0x60) = CSF_AT(int, mission, 0x228);
    CSF_AT(uint8_t, options, 0x66) = CSF_AT(int, options, 0x04) != 0;
    CSF_AT(uint8_t, options, 0x64) = CSF_AT(uint8_t, mission, 0x21C);
    CSF_AT(uint8_t, options, 0x65) = CSF_AT(uint8_t, mission, 0x21D);
    CSF_AT(int, options, 0x68) = CSF_AT(int, mission, 0xCC);
    CSF_AT(int, options, 0x6C) = CSF_AT(int, mission, 0x160);
    CSF_AT(int, options, 0x70) = CSF_AT(int, mission, 0x164);
    CSF_AT(int, options, 0x74) = CSF_AT(int, mission, 0x168);

    AssignStringSubstring(options + 0x78, nullptr, mission + 0x16C, 0, -1);
    CSF_AT(uint32_t, options, 0x94) = CSF_AT(uint32_t, mission, 0x188) - 1;
    CSF_AT(int, options, 0x98) = CSF_AT(int, mission, 0x18C);
    CSF_AT(int, options, 0x9C) = CSF_AT(int, mission, 0x190);
    CSF_AT(int, options, 0xA0) = CSF_AT(int, mission, 0x194);
    CSF_AT(int, options, 0xA4) = CSF_AT(int, mission, 0xD0);

    void* loadedObject = reinterpret_cast<void*>(loadedScene);
    ((void (__fastcall*)(void*, void*, uint32_t*))
        CSF_AT(void**, loadedObject, 0)[4])(
            loadedObject, nullptr, reinterpret_cast<uint32_t*>(options));

    scene->field_0078 = 0;
    C_00833908_SetPrimarySceneOption(scene, nullptr, CSF_AT(int, mission, 0x1B4));
    C_00833908_SetSecondarySceneOption(scene, nullptr, CSF_AT(int, mission, 0x1B8));
    C_00833908_SetTertiarySceneOption(scene, nullptr, CSF_AT(int, mission, 0x1BC));
    C_00833908_SetQuaternarySceneOption(scene, nullptr, CSF_AT(int, mission, 0x1C0));
    C_00833908_SetSceneMode(scene, nullptr, 6);
    C_00833908_ConfigureSceneParameters(
        scene, nullptr, 3,
        CSF_AT(int, mission, 0x1C4), CSF_AT(int, mission, 0x1C8),
        CSF_AT(int, mission, 0x1CC), CSF_AT(int, mission, 0x1D0));
    C_00833908_ConfigureSceneTiming(
        scene, nullptr, 1, CSF_AT(float, mission, 0x120),
        CSF_AT(int, mission, 0x124), CSF_AT(int, mission, 0x128));

    void* contextStorage = Mem_Alloc(124);
    void* context = contextStorage
        ? C_00834790_ConstructForScene(
            contextStorage, nullptr, reinterpret_cast<int>(scene))
        : nullptr;
    C_00833908_SetSceneContext(scene, nullptr, context);

    missionKey = reinterpret_cast<C_00833908*>(CSF_ADDR_DATA(0x0083ED08));
    C_00833908_RegisterSceneResource(
        scene, nullptr, 19, reinterpret_cast<int>(&missionKey));
    C_00833908_SetSceneCallback(scene, nullptr, nullptr);
    C_00833908_ConfigureMissionInterface(scene, nullptr, 1, 1072, 1004, 5);

    if (CSF_AT(uint32_t, options, 0x90) >= 0x10 &&
        CSF_AT(void*, options, 0x7C))
        Mem_FreeDebug(CSF_AT(void*, options, 0x7C));

    CSF_AT(uint32_t, options, 0x90) = 15;
    CSF_AT(int, options, 0x8C) = 0;
    CSF_AT(uint8_t, options, 0x7C) = 0;
    return DestroySceneObjectOptionsStorage(options + 0x04, nullptr);
}

HOOK(0x004EBA60, S_004e9090_InitializeMissionScene, "auto");
