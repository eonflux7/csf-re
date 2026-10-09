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
CSF_FUNC(uint32_t, __fastcall, C_00833908_SetSetupMode, 0x00490D70, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_00833908_SetSetupParameters, 0x00490EE0, (void* self, void* edx, int a2, int a3, int a4, int a5, int a6));
CSF_FUNC(int, __fastcall, C_00833908_SetSetupFloatParameters, 0x00490F60, (void* self, void* edx, int a2, float a3, int a4, int a5));
CSF_FUNC(uint32_t*, __fastcall, C_00833908_EnableSetup, 0x00491050, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_00833908_SetSetupPropertyA, 0x0049EBA0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_00833908_SetSetupPropertyB, 0x0049EBC0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_00833908_SetSetupPropertyC, 0x0049EBD0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_00833908_SetSetupPropertyD, 0x0049EBE0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, C_00834790_ConstructForSceneObject, 0x004A6C80, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, SceneMissionSetup_ResolveObjectOptions, 0x004E9090, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, SceneMissionSetup_GetSceneObject, 0x004E9B80, (void* self, void* edx, int a2, char a3));
CSF_FUNC(void, __fastcall, C_00833908_SetAttachedContext, 0x0055D860, (void* self, void* edx, void* a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

static unsigned int __fastcall S_004e9090_ConfigureSceneObject(
    S_004e9090* self, void* /*edx*/, C_00833908* object, int objectId)
{
    void* definitions = reinterpret_cast<void*>(self->field_1fc8);
    void* definition =
        reinterpret_cast<void* (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, definitions, 0)[2])(definitions, nullptr, objectId);

    int sceneObjectValue =
        SceneMissionSetup_GetSceneObject(self, nullptr, objectId, 0);
    void* sceneObject = reinterpret_cast<void*>(sceneObjectValue);
    C_00833908_SetDoorStateFlags(object, nullptr, sceneObjectValue, 1);

    // These fields and strings are one contiguous options object.
    alignas(4) std::uint8_t options[0xB8]{};
    InitializeSceneObjectOptions(options, nullptr);

    CSF_AT(uint32_t, options, 0) |= 0x40002;
    if (CSF_AT(std::uint8_t, definition, 0x159))
        CSF_AT(uint32_t, options, 0) |= 0x400;
    else
        CSF_AT(uint32_t, options, 0) &= 0xFFFFFBFF;

    if (CSF_AT(std::uint8_t, definition, 0x15A))
        CSF_AT(uint32_t, options, 0) &= 0xFFFFFFDF;
    else
        CSF_AT(uint32_t, options, 0) |= 0x20;

    if (CSF_AT(std::uint8_t, definition, 0x15B))
        CSF_AT(uint32_t, options, 0) |= 0x4000;
    else
        CSF_AT(uint32_t, options, 0) &= 0xFFFFBFFF;

    CSF_AT(uint32_t, options, 0x04) = CSF_AT(uint32_t, definition, 0x1F0);
    AssignStringSubstring(options + 0x08, nullptr,
                          static_cast<std::uint8_t*>(definition) + 0x1F4,
                          0, static_cast<std::uint32_t>(-1));
    AssignStringSubstring(options + 0x24, nullptr,
                          static_cast<std::uint8_t*>(definition) + 0x3C,
                          0, static_cast<std::uint32_t>(-1));

    CSF_AT(uint32_t, options, 0x58) = CSF_AT(uint32_t, definition, 0x220);
    CSF_AT(uint32_t, options, 0x5C) = CSF_AT(uint32_t, definition, 0x224);
    CSF_AT(uint32_t, options, 0x60) = CSF_AT(uint32_t, definition, 0x228);
    CSF_AT(bool, options, 0x66) = CSF_AT(uint32_t, options, 0x04) != 0;
    CSF_AT(std::uint8_t, options, 0x64) = CSF_AT(std::uint8_t, definition, 0x21C);
    CSF_AT(std::uint8_t, options, 0x65) = CSF_AT(std::uint8_t, definition, 0x21D);
    CSF_AT(uint32_t, options, 0x68) = CSF_AT(uint32_t, definition, 0xCC);
    CSF_AT(uint32_t, options, 0x6C) = CSF_AT(uint32_t, definition, 0x160);
    CSF_AT(uint32_t, options, 0x70) = CSF_AT(uint32_t, definition, 0x164);
    CSF_AT(uint32_t, options, 0x74) = CSF_AT(uint32_t, definition, 0x168);
    AssignStringSubstring(options + 0x78, nullptr,
                          static_cast<std::uint8_t*>(definition) + 0x16C,
                          0, static_cast<std::uint32_t>(-1));

    CSF_AT(uint32_t, options, 0x94) = CSF_AT(uint32_t, definition, 0x188) - 1;
    CSF_AT(uint32_t, options, 0x98) = CSF_AT(uint32_t, definition, 0x18C);
    CSF_AT(uint32_t, options, 0x9C) = CSF_AT(uint32_t, definition, 0x190);
    CSF_AT(uint32_t, options, 0xA0) = CSF_AT(uint32_t, definition, 0x194);
    CSF_AT(uint32_t, options, 0xA4) = CSF_AT(uint32_t, definition, 0xD0);

    // The packet also labels this direct-call address as a vtable reference.
    (void)CSF_VTBL(0x004E9090);
    SceneMissionSetup_ResolveObjectOptions(
        self, nullptr, CSF_AT(int, definition, 0),
        reinterpret_cast<int>(options));

    reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, sceneObject, 0)[4])(sceneObject, nullptr, options);

    object->field_0078 = 1;
    C_00833908_SetSetupPropertyA(object, nullptr, CSF_AT(int, definition, 0x1B4));
    C_00833908_SetSetupPropertyB(object, nullptr, CSF_AT(int, definition, 0x1B8));
    C_00833908_SetSetupPropertyC(object, nullptr, CSF_AT(int, definition, 0x1BC));
    C_00833908_SetSetupPropertyD(object, nullptr, CSF_AT(int, definition, 0x1C0));
    C_00833908_SetSetupMode(object, nullptr, 2);
    C_00833908_SetSetupParameters(
        object, nullptr, 3,
        CSF_AT(int, definition, 0x1C4), CSF_AT(int, definition, 0x1C8),
        CSF_AT(int, definition, 0x1CC), CSF_AT(int, definition, 0x1D0));
    C_00833908_SetSetupFloatParameters(
        object, nullptr, 1, CSF_AT(float, definition, 0x120),
        CSF_AT(int, definition, 0x124), CSF_AT(int, definition, 0x128));

    void* storage = Mem_Alloc(124);
    void* context = nullptr;
    if (storage)
        context = C_00834790_ConstructForSceneObject(
            storage, nullptr, reinterpret_cast<int>(object));
    C_00833908_SetAttachedContext(object, nullptr, context);
    C_00833908_EnableSetup(object, nullptr, 1);

    std::uintptr_t eventDescriptor = CSF_ADDR_DATA(0x0083ED08);
    C_00833908_SendSetupEvent(
        object, nullptr, 7, reinterpret_cast<int>(&eventDescriptor));

    if (CSF_AT(uint32_t, options, 0x90) >= 0x10 &&
        CSF_AT(uint32_t, options, 0x7C) != 0)
    {
        Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(uint32_t, options, 0x7C)));
    }
    CSF_AT(uint32_t, options, 0x90) = 15;
    CSF_AT(uint32_t, options, 0x8C) = 0;
    CSF_AT(std::uint8_t, options, 0x7C) = 0;
    return DestroySceneObjectOptionsStorage(options + 0x04, nullptr);
}

HOOK(0x004EB250, S_004e9090_ConfigureSceneObject, "auto");
