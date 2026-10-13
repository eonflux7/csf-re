#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_00692b80_ParseIntegerComponent, 0x00692C40, (S_00692b80* self, void*));
CSF_FUNC(S_00692b80*, __fastcall, S_00692b80_FindProperty, 0x006932B0, (S_00692b80* self, void* , char* name));
CSF_FUNC(int, __stdcall, CGuiBuilder_ReadNonzeroFloatAttribute, 0x0071D9E0, (std::uint8_t* node, float* output));
CSF_FUNC(char, __stdcall, CGuiBuilder_ReadFourComponents, 0x0071DA70, (std::uint8_t* node, float* output));
CSF_FUNC(int, __fastcall, CGuiBuilder_CreateTextureQuad, 0x0071E4E0, (CGuiBuilder* self, void* , S_00692b80* properties));
CSF_FUNC(uint8_t*, __fastcall, CGuiBuilder_ReadControlProps, 0x0071EBD0, (CGuiBuilder* self, void* edx, void* control, uint8_t* node));
CSF_FUNC(int, __fastcall, CGuiBuilder_CreateText, 0x0071F910, (CGuiBuilder* self, void* , S_00692b80* properties));

CSF_GLOBAL(const char*, g_pszKey_TEXTO, 0x008A86BC);
CSF_GLOBAL(const char*, g_pszKey_EFECTOS, 0x008A86C0);
CSF_GLOBAL(const char*, g_pszKey_REPOSO, 0x008A86D4);
CSF_GLOBAL(const char*, g_pszKey_ACTIVO, 0x008A86D8);
CSF_GLOBAL(const char*, g_pszKey_PULSADO, 0x008A86DC);
CSF_GLOBAL(const char*, g_pszKey_PERMANENTE, 0x008A86E0);
CSF_GLOBAL(const char*, g_pszKey_COLOR_TEXTO_FOCUS, 0x008A8730);

static void __fastcall CGuiBuilder_ReadButtonProps(
    CGuiBuilder* self, void* /*edx*/, void* button, std::uint8_t* node)
{
    if (!button || !node)
        return;

    alignas(8) std::uint8_t colorBytes[0x10];
    float* focusColor = reinterpret_cast<float*>(colorBytes);
    focusColor[0] = 0.0f;
    focusColor[1] = 0.0f;
    focusColor[2] = 0.0f;
    focusColor[3] = 1.0f;

    alignas(8) std::uint8_t attributeBytes[0x4];
    float* attribute = reinterpret_cast<float*>(attributeBytes);

    ((void(__fastcall*)(void*, void*, std::uint32_t))
        CSF_AT(void**, button, 0)[368 / 4])(button, nullptr, self->field_0020);
    ((void(__fastcall*)(void*, void*, std::uint32_t))
        CSF_AT(void**, button, 0)[376 / 4])(button, nullptr, self->field_0024);
    ((void(__fastcall*)(void*, void*, std::uint32_t))
        CSF_AT(void**, button, 0)[380 / 4])(button, nullptr, self->field_0028);
    ((void(__fastcall*)(void*, void*, std::uint32_t))
        CSF_AT(void**, button, 0)[372 / 4])(button, nullptr, self->field_002c);
    ((void(__fastcall*)(void*, void*, std::uint32_t))
        CSF_AT(void**, button, 0)[636 / 4])(button, nullptr, self->field_0030);
    ((void(__fastcall*)(void*, void*, std::uint32_t))
        CSF_AT(void**, button, 0)[640 / 4])(button, nullptr, self->field_0034);

    CGuiBuilder_ReadControlProps(self, nullptr, button, node);

    S_00692b80* property = S_00692b80_FindProperty(
        reinterpret_cast<S_00692b80*>(node), nullptr, const_cast<char*>(g_pszKey_EFECTOS));
    if (property)
    {
        // The original captures this virtual target before parsing the value.
        auto setEffects = reinterpret_cast<void(__fastcall*)(void*, void*, bool)>(
            CSF_AT(void**, button, 0)[628 / 4]);
        bool enabled = S_00692b80_ParseIntegerComponent(property, nullptr) != 0;
        setEffects(button, nullptr, enabled);
    }

    property = S_00692b80_FindProperty(
        reinterpret_cast<S_00692b80*>(node), nullptr, const_cast<char*>(g_pszKey_TEXTO));
    if (property)
    {
        void* text = reinterpret_cast<void*>(CGuiBuilder_CreateText(self, nullptr, property));
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, text, 0)[168 / 4])(text, nullptr, 0);
        ((void(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, button, 0)[616 / 4])(button, nullptr, text);
    }

    property = S_00692b80_FindProperty(
        reinterpret_cast<S_00692b80*>(node), nullptr,
        const_cast<char*>(g_pszKey_COLOR_TEXTO_FOCUS));
    if (CGuiBuilder_ReadFourComponents(reinterpret_cast<std::uint8_t*>(property), focusColor))
    {
        ((void(__fastcall*)(void*, void*, float*))
            CSF_AT(void**, button, 0)[624 / 4])(button, nullptr, focusColor);
    }

    property = S_00692b80_FindProperty(
        reinterpret_cast<S_00692b80*>(node), nullptr, const_cast<char*>(g_pszKey_REPOSO));
    if (property)
    {
        void* texture = reinterpret_cast<void*>(
            CGuiBuilder_CreateTextureQuad(self, nullptr, property));
        ((void(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, button, 0)[580 / 4])(button, nullptr, texture);
        if (texture)
            ReleaseObjectReference(static_cast<std::uint8_t*>(texture) + 4, nullptr);

        *attribute = 0.0f;
        if (static_cast<std::uint8_t>(CGuiBuilder_ReadNonzeroFloatAttribute(
                reinterpret_cast<std::uint8_t*>(property), attribute)) != 0)
        {
            ((void(__fastcall*)(void*, void*, int, float))
                CSF_AT(void**, button, 0)[648 / 4])(button, nullptr, 0, *attribute);
        }
    }

    property = S_00692b80_FindProperty(
        reinterpret_cast<S_00692b80*>(node), nullptr, const_cast<char*>(g_pszKey_ACTIVO));
    if (property)
    {
        void* texture = reinterpret_cast<void*>(
            CGuiBuilder_CreateTextureQuad(self, nullptr, property));
        ((void(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, button, 0)[584 / 4])(button, nullptr, texture);
        if (texture)
            ReleaseObjectReference(static_cast<std::uint8_t*>(texture) + 4, nullptr);

        *attribute = 0.0f;
        if (static_cast<std::uint8_t>(CGuiBuilder_ReadNonzeroFloatAttribute(
                reinterpret_cast<std::uint8_t*>(property), attribute)) != 0)
        {
            ((void(__fastcall*)(void*, void*, int, float))
                CSF_AT(void**, button, 0)[648 / 4])(button, nullptr, 1, *attribute);
        }
    }

    property = S_00692b80_FindProperty(
        reinterpret_cast<S_00692b80*>(node), nullptr, const_cast<char*>(g_pszKey_PULSADO));
    if (property)
    {
        void* texture = reinterpret_cast<void*>(
            CGuiBuilder_CreateTextureQuad(self, nullptr, property));
        ((void(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, button, 0)[588 / 4])(button, nullptr, texture);
        if (texture)
            ReleaseObjectReference(static_cast<std::uint8_t*>(texture) + 4, nullptr);

        *attribute = 0.0f;
        if (static_cast<std::uint8_t>(CGuiBuilder_ReadNonzeroFloatAttribute(
                reinterpret_cast<std::uint8_t*>(property), attribute)) != 0)
        {
            ((void(__fastcall*)(void*, void*, int, float))
                CSF_AT(void**, button, 0)[648 / 4])(button, nullptr, 2, *attribute);
        }
    }

    property = S_00692b80_FindProperty(
        reinterpret_cast<S_00692b80*>(node), nullptr, const_cast<char*>(g_pszKey_PERMANENTE));
    if (property)
    {
        void* texture = reinterpret_cast<void*>(
            CGuiBuilder_CreateTextureQuad(self, nullptr, property));
        ((void(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, button, 0)[592 / 4])(button, nullptr, texture);
        if (texture)
            ReleaseObjectReference(static_cast<std::uint8_t*>(texture) + 4, nullptr);

        *attribute = 0.0f;
        if (static_cast<std::uint8_t>(CGuiBuilder_ReadNonzeroFloatAttribute(
                reinterpret_cast<std::uint8_t*>(property), attribute)) != 0)
        {
            ((void(__fastcall*)(void*, void*, float))
                CSF_AT(void**, button, 0)[652 / 4])(button, nullptr, *attribute);
        }
    }
}

HOOK(0x0071FCB0, CGuiBuilder_ReadButtonProps, "auto");
