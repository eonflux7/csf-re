#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (size_t size));
CSF_FUNC(void*, __fastcall, CGuiEdit_Construct, 0x007214F0,
         (void* self, void* edx, uint32_t resource, void* context));
CSF_FUNC(uint8_t*, __fastcall, CGuiBuilder_ReadControlProps, 0x0071EBD0,
         (CGuiBuilder* self, void* edx, void* control, uint8_t* node));
CSF_FUNC(uint8_t*, __fastcall, FindProperty, 0x006932B0,
         (uint8_t* node, void* edx, char* keyword));
CSF_FUNC(uint8_t*, __fastcall, LookupNamedElement, 0x00693360,
         (uint8_t* node, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970,
         (uint8_t* node, void* edx, char* keyword));
CSF_FUNC(double, __fastcall, ReadFloatAttribute, 0x00693990,
         (uint8_t* node, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, SetEditIntegerProperty, 0x0044F890,
         (void* self, void* edx, int value));
CSF_FUNC(void*, __fastcall, CGuiBuilder_CreateText, 0x0071F910,
         (CGuiBuilder* self, void* edx, uint8_t* node));
CSF_FUNC(void, __fastcall, ReplaceAttachedObject, 0x00722B80,
         (void* self, void* edx, void* text));

CSF_GLOBAL(char*, g_EditNumericKeyword, 0x008A86B8);
CSF_GLOBAL(char*, g_EditIntegerKeyword, 0x008A874C);
CSF_GLOBAL(char*, g_TextKeyword, 0x008A86BC);

static void* __fastcall CGuiBuilder_CreateEdit(
    CGuiBuilder* self, void* /*edx*/, uint8_t* node)
{
    void* storage = Mem_Alloc(0x1C8);
    void* control = nullptr;
    if (storage)
        control = CGuiEdit_Construct(
            storage, nullptr, self->field_0004,
            reinterpret_cast<uint8_t*>(self) + 0x0C);

    CGuiBuilder_ReadControlProps(self, nullptr, control, node);

    if (FindProperty(node, nullptr, g_EditNumericKeyword))
    {
        char* keyword = g_EditNumericKeyword;
        void** controlVtable = CSF_AT(void**, control, 0);
        float value = static_cast<float>(
            ReadFloatAttribute(node, nullptr, keyword));
        reinterpret_cast<void (__fastcall*)(void*, void*, float)>(
            controlVtable[0x24C / 4])(control, nullptr, value);
    }

    if (FindProperty(node, nullptr, g_EditIntegerKeyword))
    {
        int value = ReadIntegerAttribute(node, nullptr, g_EditIntegerKeyword);
        SetEditIntegerProperty(control, nullptr, value);
    }

    uint8_t* textNode = LookupNamedElement(node, nullptr, g_TextKeyword);
    void* text = CGuiBuilder_CreateText(self, nullptr, textNode);
    ReplaceAttachedObject(control, nullptr, text);

    return control;
}

HOOK(0x00720360, CGuiBuilder_CreateEdit, "auto");
