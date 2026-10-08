#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, ReadTextControlProps, 0x00720420,
         (CGuiBuilder* self, void* edx, uint8_t* node, void* control));
CSF_FUNC(void*, __fastcall, CGuiBuilder_GetFont, 0x0071DB60,
         (CGuiBuilder* self, void* edx, uint8_t* node));
CSF_FUNC(int, __fastcall, ForwardLinkedObjectAndApplyValue, 0x00722DB0,
         (void* self, void* edx, void* font));
CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0,
         (void* self, void* edx));
CSF_FUNC(uint8_t*, __fastcall, FindProperty, 0x006932B0,
         (uint8_t* node, void* edx, char* keyword));
CSF_FUNC(void*, __fastcall, CGuiBuilder_CreateText, 0x0071F910,
         (CGuiBuilder* self, void* edx, uint8_t* node));
CSF_FUNC(int, __fastcall, ReplaceOwnedObject, 0x00722D70,
         (void* self, void* edx, void* text));
CSF_FUNC(int, __fastcall, SetFourDwordsAt1F8, 0x00722E10,
         (void* self, void* edx, void* components));
CSF_FUNC(char, __stdcall, CGuiBuilder_ReadFourComponents, 0x0071DA70,
         (uint8_t* node, float* components));

CSF_GLOBAL(char*, g_TextKeyword, 0x008A86BC);
CSF_GLOBAL(char*, g_PrimaryComponentsKeyword, 0x008A866C);
CSF_GLOBAL(char*, g_SecondaryComponentsKeyword, 0x008A8700);

static void __fastcall CGuiBuilder_ApplyTextAndComponentProps(
    CGuiBuilder* self, void* /*edx*/, uint8_t* node, void* control)
{
    if (!node || !control)
        return;

    float components[4] = {0.0f, 0.0f, 0.0f, 1.0f};

    ReadTextControlProps(self, nullptr, node, control);

    void* font = CGuiBuilder_GetFont(self, nullptr, node);
    if (font)
    {
        ForwardLinkedObjectAndApplyValue(control, nullptr, font);
        ReleaseObjectReference(font, nullptr);
    }

    uint8_t* attribute = FindProperty(node, nullptr, g_TextKeyword);
    if (attribute)
    {
        void* text = CGuiBuilder_CreateText(self, nullptr, attribute);
        if (text)
        {
            ReplaceOwnedObject(control, nullptr, text);
            void* textComponents =
                reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, text, 0)[0x64 / 4])(text, nullptr);
            SetFourDwordsAt1F8(control, nullptr, textComponents);
        }
    }

    attribute = FindProperty(node, nullptr, g_PrimaryComponentsKeyword);
    if (CGuiBuilder_ReadFourComponents(attribute, components))
        SetFourDwordsAt1F8(control, nullptr, components);

    attribute = FindProperty(node, nullptr, g_SecondaryComponentsKeyword);
    if (CGuiBuilder_ReadFourComponents(attribute, components))
    {
        uint32_t first = CSF_AT(uint32_t, components, 0);
        uint32_t second = CSF_AT(uint32_t, components, 4);
        CSF_AT(uint32_t, control, 0x208) = first;
        uint32_t third = CSF_AT(uint32_t, components, 8);
        CSF_AT(uint32_t, control, 0x20C) = second;
        uint32_t fourth = CSF_AT(uint32_t, components, 0x0C);
        CSF_AT(uint32_t, control, 0x210) = third;
        CSF_AT(uint32_t, control, 0x214) = fourth;
    }
}

HOOK(0x007207E0, CGuiBuilder_ApplyTextAndComponentProps, "auto");
