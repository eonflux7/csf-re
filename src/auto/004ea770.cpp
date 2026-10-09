#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, DestroySceneObjectOptionsStorage, 0x004018E0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(uint32_t*, __fastcall, InitializeSceneObjectOptions, 0x004777D0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, SceneObject_SetAttachment, 0x0048E2B0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(unsigned int, __fastcall, C_00833908_SetDoorStateFlags, 0x00490D20, (C_00833908* self, void* edx, uint32_t a2, char a3));
CSF_FUNC(uint32_t, __fastcall, SceneObject_SetSetupMode, 0x00490D70, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SceneObject_SetSetupValue, 0x00490E70, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, SceneObject_SetSetupParameters, 0x00490F60, (void* self, void* edx, int a2, float a3, int a4, int a5));
CSF_FUNC(int, __fastcall, SceneObject_SetSetupOption, 0x00490FE0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, SceneObject_FinalizeSetupOption, 0x00491050, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SceneObject_CreateController, 0x004A66C0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, MissionSetup_CreateSceneObject, 0x004E9B80, (void* self, void* edx, int a2, char a3));
CSF_FUNC(void, __fastcall, SceneObject_SetController, 0x0055D860, (void* self, void* edx, void* a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

static unsigned int __fastcall S_004e9090_ConfigureMissionSceneObject(
    S_004e9090* self, void* /*edx*/, C_00833908* object,
    int definitionIndex, char alternateMode)
{
    void* definitions = reinterpret_cast<void*>(self->field_1fc8);
    void* definition =
        reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, definitions, 0)[8 / 4])(
                definitions, nullptr, definitionIndex);

    int sceneObjectAddress = MissionSetup_CreateSceneObject(
        self, nullptr, definitionIndex, alternateMode);
    void* sceneObject = reinterpret_cast<void*>(sceneObjectAddress);
    C_00833908_SetDoorStateFlags(object, nullptr, sceneObjectAddress, 1);

    struct OptionsStorage {
        uint32_t flags;
        alignas(4) std::uint8_t storage[0x74];
        C_0082d410 text;
        std::uint8_t trailing[0x40 - sizeof(C_0082d410)];
    } options;

    InitializeSceneObjectOptions(&options, nullptr);
    options.flags |= 5u;
    if (CSF_AT(std::uint8_t, definition, 0x159))
        options.flags |= 0x400;
    else
        options.flags &= 0xFFFFFBFF;

    if (CSF_AT(std::uint8_t, definition, 0x15A))
        options.flags &= 0xFFFFFFDF;
    else
        options.flags |= 0x20;

    if (CSF_AT(std::uint8_t, definition, 0x15B))
        options.flags |= 0x4000;
    else
        options.flags &= 0xFFFFBFFF;

    CSF_AT(uint32_t, options.storage, 0x64) =
        CSF_AT(uint32_t, definition, 0xCC);
    CSF_AT(uint32_t, options.storage, 0x68) =
        CSF_AT(uint32_t, definition, 0x160);
    CSF_AT(uint32_t, options.storage, 0x6C) =
        CSF_AT(uint32_t, definition, 0x164);
    CSF_AT(uint32_t, options.storage, 0x70) =
        CSF_AT(uint32_t, definition, 0x168);

    AssignStringSubstring(
        &options.text, nullptr,
        static_cast<std::uint8_t*>(definition) + 0x16C, 0, -1);

    CSF_AT(uint32_t, &options.text, 0x1C) =
        CSF_AT(uint32_t, definition, 0x188) - 1;
    CSF_AT(uint32_t, &options.text, 0x20) =
        CSF_AT(uint32_t, definition, 0x18C);
    CSF_AT(uint32_t, &options.text, 0x24) =
        CSF_AT(uint32_t, definition, 0x190);
    CSF_AT(uint32_t, &options.text, 0x28) =
        CSF_AT(uint32_t, definition, 0x194);
    CSF_AT(uint32_t, &options.text, 0x2C) =
        CSF_AT(uint32_t, definition, 0xD0);

    reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, sceneObject, 0)[0x10 / 4])(
            sceneObject, nullptr, &options);

    object->field_0078 = CSF_AT(int, definition, 0xB4) == 1;
    if (alternateMode) {
        SceneObject_SetSetupMode(object, nullptr, 3);
        SceneObject_SetSetupValue(
            object, nullptr, 4, CSF_AT(int, definition, 0xDC));
    } else {
        SceneObject_SetSetupMode(object, nullptr, 1);
        SceneObject_SetSetupValue(
            object, nullptr, 0, CSF_AT(int, definition, 0xDC));
        SceneObject_SetSetupParameters(
            object, nullptr, 0, CSF_AT(float, definition, 0x120),
            CSF_AT(int, definition, 0x124),
            CSF_AT(int, definition, 0x128));
        SceneObject_SetSetupOption(object, nullptr, 0);
    }
    SceneObject_FinalizeSetupOption(object, nullptr, 0);

    void* allocation = Mem_Alloc(132);
    void* controller = nullptr;
    if (allocation) {
        controller = reinterpret_cast<void*>(
            SceneObject_CreateController(
                allocation, nullptr, reinterpret_cast<int>(object)));
    }
    SceneObject_SetController(object, nullptr, controller);

    struct AttachmentDescriptor {
        void* table;
        uint32_t value;
    } attachment;
    attachment.table = reinterpret_cast<void*>(CSF_ADDR_DATA(0x0083ED0C));
    attachment.value = 0;
    SceneObject_SetAttachment(
        object, nullptr, 0, reinterpret_cast<int>(&attachment));

    if (CSF_AT(uint32_t, &options.text, 0x18) >= 0x10 &&
        options.text.field_0004) {
        Mem_FreeDebug(reinterpret_cast<void*>(options.text.field_0004));
    }
    CSF_AT(uint32_t, &options.text, 0x18) = 15;
    CSF_AT(uint32_t, &options.text, 0x14) = 0;
    reinterpret_cast<std::uint8_t*>(&options.text.field_0004)[0] = 0;
    return DestroySceneObjectOptionsStorage(options.storage, nullptr);
}

HOOK(0x004EA770, S_004e9090_ConfigureMissionSceneObject, "auto");
