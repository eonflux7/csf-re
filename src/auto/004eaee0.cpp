#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(unsigned int, __fastcall, FinalizeSceneObjectOptions, 0x004772B0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, InitializeSceneObjectOptions, 0x004777D0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_00833908_AttachSceneComponent, 0x0048E2B0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(unsigned int, __fastcall, C_00833908_SetDoorStateFlags, 0x00490D20, (C_00833908* self, void* edx, uint32_t a2, char a3));
CSF_FUNC(int, __fastcall, GetSceneRegionAt, 0x004FC130, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneMissionSetup_SetBoxBounds, 0x006AB4E0, (void* self, void* edx, int a2, int a3, int a4, int a5, int a6, int a7));
CSF_FUNC(float*, __fastcall, SceneMissionSetup_SetBoxOrigin, 0x006AB5C0, (void* self, void* edx, float* a2));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_ResolveRegionIndex, 0x00774C30, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(int, g_Value8B0554, 0x008B0554);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

// Retain the packet's binding annotation for the array-growth entry.
[[maybe_unused]] static const auto kButtonArrayBinding = CSF_VTBL(0x0041E5F0);

static int SceneMissionSetup_FloatArgument(float value)
{
    int argument;
    std::memcpy(&argument, &value, sizeof(argument));
    return argument;
}

static char __fastcall S_004e9090_ConfigureRegionSceneObject(
    S_004e9090* self, void* /*edx*/, C_00833908* object,
    int definitionIndex, int regionIndex, int objectType)
{
    void* definitions = reinterpret_cast<void*>(self->field_1fc8);
    void* definition = reinterpret_cast<void*>(
        ((int(__fastcall*)(void*, void*, int))
            CSF_AT(void**, definitions, 0)[8 / 4])(
                definitions, nullptr, definitionIndex));

    int category;
    if (objectType == 26)
        category = 1;
    else if (objectType == 27)
        category = 2;
    else
        return 0;

    int resolvedIndex = SceneLoader_ResolveRegionIndex(
        reinterpret_cast<void*>(g_Value8B0554), nullptr, regionIndex);
    auto* region = reinterpret_cast<std::uint8_t*>(
        GetSceneRegionAt(reinterpret_cast<void*>(g_Value8B0554),
                         nullptr, resolvedIndex));
    int globalContext = GetGlobalInt();
    void* controller = reinterpret_cast<void*>(
        SceneLoader_GetSceneController(
            reinterpret_cast<void*>(globalContext), nullptr));

    void* regionName;
    if (CSF_AT(std::uint32_t, region, 0x20) < 0x10u)
        regionName = region + 0x0C;
    else
        regionName = CSF_AT(void*, region, 0x0C);

    int sceneObjectIndex =
        ((int(__fastcall*)(void*, void*, int, void*))
            CSF_AT(void**, controller, 0)[80 / 4])(
                controller, nullptr, category, regionName);

    if (sceneObjectIndex == -1)
    {
        C_00833908_SetDoorStateFlags(object, nullptr, 0, 0);
        return 0;
    }

    int sceneObject =
        ((int(__fastcall*)(void*, void*, int, int))
            CSF_AT(void**, controller, 0)[76 / 4])(
                controller, nullptr, category, sceneObjectIndex);
    C_00833908_SetDoorStateFlags(object, nullptr, sceneObject, 0);
    if (!sceneObject)
        return 0;

    // One complete options object, including both strings and the box member.
    alignas(4) std::uint8_t options[0xB8] = {};
    InitializeSceneObjectOptions(options, nullptr);

    std::uint32_t flags = CSF_AT(std::uint32_t, options, 0);
    if (CSF_AT(std::uint8_t, definition, 0x159))
        flags |= 0x40400;
    else
        flags = (flags & 0xFFFBFBFF) | 0x40000;
    if (CSF_AT(std::uint8_t, definition, 0x15A))
        flags &= 0xFFFFFFDF;
    else
        flags |= 0x20;
    if (CSF_AT(std::uint8_t, definition, 0x15B))
        flags |= 0x4000;
    else
        flags &= 0xFFFFBFFF;
    CSF_AT(std::uint32_t, options, 0) = flags;

    auto* definitionBytes = reinterpret_cast<std::uint8_t*>(definition);
    if (category == 2)
    {
        CSF_AT(int, options, 0x04) = CSF_AT(int, definition, 0x1F0);
        CSF_AT(std::uint32_t, options, 0) = flags & 0xFFFFFFBF;
        AssignStringSubstring(options + 0x08, nullptr,
                              definitionBytes + 0x1F4, 0, -1);
        AssignStringSubstring(options + 0x24, nullptr,
                              definitionBytes + 0x3C, 0, -1);

        int parameterZ = CSF_AT(int, definition, 0x228);
        int parameterX = CSF_AT(int, definition, 0x220);
        int parameterY = CSF_AT(int, definition, 0x224);
        CSF_AT(int, options, 0x60) = parameterZ;
        bool enabled = CSF_AT(int, options, 0x04) != 0;
        std::uint8_t secondaryFlag = CSF_AT(std::uint8_t, definition, 0x21D);
        CSF_AT(int, options, 0x58) = parameterX;
        CSF_AT(int, options, 0x5C) = parameterY;
        std::uint8_t primaryFlag = CSF_AT(std::uint8_t, definition, 0x21C);
        CSF_AT(bool, options, 0x66) = enabled;
        CSF_AT(std::uint8_t, options, 0x64) = primaryFlag;
        CSF_AT(std::uint8_t, options, 0x65) = secondaryFlag;
    }
    else
    {
        float extentX = CSF_AT(float, region, 0x44);
        float extentY = CSF_AT(float, region, 0x48);
        float extentZ = CSF_AT(float, region, 0x4C);
        float origin[3];
        origin[0] = CSF_AT(float, region, 0x38);
        origin[1] = CSF_AT(float, region, 0x3C);
        origin[2] = CSF_AT(float, region, 0x40);

        void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
        CSF_AT(int, options, 0x04) =
            CSF_AT(std::uint8_t, globalState, 0xD0) ? 0 : 2;
        SceneMissionSetup_SetBoxBounds(
            options + 0x40, nullptr,
            SceneMissionSetup_FloatArgument(extentX),
            SceneMissionSetup_FloatArgument(-extentX),
            SceneMissionSetup_FloatArgument(extentY),
            SceneMissionSetup_FloatArgument(-extentY),
            SceneMissionSetup_FloatArgument(extentZ),
            SceneMissionSetup_FloatArgument(-extentZ));
        SceneMissionSetup_SetBoxOrigin(options + 0x40, nullptr, origin);
        AssignStringSubstring(options + 0x24, nullptr,
                              definitionBytes + 0x3C, 0, -1);
        CSF_AT(float, options, 0x60) = 1.0f;
        CSF_AT(float, options, 0x5C) = 0.5f;
        CSF_AT(bool, options, 0x66) = true;
        CSF_AT(std::uint8_t, options, 0x64) = 0;
        CSF_AT(std::uint8_t, options, 0x65) = 1;
    }

    float sizeX = CSF_AT(float, region, 0x44);
    CSF_AT(int, options, 0x68) = 0;
    CSF_AT(int, options, 0x94) = 2;
    CSF_AT(int, options, 0xA4) = 1;
    sizeX += sizeX;
    CSF_AT(float, options, 0x6C) = sizeX;
    float sizeY = CSF_AT(float, region, 0x48);
    sizeY += sizeY;
    CSF_AT(float, options, 0x70) = sizeY;
    float sizeZ = CSF_AT(float, region, 0x4C);
    sizeZ += sizeZ;
    CSF_AT(float, options, 0x74) = sizeZ;

    float positionX;
    float positionZ;
    if (category == 1)
    {
        positionX = CSF_AT(float, region, 0x38);
        positionZ = CSF_AT(float, region, 0x40);
        CSF_AT(float, options, 0x9C) = 0.0f;
    }
    else
    {
        positionX = g_ZeroVectorX;
        positionZ = g_ZeroVectorZ;
        CSF_AT(float, options, 0x9C) = g_ZeroVectorY;
    }
    CSF_AT(float, options, 0x98) = positionX;
    CSF_AT(float, options, 0xA0) = positionZ;

    void* configuredObject = reinterpret_cast<void*>(sceneObject);
    ((void(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, configuredObject, 0)[16 / 4])(
            configuredObject, nullptr, options);
    FinalizeSceneObjectOptions(options, nullptr);
    object->field_0078 = 2;

    alignas(4) std::uint8_t sizeVector[0x10] = {};
    CSF_AT(void*, sizeVector, 0) =
        reinterpret_cast<void*>(CSF_ADDR_DATA(0x0083ED08));
    CSF_AT(float, sizeVector, 0x04) = sizeX;
    CSF_AT(float, sizeVector, 0x08) = sizeY;
    CSF_AT(float, sizeVector, 0x0C) = sizeZ;

    if (category == 1)
    {
        C_00833908_AttachSceneComponent(
            object, nullptr, 14, reinterpret_cast<int>(sizeVector));
        auto* array = reinterpret_cast<std::uint8_t*>(g_pCurrentScene) + 136;
        if (CSF_AT(std::uint32_t, g_pCurrentScene, 0x90) >=
            CSF_AT(std::uint32_t, g_pCurrentScene, 0x8C))
        {
            GrowButtonArray(
                reinterpret_cast<std::uint8_t*>(g_pCurrentScene) + 136,
                nullptr, CSF_AT(int, g_pCurrentScene, 0x94));
        }
        int count = CSF_AT(int, array, 0x08);
        auto* destination = CSF_AT(C_00833908**, array, 0) + count;
        CSF_AT(int, array, 0x08) = count + 1;
        *destination = object;
    }
    else
    {
        C_00833908_AttachSceneComponent(
            object, nullptr, 11, reinterpret_cast<int>(sizeVector));
    }
    return 1;
}

HOOK(0x004EAEE0, S_004e9090_ConfigureRegionSceneObject, "auto");
