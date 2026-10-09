#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, DestroySceneObjectOptionsStorage, 0x004018E0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(uint32_t*, __fastcall, InitializeSceneObjectOptions, 0x004777D0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, sub_48E2B0, 0x0048E2B0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(unsigned int, __fastcall, C_00833908_SetDoorStateFlags, 0x00490D20, (C_00833908* self, void* edx, uint32_t a2, char a3));
CSF_FUNC(uint32_t, __fastcall, SceneMissionSetup_SetObjectMode, 0x00490D70, (void* self, void* edx, int mode));
CSF_FUNC(uint32_t*, __fastcall, sub_4A4280, 0x004A4280, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, sub_4E9090, 0x004E9090, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, sub_4E9B80, 0x004E9B80, (void* self, void* edx, int a2, char a3));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(void, __fastcall, sub_55D860, 0x0055D860, (void* self, void* edx, void* a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// The packet labels this call target as a vtable; the assembly only calls it.
[[maybe_unused]] static const auto kReportedSetupVtable = CSF_VTBL(0x004E9090);

static int __fastcall S_004e9090_ConfigureSceneObject(
    S_004e9090* self, void* /*edx*/,
    C_00833908* object, C_00833908* sourceObject)
{
    void* definitions = reinterpret_cast<void*>(self->field_1fc8);
    auto* definition = reinterpret_cast<std::uint8_t*>(
        reinterpret_cast<void*(__fastcall*)(void*, void*, C_00833908*)>(
            CSF_AT(void**, definitions, 0)[8 / 4])(
                definitions, nullptr, sourceObject));

    void* sceneObject = reinterpret_cast<void*>(
        sub_4E9B80(self, nullptr, reinterpret_cast<int>(sourceObject), 0));
    C_00833908_SetDoorStateFlags(
        object, nullptr, reinterpret_cast<uint32_t>(sceneObject), 1);

    // The initializer writes across these contiguous options members.
    alignas(4) std::uint8_t options[0xB8]{};
    InitializeSceneObjectOptions(options, nullptr);

    CSF_AT(uint32_t, options, 0) |= 1;
    if (CSF_AT(std::uint8_t, definition, 0x159))
        CSF_AT(uint32_t, options, 0) |= 0x400;
    else
        CSF_AT(uint32_t, options, 0) &= 0xFFFFFBFF;

    if (CSF_AT(std::uint8_t, definition, 0x15A))
        CSF_AT(uint32_t, options, 0) &= 0xFFFFFFDF;
    else
        CSF_AT(uint32_t, options, 0) |= 0x20;

    CSF_AT(uint32_t, options, 0x04) = CSF_AT(uint32_t, definition, 0x1F0);
    AssignStringSubstring(options + 0x08, nullptr, definition + 0x1F4, 0, -1);
    AssignStringSubstring(options + 0x24, nullptr, definition + 0x3C, 0, -1);

    CSF_AT(uint32_t, options, 0x58) = CSF_AT(uint32_t, definition, 0x220);
    CSF_AT(uint32_t, options, 0x5C) = CSF_AT(uint32_t, definition, 0x224);
    CSF_AT(uint32_t, options, 0x60) = CSF_AT(uint32_t, definition, 0x228);
    CSF_AT(std::uint8_t, options, 0x66) = CSF_AT(uint32_t, options, 0x04) != 0;
    CSF_AT(std::uint8_t, options, 0x64) = CSF_AT(std::uint8_t, definition, 0x21C);
    CSF_AT(std::uint8_t, options, 0x65) = CSF_AT(std::uint8_t, definition, 0x21D);

    if (CSF_AT(std::uint8_t, definition, 0x15B))
        CSF_AT(uint32_t, options, 0) |= 0x4000;
    else
        CSF_AT(uint32_t, options, 0) &= 0xFFFFBFFF;

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

    sub_4E9090(self, nullptr, CSF_AT(int, definition, 0),
              reinterpret_cast<int>(options));
    reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, sceneObject, 0)[0x10 / 4])(
            sceneObject, nullptr, options);

    if (reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, sceneObject, 0)[0x78 / 4])(sceneObject, nullptr))
    {
        void* relatedObject = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, sceneObject, 0)[0x78 / 4])(sceneObject, nullptr);
        reinterpret_cast<void(__fastcall*)(void*, void*, C_00833908*)>(
            CSF_AT(void**, relatedObject, 0)[0x68 / 4])(
                relatedObject, nullptr, object);
    }

    GetGlobalState_50D190();
    void* eventData = reinterpret_cast<void*>(CSF_ADDR_DATA(0x0083ED08));
    SceneMissionSetup_SetObjectMode(object, nullptr, 4);

    void* storage = Mem_Alloc(124);
    void* context = storage
        ? sub_4A4280(storage, nullptr, reinterpret_cast<int>(object))
        : nullptr;
    sub_55D860(object, nullptr, context);
    sub_48E2B0(object, nullptr, 11, reinterpret_cast<int>(&eventData));

    if (CSF_AT(uint32_t, options, 0x90) >= 0x10 &&
        CSF_AT(uint32_t, options, 0x7C))
    {
        Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(uint32_t, options, 0x7C)));
    }
    CSF_AT(uint32_t, options, 0x90) = 15;
    CSF_AT(uint32_t, options, 0x8C) = 0;
    CSF_AT(std::uint8_t, options, 0x7C) = 0;
    return DestroySceneObjectOptionsStorage(options + 0x04, nullptr);
}

HOOK(0x004EC6C0, S_004e9090_ConfigureSceneObject, "auto");
