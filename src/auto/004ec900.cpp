#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, DestroySceneObjectOptionsStorage, 0x004018E0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(uint32_t*, __fastcall, InitializeSceneObjectOptions, 0x004777D0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_00833908_ApplySelectionDescriptor, 0x0048E2B0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(unsigned int, __fastcall, C_00833908_SetDoorStateFlags, 0x00490D20, (C_00833908* self, void* edx, uint32_t a2, char a3));
CSF_FUNC(int, __fastcall, C_00833908_SetInteractionParameter, 0x00490E70, (void* self, void* edx, int a2, int a3));
CSF_FUNC(void, __fastcall, S_004e9090_CompleteObjectOptions, 0x004E9090, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, S_004e9090_GetSceneObject, 0x004E9B80, (void* self, void* edx, int a2, char a3));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static unsigned int __fastcall S_004e9090_ConfigureMissionObject(
    S_004e9090* self, void* /*edx*/, C_00833908* object,
    int objectIndex, int /*unused*/)
{
    void* definitions = reinterpret_cast<void*>(self->field_1fc8);
    auto* definition = reinterpret_cast<std::uint8_t*>(
        ((int (__fastcall*)(void*, void*, int))
            CSF_AT(void**, definitions, 0)[2])(
                definitions, nullptr, objectIndex));

    void* sceneObject = reinterpret_cast<void*>(
        S_004e9090_GetSceneObject(self, nullptr, objectIndex, 0));
    C_00833908_SetDoorStateFlags(
        object, nullptr, reinterpret_cast<uint32_t>(sceneObject), 1);

    // Contiguous options record, including its embedded string storage.
    alignas(4) std::uint8_t options[0xB8]{};
    InitializeSceneObjectOptions(options, nullptr);

    if (CSF_AT(std::uint8_t, definition, 0x159))
        CSF_AT(uint32_t, options, 0) |= 0x400;
    else
        CSF_AT(uint32_t, options, 0) &= 0xFFFFFBFF;

    if (CSF_AT(std::uint8_t, definition, 0x15A))
        CSF_AT(uint32_t, options, 0) &= 0xFFFFFFDF;
    else
        CSF_AT(uint32_t, options, 0) |= 0x20;

    AssignStringSubstring(options + 0x08, nullptr, definition + 0x1F4, 0, -1);
    AssignStringSubstring(options + 0x24, nullptr, definition + 0x3C, 0, -1);

    CSF_AT(int, options, 0x58) = CSF_AT(int, definition, 0x220);
    CSF_AT(int, options, 0x5C) = CSF_AT(int, definition, 0x224);
    CSF_AT(int, options, 0x60) = CSF_AT(int, definition, 0x228);

    if (CSF_AT(std::uint8_t,
               reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0))
        CSF_AT(int, options, 0x04) = 0;
    else
        CSF_AT(int, options, 0x04) = CSF_AT(int, definition, 0x1F0);

    CSF_AT(std::uint8_t, options, 0x66) = 0;
    CSF_AT(std::uint8_t, options, 0x64) =
        CSF_AT(std::uint8_t, definition, 0x21C);
    CSF_AT(std::uint8_t, options, 0x65) =
        CSF_AT(std::uint8_t, definition, 0x21D);

    if (CSF_AT(std::uint8_t, definition, 0x15B))
        CSF_AT(uint32_t, options, 0) |= 0x4000;
    else
        CSF_AT(uint32_t, options, 0) &= 0xFFFFBFFF;
    CSF_AT(uint32_t, options, 0) |= 0x8000;

    CSF_AT(int, options, 0x68) = CSF_AT(int, definition, 0xCC);
    CSF_AT(int, options, 0x6C) = CSF_AT(int, definition, 0x160);
    CSF_AT(int, options, 0x70) = CSF_AT(int, definition, 0x164);
    CSF_AT(int, options, 0x74) = CSF_AT(int, definition, 0x168);

    AssignStringSubstring(options + 0x78, nullptr, definition + 0x16C, 0, -1);
    CSF_AT(uint32_t, options, 0x94) =
        CSF_AT(uint32_t, definition, 0x188) - 1;
    CSF_AT(int, options, 0x98) = CSF_AT(int, definition, 0x18C);
    CSF_AT(int, options, 0x9C) = CSF_AT(int, definition, 0x190);
    CSF_AT(int, options, 0xA0) = CSF_AT(int, definition, 0x194);
    CSF_AT(int, options, 0xA4) = CSF_AT(int, definition, 0xD0);

    S_004e9090_CompleteObjectOptions(
        self, nullptr, CSF_AT(int, definition, 0),
        reinterpret_cast<int>(options));

    ((void (__fastcall*)(void*, void*, void*))
        CSF_AT(void**, sceneObject, 0)[4])(
            sceneObject, nullptr, options);

    if (((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, sceneObject, 0)[30])(sceneObject, nullptr))
    {
        void* attachedObject = ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, sceneObject, 0)[30])(sceneObject, nullptr);
        ((void (__fastcall*)(void*, void*, C_00833908*))
            CSF_AT(void**, attachedObject, 0)[26])(
                attachedObject, nullptr, object);
    }

    if (CSF_AT(int, definition, 0xB4) == 17)
        C_00833908_SetInteractionParameter(
            object, nullptr, 7, CSF_AT(int, definition, 0xDC));

    object->field_0078 = -1;

    alignas(4) std::uint8_t normalDescriptor[0x8]{};
    alignas(4) std::uint8_t timedDescriptor[0xC]{};
    CSF_AT(void*, normalDescriptor, 0) =
        reinterpret_cast<void*>(CSF_ADDR_DATA(0x0083ED14));
    CSF_AT(int, normalDescriptor, 4) = 0;
    CSF_AT(int, timedDescriptor, 4) = 0;
    CSF_AT(void*, timedDescriptor, 0) =
        reinterpret_cast<void*>(CSF_ADDR_DATA(0x0083ED18));
    CSF_AT(float, timedDescriptor, 8) = 0.2f;

    const bool useTimedDescriptor = CSF_AT(std::uint8_t,
        reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0) != 0;

    switch (CSF_AT(int, definition, 0xB4))
    {
    case 8:
    case 0xE:
    case 0xF:
        if (useTimedDescriptor)
            CSF_AT(int, timedDescriptor, 4) = 1;
        else
            CSF_AT(int, normalDescriptor, 4) = 1;
        break;
    case 9:
        if (useTimedDescriptor)
            CSF_AT(int, timedDescriptor, 4) = 2;
        else
            CSF_AT(int, normalDescriptor, 4) = 2;
        break;
    case 0xA:
    case 0x10:
        if (useTimedDescriptor)
            CSF_AT(int, timedDescriptor, 4) = 3;
        else
            CSF_AT(int, normalDescriptor, 4) = 3;
        break;
    case 0xB:
        if (useTimedDescriptor)
            CSF_AT(int, timedDescriptor, 4) = 4;
        else
            CSF_AT(int, normalDescriptor, 4) = 4;
        break;
    case 0x11:
    case 0x14:
    case 0x15:
        if (useTimedDescriptor)
            CSF_AT(int, timedDescriptor, 4) = 5;
        else
            CSF_AT(int, normalDescriptor, 4) = 5;
        break;
    default:
        break;
    }

    if (useTimedDescriptor)
    {
        CSF_AT(float, timedDescriptor, 8) = 0.2f;
        C_00833908_ApplySelectionDescriptor(
            object, nullptr, (useTimedDescriptor != 0) + 2,
            reinterpret_cast<int>(timedDescriptor));
    }
    else
    {
        C_00833908_ApplySelectionDescriptor(
            object, nullptr, 2, reinterpret_cast<int>(normalDescriptor));
    }

    if (CSF_AT(uint32_t, options, 0x90) >= 0x10 &&
        CSF_AT(void*, options, 0x7C))
    {
        Mem_FreeDebug(CSF_AT(void*, options, 0x7C));
    }
    CSF_AT(uint32_t, options, 0x90) = 15;
    CSF_AT(int, options, 0x8C) = 0;
    CSF_AT(std::uint8_t, options, 0x7C) = 0;
    return DestroySceneObjectOptionsStorage(options + 0x04, nullptr);
}

HOOK(0x004EC900, S_004e9090_ConfigureMissionObject, "auto");
