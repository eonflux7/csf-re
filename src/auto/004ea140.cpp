#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, DestroySceneObjectOptionsStorage, 0x004018E0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(uint32_t*, __fastcall, InitializeSceneObjectOptions, 0x004777D0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, SceneMissionSetup_SetObjectNotification, 0x0048E2B0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(unsigned int, __fastcall, C_00833908_SetDoorStateFlags, 0x00490D20, (C_00833908* self, void* edx, uint32_t a2, char a3));
CSF_FUNC(uint32_t, __fastcall, C_00833908_SelectState, 0x00490D70, (C_00833908* self, void* edx, int state));
CSF_FUNC(int, __fastcall, SceneMissionSetup_SetObjectStateValue, 0x00490E70, (void* self, void* edx, int a2, int a3));
CSF_FUNC(void, __fastcall, SceneMissionSetup_ResolveObjectOptions, 0x004E9090, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, SceneMissionSetup_CreateSceneObject, 0x004E9B80, (void* self, void* edx, int a2, char a3));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static unsigned int __fastcall S_004e9090_ConfigureMissionObject(
    S_004e9090* self, void* /*edx*/, C_00833908* object, int definitionIndex)
{
    void* definitions = reinterpret_cast<void*>(self->field_1fc8);
    void* definition =
        reinterpret_cast<void* (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, definitions, 0)[8 / 4])(
                definitions, nullptr, definitionIndex);

    int sceneObjectAddress =
        SceneMissionSetup_CreateSceneObject(self, nullptr, definitionIndex, 0);
    C_00833908_SetDoorStateFlags(object, nullptr, sceneObjectAddress, 1);

    alignas(4) std::uint8_t options[0xB8]{};
    InitializeSceneObjectOptions(options, nullptr);

    auto& flags = CSF_AT(uint32_t, options, 0);
    flags |= 5;
    if (CSF_AT(std::uint8_t, definition, 0x159))
        flags |= 0x400;
    else
        flags &= 0xFFFFFBFF;

    if (CSF_AT(std::uint8_t, definition, 0x15A))
        flags &= 0xFFFFFFDF;
    else
        flags |= 0x20;

    if (CSF_AT(std::uint8_t, definition, 0x15B))
        flags |= 0x4000;
    else
        flags &= 0xFFFFBFFF;

    CSF_AT(uint32_t, options, 0x68) = CSF_AT(uint32_t, definition, 0xCC);
    CSF_AT(uint32_t, options, 0x6C) = CSF_AT(uint32_t, definition, 0x160);
    CSF_AT(uint32_t, options, 0x70) = CSF_AT(uint32_t, definition, 0x164);
    CSF_AT(uint32_t, options, 0x74) = CSF_AT(uint32_t, definition, 0x168);

    auto* name = reinterpret_cast<C_0082d410*>(options + 0x78);
    AssignStringSubstring(
        name, nullptr, static_cast<std::uint8_t*>(definition) + 0x16C,
        0, static_cast<std::uint32_t>(-1));

    CSF_AT(uint32_t, options, 0x94) =
        CSF_AT(uint32_t, definition, 0x188) - 1;
    CSF_AT(uint32_t, options, 0x98) = CSF_AT(uint32_t, definition, 0x18C);
    CSF_AT(uint32_t, options, 0x9C) = CSF_AT(uint32_t, definition, 0x190);
    CSF_AT(uint32_t, options, 0xA0) = CSF_AT(uint32_t, definition, 0x194);
    CSF_AT(uint32_t, options, 0xA4) = CSF_AT(uint32_t, definition, 0xD0);

    SceneMissionSetup_ResolveObjectOptions(
        self, nullptr, CSF_AT(int, definition, 0),
        reinterpret_cast<int>(options));

    void* sceneObject = reinterpret_cast<void*>(sceneObjectAddress);
    reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, sceneObject, 0)[0x10 / 4])(
            sceneObject, nullptr, options);

    object->field_0078 = -1;
    C_00833908_SelectState(object, nullptr, 5);
    SceneMissionSetup_SetObjectStateValue(
        object, nullptr, 5, CSF_AT(int, definition, 0xDC));

    uint32_t notification[2];
    notification[0] = reinterpret_cast<uint32_t>(CSF_VTBL(0x0083ED0C));
    notification[1] = 0;
    SceneMissionSetup_SetObjectNotification(
        object, nullptr, 0, reinterpret_cast<int>(notification));

    if (CSF_AT(uint32_t, options, 0x90) >= 0x10 && name->field_0004)
        Mem_FreeDebug(reinterpret_cast<void*>(name->field_0004));

    CSF_AT(uint32_t, options, 0x90) = 15;
    CSF_AT(uint32_t, options, 0x8C) = 0;
    name->field_0004 &= 0xFFFFFF00;

    return DestroySceneObjectOptionsStorage(options + 4, nullptr);
}

HOOK(0x004EA140, S_004e9090_ConfigureMissionObject, "auto");
