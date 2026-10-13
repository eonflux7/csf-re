#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, SceneObjectOptions_ReleaseAndResetStorage, 0x004018E0, (void* self, void*));
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));
CSF_FUNC(unsigned int, __fastcall, WeaponFirearms_DecodeColor, 0x00476430, (float* self, void* , unsigned int color));
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(void, __fastcall, ForwardToChildIfUnhandled, 0x006C8640, (void* self, void* , int a2));
CSF_FUNC(C_00860908*, __fastcall, C_00860908_InitDefaults, 0x006C8670, (C_00860908* self, void*));
CSF_FUNC(int, __fastcall, InitializeSceneObjectResources, 0x006C89E0, (void* self, void* , void* definition));

CSF_GLOBAL(std::uint8_t, g_byte_82E65C, 0x0082E65C);

static int __fastcall WeaponFirearms_InitializeSceneObject(void* self, void* /*edx*/)
{
    auto* effectName =
        static_cast<std::uint8_t*>(CSF_AT(void*, self, 0x40)) + 0xA4;

    CSF_AT(C_00860908*, self, 0x88) = nullptr;
    const int comparison = CompareStringSubstring(
        effectName, nullptr, 0, CSF_AT(unsigned int, effectName, 0x14),
        reinterpret_cast<const char*>(&g_byte_82E65C), 0);
    if (!comparison)
        return comparison;

    auto* allocation =
        reinterpret_cast<C_00860908*>(CoreMemory_AllocUntagged(52));
    C_00860908* sceneObject =
        allocation ? C_00860908_InitDefaults(allocation, nullptr) : nullptr;

    void* owner = CSF_AT(void*, self, 0x04);
    CSF_AT(C_00860908*, self, 0x88) = sceneObject;

    alignas(8) std::uint8_t optionsBytes[0x5C];
    auto& resourceName =
        *reinterpret_cast<C_0082d410*>(optionsBytes + 0x04);
    auto& sceneName =
        *reinterpret_cast<C_0082d410*>(optionsBytes + 0x20);

    CSF_AT(unsigned int, &resourceName, 0x18) = 15;
    CSF_AT(unsigned int, &sceneName, 0x18) = 15;

    auto* definition =
        static_cast<std::uint8_t*>(CSF_AT(void*, self, 0x40));

    CSF_AT(int, optionsBytes, 0x00) = 0;
    CSF_AT(bool, optionsBytes, 0x50) = false;
    CSF_AT(unsigned int, &resourceName, 0x14) = 0;
    *reinterpret_cast<std::uint8_t*>(&resourceName.field_0004) = 0;
    CSF_AT(unsigned int, &sceneName, 0x14) = 0;
    *reinterpret_cast<std::uint8_t*>(&sceneName.field_0004) = 0;
    CSF_AT(float, optionsBytes, 0x3C) = 0.0f;
    CSF_AT(float, optionsBytes, 0x40) = 0.0f;
    CSF_AT(float, optionsBytes, 0x44) = 0.0f;
    CSF_AT(float, optionsBytes, 0x48) = 1.0f;
    CSF_AT(int, optionsBytes, 0x4C) = 0;
    CSF_AT(float, optionsBytes, 0x58) = 15.0f;
    CSF_AT(float, optionsBytes, 0x54) = 15.0f;

    CSF_AT(bool, optionsBytes, 0x50) = CSF_AT(int, owner, 0x50) == 0;
    CSF_AT(int, optionsBytes, 0x00) = CSF_AT(int, definition, 0xC4);

    AssignStringSubstring(&resourceName, nullptr, definition + 0x88, 0, -1);
    AssignStringSubstring(
        &sceneName, nullptr,
        static_cast<std::uint8_t*>(CSF_AT(void*, self, 0x40)) + 0xA4,
        0, -1);

    definition = static_cast<std::uint8_t*>(CSF_AT(void*, self, 0x40));
    CSF_AT(std::uint32_t, optionsBytes, 0x4C) =
        CSF_AT(std::uint32_t, definition, 0xC8);
    WeaponFirearms_DecodeColor(
        reinterpret_cast<float*>(optionsBytes + 0x3C), nullptr,
        CSF_AT(unsigned int, definition, 0xC0));
    CSF_AT(std::uint32_t, optionsBytes, 0x54) =
        CSF_AT(std::uint32_t, definition, 0xCC);
    CSF_AT(std::uint32_t, optionsBytes, 0x58) =
        CSF_AT(std::uint32_t, definition, 0xD0);

    InitializeSceneObjectResources(
        CSF_AT(void*, self, 0x88), nullptr, optionsBytes);

    void* attachmentContainer = CSF_AT(void*, self, 0x78);
    if (attachmentContainer)
    {
        const int index =
            reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, attachmentContainer, 0)[0x98 / 4])(
                    attachmentContainer, nullptr, 100);
        if (index != -1)
        {
            attachmentContainer = CSF_AT(void*, self, 0x78);
            void* attachment =
                reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, attachmentContainer, 0)[0x8C / 4])(
                        attachmentContainer, nullptr, index);
            const int child =
                reinterpret_cast<int(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, attachment, 0)[0x18 / 4])(
                        attachment, nullptr);
            ForwardToChildIfUnhandled(
                CSF_AT(void*, self, 0x88), nullptr, child);
        }
    }

    return static_cast<int>(
        SceneObjectOptions_ReleaseAndResetStorage(optionsBytes, nullptr));
}

HOOK(0x0047A520, WeaponFirearms_InitializeSceneObject, "auto");
