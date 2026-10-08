#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, AllocateSwitchMemory, 0x006910F0,
         (std::size_t size));
CSF_FUNC(std::uint32_t*, __fastcall, ConstructSwitch, 0x007302A0,
         (void* self, void* edx, std::uint32_t context, void* parameters));
CSF_FUNC(std::uint8_t*, __fastcall, FindControlProperty, 0x006932B0,
         (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(std::uint8_t*, __fastcall, ReadControlProperties, 0x0071EBD0,
         (CGuiBuilder* self, void* edx, std::uint32_t* control,
          std::uint8_t* properties));
CSF_FUNC(void*, __fastcall, CreateTextControl, 0x0071F910,
         (CGuiBuilder* self, void* edx, std::uint8_t* properties));
CSF_FUNC(void, __fastcall, ReplaceAttachedObjectAndCacheParameters, 0x00730120,
         (std::uint32_t* self, void* edx, void* attachedObject));
CSF_FUNC(char, __stdcall, ReadFourComponents, 0x0071DA70,
         (std::uint8_t* property, float* components));
CSF_FUNC(void*, __fastcall, CreateTextureQuad, 0x0071E4E0,
         (CGuiBuilder* self, void* edx, std::uint8_t* properties));
CSF_FUNC(int, __fastcall, ReleaseTextureReference, 0x006923C0,
         (void* self, void* edx));

CSF_GLOBAL(char*, g_SwitchTextKeyword, 0x008A86BC);
CSF_GLOBAL(char*, g_SwitchComponentsKeyword, 0x008A8734);
CSF_GLOBAL(char*, g_SwitchTextureKeyword1, 0x008A86CC);
CSF_GLOBAL(char*, g_SwitchTextureKeyword2, 0x008A86D0);
CSF_GLOBAL(char*, g_SwitchTextureKeyword3, 0x008A86D8);
CSF_GLOBAL(char*, g_SwitchTextureKeyword4, 0x008A86E0);

static std::uint32_t* __fastcall CGuiBuilder_CreateSwitch(
    CGuiBuilder* self, void* /*edx*/, std::uint8_t* properties)
{
    void* memory = AllocateSwitchMemory(0x180);
    std::uint32_t* control = nullptr;
    if (memory)
    {
        control = ConstructSwitch(
            memory, nullptr, self->field_0004,
            reinterpret_cast<std::uint8_t*>(self) + 0x0C);
    }

    const std::uint32_t controlContext = self->field_0020;
    ((void(__fastcall*)(void*, void*, std::uint32_t))
        CSF_AT(void**, control, 0)[0x170 / 4])(
            control, nullptr, controlContext);

    ReadControlProperties(self, nullptr, control, properties);

    std::uint8_t* property =
        FindControlProperty(properties, nullptr, g_SwitchTextKeyword);
    if (property)
    {
        void* text = CreateTextControl(self, nullptr, property);
        ReplaceAttachedObjectAndCacheParameters(control, nullptr, text);
    }

    float components[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    property =
        FindControlProperty(properties, nullptr, g_SwitchComponentsKeyword);
    if (ReadFourComponents(property, components))
    {
        CSF_AT(float, control, 0x16C) = components[0];
        CSF_AT(float, control, 0x170) = components[1];
        CSF_AT(float, control, 0x174) = components[2];
        CSF_AT(float, control, 0x178) = components[3];
    }

    property = FindControlProperty(properties, nullptr, g_SwitchTextureKeyword1);
    if (property)
    {
        void* texture = CreateTextureQuad(self, nullptr, property);
        ((void(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, control, 0)[0x240 / 4])(
                control, nullptr, texture);
        if (texture)
            ReleaseTextureReference(
                reinterpret_cast<std::uint8_t*>(texture) + 4, nullptr);
    }

    property = FindControlProperty(properties, nullptr, g_SwitchTextureKeyword2);
    if (property)
    {
        void* texture = CreateTextureQuad(self, nullptr, property);
        ((void(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, control, 0)[0x244 / 4])(
                control, nullptr, texture);
        if (texture)
            ReleaseTextureReference(
                reinterpret_cast<std::uint8_t*>(texture) + 4, nullptr);
    }

    property = FindControlProperty(properties, nullptr, g_SwitchTextureKeyword3);
    if (property)
    {
        void* texture = CreateTextureQuad(self, nullptr, property);
        ((void(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, control, 0)[0x248 / 4])(
                control, nullptr, texture);
        if (texture)
            ReleaseTextureReference(
                reinterpret_cast<std::uint8_t*>(texture) + 4, nullptr);
    }

    property = FindControlProperty(properties, nullptr, g_SwitchTextureKeyword4);
    if (property)
    {
        void* texture = CreateTextureQuad(self, nullptr, property);
        ((void(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, control, 0)[0x24C / 4])(
                control, nullptr, texture);
        if (texture)
            ReleaseTextureReference(
                reinterpret_cast<std::uint8_t*>(texture) + 4, nullptr);
    }

    return control;
}

HOOK(0x007200F0, CGuiBuilder_CreateSwitch, "auto");
