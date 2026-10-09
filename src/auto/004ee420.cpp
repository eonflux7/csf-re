#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, DestroySceneObjectOptionsStorage, 0x004018E0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(uint32_t*, __fastcall, InitializeSceneObjectOptions, 0x004777D0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_00833908_SetResourceProperty, 0x0048E2B0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(unsigned int, __fastcall, C_00833908_SetDoorStateFlags, 0x00490D20, (C_00833908* self, void* edx, uint32_t a2, char a3));
CSF_FUNC(int, __fastcall, C_00833908_SetSceneState, 0x00490E70, (void* self, void* edx, int a2, int a3));
CSF_FUNC(uint32_t*, __fastcall, C_008344f0_ConstructForSceneObject, 0x004A4280, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SceneMissionSetup_FindSceneObject, 0x004E9B80, (void* self, void* edx, int a2, char a3));
CSF_FUNC(void, __fastcall, C_00833908_SetOwnedContext, 0x0055D860, (void* self, void* edx, void* a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(char, __cdecl, NormalizeResourceName, 0x006949D0, (char* source, char* destination, unsigned int capacity));

static uint32_t* __fastcall S_004e9090_SetupDoorSceneObject(
    S_004e9090* self, void* /*edx*/, C_00833908* sceneObject, int objectId)
{
    void* definitions = reinterpret_cast<void*>(self->field_1fc8);
    void* definition =
        ((void* (__fastcall*)(void*, void*, int))
            CSF_AT(void**, definitions, 0)[8 / 4])(
                definitions, nullptr, objectId);

    int objectAddress =
        SceneMissionSetup_FindSceneObject(self, nullptr, objectId, 0);

    if (objectAddress)
    {
        C_00833908_SetDoorStateFlags(sceneObject, nullptr, objectAddress, 1);

        // Flags, storage, string, and trailing properties form one options object.
        alignas(4) std::uint8_t options[0xB8] = {};
        InitializeSceneObjectOptions(options, nullptr);

        uint32_t flags = CSF_AT(uint32_t, options, 0);
        if (CSF_AT(std::uint8_t, definition, 0x159))
            flags |= 0x40C00;
        else
            flags = (flags & 0xFFFBF3FF) | 0x40800;

        if (CSF_AT(std::uint8_t, definition, 0x15A))
            flags &= 0xFFFFFFDF;
        else
            flags |= 0x20;

        if (CSF_AT(std::uint8_t, definition, 0x15B))
            flags |= 0x4000;
        else
            flags &= 0xFFFFBFFF;

        CSF_AT(uint32_t, options, 0) = flags;
        CSF_AT(uint32_t, options, 0x68) = CSF_AT(uint32_t, definition, 0xCC);
        CSF_AT(uint32_t, options, 0x6C) = CSF_AT(uint32_t, definition, 0x160);
        CSF_AT(uint32_t, options, 0x70) = CSF_AT(uint32_t, definition, 0x164);
        CSF_AT(uint32_t, options, 0x74) = CSF_AT(uint32_t, definition, 0x168);

        auto* optionString = reinterpret_cast<C_0082d410*>(options + 0x78);
        AssignStringSubstring(
            optionString, nullptr,
            static_cast<std::uint8_t*>(definition) + 0x16C, 0, -1);

        CSF_AT(uint32_t, options, 0x94) =
            CSF_AT(uint32_t, definition, 0x188) - 1;
        CSF_AT(uint32_t, options, 0x98) = CSF_AT(uint32_t, definition, 0x18C);
        CSF_AT(uint32_t, options, 0x9C) = CSF_AT(uint32_t, definition, 0x190);
        CSF_AT(uint32_t, options, 0xA0) = CSF_AT(uint32_t, definition, 0x194);
        CSF_AT(uint32_t, options, 0xA4) = CSF_AT(uint32_t, definition, 0xD0);

        void* door = reinterpret_cast<void*>(objectAddress);
        ((void (__fastcall*)(void*, void*, uint32_t*))
            CSF_AT(void**, door, 0)[0x10 / 4])(
                door, nullptr, reinterpret_cast<uint32_t*>(options));

        void* allocation = Mem_Alloc(124);
        uint32_t* context = nullptr;
        if (allocation)
            context = C_008344f0_ConstructForSceneObject(
                allocation, nullptr, reinterpret_cast<int>(sceneObject));

        C_00833908_SetOwnedContext(sceneObject, nullptr, context);
        C_00833908_SetSceneState(sceneObject, nullptr, 2, 1);

        if (CSF_AT(uint32_t, options, 0x90) >= 0x10 &&
            optionString->field_0004)
        {
            Mem_FreeDebug(reinterpret_cast<void*>(optionString->field_0004));
        }
        CSF_AT(uint32_t, options, 0x90) = 15;
        CSF_AT(uint32_t, options, 0x8C) = 0;
        *reinterpret_cast<std::uint8_t*>(&optionString->field_0004) = 0;
        DestroySceneObjectOptionsStorage(options + 4, nullptr);
    }

    alignas(4) std::uint8_t resourceProperty[0x24] = {};
    CSF_AT(void**, resourceProperty, 0) = CSF_VTBL(0x0083ED20);
    CSF_AT(uint32_t, resourceProperty, 0x20) = 15;
    CSF_AT(uint32_t, resourceProperty, 0x1C) = 0;
    auto* resourceString =
        reinterpret_cast<C_0082d410*>(resourceProperty + 8);
    *reinterpret_cast<std::uint8_t*>(&resourceString->field_0004) = 0;
    CSF_AT(uint32_t, resourceProperty, 4) = 0;

    char* resourceName;
    if (CSF_AT(uint32_t, definition, 0x1B0) < 0x10)
        resourceName = static_cast<char*>(definition) + 0x19C;
    else
        resourceName = CSF_AT(char*, definition, 0x19C);

    char normalizedName[256] = {};
    NormalizeResourceName(resourceName, normalizedName, 256);
    AssignStringRange_429B60(
        resourceString, nullptr, normalizedName,
        static_cast<unsigned int>(std::strlen(normalizedName)));

    uint32_t* result = C_00833908_SetResourceProperty(
        sceneObject, nullptr, 15, reinterpret_cast<int>(resourceProperty));

    if (CSF_AT(uint32_t, resourceProperty, 0x20) >= 0x10)
    {
        result = reinterpret_cast<uint32_t*>(resourceString->field_0004);
        if (resourceString->field_0004)
            return reinterpret_cast<uint32_t*>(
                Mem_FreeDebug(reinterpret_cast<void*>(resourceString->field_0004)));
    }
    return result;
}

HOOK(0x004EE420, S_004e9090_SetupDoorSceneObject, "auto");
