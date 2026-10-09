#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, DestroySceneObjectOptionsStorage, 0x004018E0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(uint32_t*, __fastcall, InitializeSceneObjectOptions, 0x004777D0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_00833908_SendSetupEvent, 0x0048E2B0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(unsigned int, __fastcall, C_00833908_SetDoorStateFlags, 0x00490D20, (C_00833908* self, void* edx, uint32_t a2, char a3));
CSF_FUNC(uint32_t, __fastcall, C_00833908_SetSetupMode, 0x00490D70, (void* self, void* edx, int mode));
CSF_FUNC(int, __fastcall, C_00833908_CreateSceneComponent, 0x00490EE0, (void* self, void* edx, int a2, int a3, int a4, int a5, int a6));
CSF_FUNC(int, __fastcall, C_00833908_SetSetupPropertyA, 0x0049EBA0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_00833908_SetSetupPropertyB, 0x0049EBC0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_00833908_SetSetupPropertyC, 0x0049EBD0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_00833908_SetSetupPropertyD, 0x0049EBE0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, C_00834790_ConstructForSceneObject, 0x004A6C80, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, MissionSetup_WriteScaledVectorFromLookup, 0x004E9090, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, MissionSetup_CreateSceneObject, 0x004E9B80, (void* self, void* edx, int a2, char a3));
CSF_FUNC(void, __fastcall, C_00833908_SetOwnedContext, 0x0055D860, (void* self, void* edx, void* a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

static unsigned int __fastcall S_004e9090_ConfigureMissionSceneObject(
    S_004e9090* self, void* /*edx*/, C_00833908* object, int definitionId)
{
    void* definitions = reinterpret_cast<void*>(self->field_1fc8);
    auto* definition =
        ((std::uint8_t* (__fastcall*)(void*, void*, int))
            CSF_AT(void**, definitions, 0)[8 / 4])(
                definitions, nullptr, definitionId);

    int sceneObjectAddress =
        MissionSetup_CreateSceneObject(self, nullptr, definitionId, 0);
    C_00833908_SetDoorStateFlags(object, nullptr, sceneObjectAddress, 1);

    // Initialization and cleanup operate on the entire contiguous options object.
    alignas(4) std::uint8_t options[0xB8] = {};
    InitializeSceneObjectOptions(options, nullptr);

    CSF_AT(uint32_t, options, 0x00) |= 0x40002;
    if (CSF_AT(uint8_t, definition, 0x159))
        CSF_AT(uint32_t, options, 0x00) |= 0x400;
    else
        CSF_AT(uint32_t, options, 0x00) &= 0xFFFFFBFF;

    if (CSF_AT(uint8_t, definition, 0x15A))
        CSF_AT(uint32_t, options, 0x00) &= 0xFFFFFFDF;
    else
        CSF_AT(uint32_t, options, 0x00) |= 0x20;

    CSF_AT(int, options, 0x04) = CSF_AT(int, definition, 0x1F0);
    AssignStringSubstring(options + 0x08, nullptr, definition + 0x1F4, 0, -1);
    AssignStringSubstring(options + 0x24, nullptr, definition + 0x3C, 0, -1);

    CSF_AT(uint32_t, options, 0x58) = CSF_AT(uint32_t, definition, 0x220);
    CSF_AT(uint32_t, options, 0x5C) = CSF_AT(uint32_t, definition, 0x224);
    CSF_AT(uint32_t, options, 0x60) = CSF_AT(uint32_t, definition, 0x228);
    CSF_AT(bool, options, 0x66) = CSF_AT(int, options, 0x04) != 0;
    CSF_AT(uint8_t, options, 0x64) = CSF_AT(uint8_t, definition, 0x21C);
    CSF_AT(uint8_t, options, 0x65) = CSF_AT(uint8_t, definition, 0x21D);

    if (CSF_AT(uint8_t, definition, 0x15B))
        CSF_AT(uint32_t, options, 0x00) |= 0x4000;
    else
        CSF_AT(uint32_t, options, 0x00) &= 0xFFFFBFFF;

    CSF_AT(uint32_t, options, 0x68) = CSF_AT(uint32_t, definition, 0xCC);
    CSF_AT(uint32_t, options, 0x6C) = CSF_AT(uint32_t, definition, 0x160);
    CSF_AT(uint32_t, options, 0x70) = CSF_AT(uint32_t, definition, 0x164);
    CSF_AT(uint32_t, options, 0x74) = CSF_AT(uint32_t, definition, 0x168);
    AssignStringSubstring(options + 0x78, nullptr, definition + 0x16C, 0, -1);

    CSF_AT(uint32_t, options, 0x94) = CSF_AT(uint32_t, definition, 0x188) - 1;
    CSF_AT(uint32_t, options, 0x98) = CSF_AT(uint32_t, definition, 0x18C);
    CSF_AT(uint32_t, options, 0x9C) = CSF_AT(uint32_t, definition, 0x190);
    CSF_AT(uint32_t, options, 0xA0) = CSF_AT(uint32_t, definition, 0x194);
    CSF_AT(uint32_t, options, 0xA4) = CSF_AT(uint32_t, definition, 0xD0);

    // The packet's CSF_VTBL(0x004E9090) annotation is a direct call,
    // not an actual vtable reference.
    MissionSetup_WriteScaledVectorFromLookup(
        self, nullptr, CSF_AT(int, definition, 0x00),
        reinterpret_cast<int>(options));

    void* sceneObject = reinterpret_cast<void*>(sceneObjectAddress);
    ((void (__fastcall*)(void*, void*, void*))
        CSF_AT(void**, sceneObject, 0)[0x10 / 4])(
            sceneObject, nullptr, options);

    object->field_0078 = 1;
    C_00833908_SetSetupPropertyA(object, nullptr, CSF_AT(int, definition, 0x1B4));
    C_00833908_SetSetupPropertyB(object, nullptr, CSF_AT(int, definition, 0x1B8));
    C_00833908_SetSetupPropertyC(object, nullptr, CSF_AT(int, definition, 0x1BC));
    C_00833908_SetSetupPropertyD(object, nullptr, CSF_AT(int, definition, 0x1C0));
    C_00833908_SetSetupMode(object, nullptr, 2);
    C_00833908_CreateSceneComponent(
        object, nullptr, 3,
        CSF_AT(int, definition, 0x1C4),
        CSF_AT(int, definition, 0x1C8),
        CSF_AT(int, definition, 0x1CC),
        CSF_AT(int, definition, 0x1D0));

    void* storage = Mem_Alloc(124);
    void* context = storage
        ? C_00834790_ConstructForSceneObject(storage, nullptr, reinterpret_cast<int>(object))
        : nullptr;
    C_00833908_SetOwnedContext(object, nullptr, context);

    void* eventData = reinterpret_cast<void*>(CSF_ADDR_DATA(0x0083ED08));
    C_00833908_SendSetupEvent(object, nullptr, 9, reinterpret_cast<int>(&eventData));

    if (CSF_AT(uint32_t, options, 0x90) >= 0x10 &&
        CSF_AT(uint32_t, options, 0x7C))
    {
        Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(uint32_t, options, 0x7C)));
    }
    CSF_AT(uint32_t, options, 0x90) = 15;
    CSF_AT(uint32_t, options, 0x8C) = 0;
    CSF_AT(uint8_t, options, 0x7C) = 0;
    return DestroySceneObjectOptionsStorage(options + 0x04, nullptr);
}

HOOK(0x004EC240, S_004e9090_ConfigureMissionSceneObject, "auto");
