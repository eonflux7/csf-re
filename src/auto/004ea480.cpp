#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, DestroySceneObjectOptionsStorage, 0x004018E0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(uint32_t*, __fastcall, InitializeSceneObjectOptions, 0x004777D0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_00833908_ApplySetupMessage, 0x0048E2B0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(unsigned int, __fastcall, C_00833908_SetDoorStateFlags, 0x00490D20, (C_00833908* self, void* edx, uint32_t a2, char a3));
CSF_FUNC(uint32_t, __fastcall, C_00833908_SetSetupMode, 0x00490D70, (C_00833908* self, void* edx, int mode));
CSF_FUNC(int, __fastcall, C_00833908_SetSetupValue, 0x00490E70, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, C_00833908_SetSetupParameters, 0x00490F60, (void* self, void* edx, int a2, float a3, int a4, int a5));
CSF_FUNC(int, __fastcall, C_00833908_FinalizeSetupParameters, 0x00490FE0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, C_00833908_FinalizeSetup, 0x00491050, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_00833908_AssignDefinitionLink, 0x00492B60, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, C_00834598_InitializeForSceneObject, 0x004A5760, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, S_004e9090_ApplyDefinitionOptions, 0x004E9090, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, S_004e9090_CreateSceneObject, 0x004E9B80, (void* self, void* edx, int a2, char a3));
CSF_FUNC(void, __fastcall, C_00833908_SetAssociatedController, 0x0055D860, (void* self, void* edx, void* a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// The packet labels this call target as a vtable, although this struct has none.
[[maybe_unused]] static void** S_004e9090_GetReportedVtableReference()
{
    return CSF_VTBL(0x004E9090);
}

static unsigned int __fastcall S_004e9090_ConfigureMissionSceneObject(
    S_004e9090* self, void* /*edx*/, C_00833908* sceneObject,
    int definitionId, char alternateMode)
{
    void* definitions = reinterpret_cast<void*>(self->field_1fc8);
    void* definition =
        reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, definitions, 0)[8 / 4])(
                definitions, nullptr, definitionId);

    int createdAddress = S_004e9090_CreateSceneObject(
        self, nullptr, definitionId, alternateMode);
    void* createdObject = reinterpret_cast<void*>(createdAddress);
    C_00833908_SetDoorStateFlags(
        sceneObject, nullptr, static_cast<uint32_t>(createdAddress), 1);

    // Contiguous storage includes the flags, option strings, and trailing fields.
    alignas(4) std::uint8_t options[0xB8];
    InitializeSceneObjectOptions(options, nullptr);

    CSF_AT(uint32_t, options, 0) |= 5;
    if (CSF_AT(uint8_t, definition, 0x159))
        CSF_AT(uint32_t, options, 0) |= 0x400;
    else
        CSF_AT(uint32_t, options, 0) &= 0xFFFFFBFF;

    if (CSF_AT(uint8_t, definition, 0x15A))
        CSF_AT(uint32_t, options, 0) &= 0xFFFFFFDF;
    else
        CSF_AT(uint32_t, options, 0) |= 0x20;

    CSF_AT(uint8_t, options, 0x64) = CSF_AT(uint8_t, definition, 0x21C);
    CSF_AT(uint8_t, options, 0x65) = CSF_AT(uint8_t, definition, 0x21D);
    CSF_AT(uint8_t, options, 0x66) = 0;
    CSF_AT(uint32_t, options, 0x04) = CSF_AT(uint32_t, definition, 0x1F0);

    AssignStringSubstring(options + 0x08, nullptr,
        static_cast<uint8_t*>(definition) + 0x1F4, 0, -1);
    AssignStringSubstring(options + 0x24, nullptr,
        static_cast<uint8_t*>(definition) + 0x3C, 0, -1);

    CSF_AT(uint32_t, options, 0x58) = CSF_AT(uint32_t, definition, 0x220);
    CSF_AT(uint32_t, options, 0x5C) = CSF_AT(uint32_t, definition, 0x224);
    CSF_AT(uint32_t, options, 0x60) = CSF_AT(uint32_t, definition, 0x228);

    if (CSF_AT(uint8_t, definition, 0x15B))
        CSF_AT(uint32_t, options, 0) |= 0x4000;
    else
        CSF_AT(uint32_t, options, 0) &= 0xFFFFBFFF;

    CSF_AT(uint32_t, options, 0x68) = CSF_AT(uint32_t, definition, 0xCC);
    CSF_AT(uint32_t, options, 0x6C) = CSF_AT(uint32_t, definition, 0x160);
    CSF_AT(uint32_t, options, 0x70) = CSF_AT(uint32_t, definition, 0x164);
    CSF_AT(uint32_t, options, 0x74) = CSF_AT(uint32_t, definition, 0x168);

    AssignStringSubstring(options + 0x78, nullptr,
        static_cast<uint8_t*>(definition) + 0x16C, 0, -1);

    CSF_AT(uint32_t, options, 0x94) =
        CSF_AT(uint32_t, definition, 0x188) - 1;
    CSF_AT(uint32_t, options, 0x98) = CSF_AT(uint32_t, definition, 0x18C);
    CSF_AT(uint32_t, options, 0x9C) = CSF_AT(uint32_t, definition, 0x190);
    CSF_AT(uint32_t, options, 0xA0) = CSF_AT(uint32_t, definition, 0x194);
    CSF_AT(uint32_t, options, 0xA4) = CSF_AT(uint32_t, definition, 0xD0);

    S_004e9090_ApplyDefinitionOptions(
        self, nullptr, CSF_AT(int, definition, 0),
        reinterpret_cast<int>(options));

    reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, createdObject, 0)[0x10 / 4])(
            createdObject, nullptr, options);

    if (reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, createdObject, 0)[0x78 / 4])(
                createdObject, nullptr))
    {
        void* relatedObject =
            reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, createdObject, 0)[0x78 / 4])(
                    createdObject, nullptr);
        reinterpret_cast<void(__fastcall*)(void*, void*, C_00833908*)>(
            CSF_AT(void**, relatedObject, 0)[0x68 / 4])(
                relatedObject, nullptr, sceneObject);
    }

    sceneObject->field_0078 = CSF_AT(int, definition, 0xB4) == 1;
    sceneObject->field_007c = CSF_AT(uint32_t, definition, 0xBC);

    if (alternateMode)
    {
        C_00833908_SetSetupMode(sceneObject, nullptr, 3);
        C_00833908_SetSetupValue(
            sceneObject, nullptr, 4, CSF_AT(int, definition, 0xDC));
    }
    else
    {
        C_00833908_SetSetupMode(sceneObject, nullptr, 1);
        C_00833908_SetSetupValue(
            sceneObject, nullptr, 0, CSF_AT(int, definition, 0xDC));
        C_00833908_SetSetupParameters(
            sceneObject, nullptr, 0, CSF_AT(float, definition, 0x120),
            CSF_AT(int, definition, 0x124), CSF_AT(int, definition, 0x128));
        C_00833908_FinalizeSetupParameters(sceneObject, nullptr, 0);
    }

    C_00833908_FinalizeSetup(sceneObject, nullptr, 0);

    void* controllerStorage = Mem_Alloc(172);
    void* controller = nullptr;
    if (controllerStorage)
    {
        controller = reinterpret_cast<void*>(
            C_00834598_InitializeForSceneObject(
                controllerStorage, nullptr, reinterpret_cast<int>(sceneObject)));
    }
    C_00833908_SetAssociatedController(sceneObject, nullptr, controller);

    uint32_t setupMessage[3];
    setupMessage[0] = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x0083ED0C));
    setupMessage[1] = 0;
    setupMessage[2] = reinterpret_cast<uint32_t>(self);
    C_00833908_ApplySetupMessage(
        sceneObject, nullptr, 0, reinterpret_cast<int>(setupMessage));

    int linkId = CSF_AT(int, definition, 0xC0);
    if (linkId != -1 && CSF_AT(int, definition, 0xB4) == 1)
    {
        definitions = reinterpret_cast<void*>(self->field_1fc8);
        int lookupArgument = CSF_AT(int, definition, 0x12C);
        int linkedDefinition =
            reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, definitions, 0)[0x10 / 4])(
                    definitions, nullptr, lookupArgument);
        C_00833908_AssignDefinitionLink(
            sceneObject, nullptr, linkedDefinition, linkId);
    }

    auto* optionString = reinterpret_cast<C_0082d410*>(options + 0x78);
    if (CSF_AT(uint32_t, options, 0x90) >= 0x10 &&
        optionString->field_0004)
    {
        Mem_FreeDebug(reinterpret_cast<void*>(optionString->field_0004));
    }
    CSF_AT(uint32_t, options, 0x90) = 15;
    CSF_AT(uint32_t, options, 0x8C) = 0;
    reinterpret_cast<uint8_t*>(&optionString->field_0004)[0] = 0;

    return DestroySceneObjectOptionsStorage(options + 0x04, nullptr);
}

HOOK(0x004EA480, S_004e9090_ConfigureMissionSceneObject, "auto");
