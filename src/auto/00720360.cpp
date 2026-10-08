#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, AllocateMemory, 0x006910F0, (size_t size));
CSF_FUNC(void*, __fastcall, ConstructEdit, 0x007214F0,
         (void* self, void* edx, uint32_t resource, void* context));
CSF_FUNC(uint8_t*, __fastcall, ReadControlProps, 0x0071EBD0,
         (CGuiBuilder* self, void* edx, void* control, uint8_t* node));
CSF_FUNC(uint8_t*, __fastcall, FindAttribute, 0x006932B0,
         (uint8_t* node, void* edx, char* keyword));
CSF_FUNC(uint8_t*, __fastcall, FindChildNode, 0x00693360,
         (uint8_t* node, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970,
         (uint8_t* node, void* edx, char* keyword));
CSF_FUNC(double, __fastcall, ReadNumericAttribute, 0x00693990,
         (uint8_t* node, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, SetEditIntegerProperty, 0x0044F890,
         (void* self, void* edx, int value));
CSF_FUNC(void*, __fastcall, CreateText, 0x0071F910,
         (CGuiBuilder* self, void* edx, uint8_t* node));
CSF_FUNC(void, __fastcall, SetEditText, 0x00722B80,
         (void* self, void* edx, void* text));

CSF_GLOBAL(char*, g_EditNumericKeyword, 0x008A86B8);
CSF_GLOBAL(char*, g_EditIntegerKeyword, 0x008A874C);
CSF_GLOBAL(char*, g_TextKeyword, 0x008A86BC);

static void* __fastcall CGuiBuilder_CreateEdit(
    CGuiBuilder* self, void* /*edx*/, uint8_t* node)
{
    void* storage = AllocateMemory(0x1C8);
    void* control = nullptr;
    if (storage)
        control = ConstructEdit(
            storage, nullptr, self->field_0004,
            reinterpret_cast<uint8_t*>(self) + 0x0C);

    ReadControlProps(self, nullptr, control, node);

    if (FindAttribute(node, nullptr, g_EditNumericKeyword))
    {
        char* keyword = g_EditNumericKeyword;
        void** controlVtable = CSF_AT(void**, control, 0);
        float value = static_cast<float>(
            ReadNumericAttribute(node, nullptr, keyword));
        reinterpret_cast<void (__fastcall*)(void*, void*, float)>(
            controlVtable[0x24C / 4])(control, nullptr, value);
    }

    if (FindAttribute(node, nullptr, g_EditIntegerKeyword))
    {
        int value = ReadIntegerAttribute(node, nullptr, g_EditIntegerKeyword);
        SetEditIntegerProperty(control, nullptr, value);
    }

    uint8_t* textNode = FindChildNode(node, nullptr, g_TextKeyword);
    void* text = CreateText(self, nullptr, textNode);
    SetEditText(control, nullptr, text);

    return control;
}

HOOK(0x00720360, CGuiBuilder_CreateEdit, "auto");
