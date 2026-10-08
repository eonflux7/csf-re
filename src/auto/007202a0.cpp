#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, AllocateMemory, 0x006910F0, (size_t size));
CSF_FUNC(void*, __fastcall, ConstructStatic, 0x00720E10,
         (void* self, void* edx, uint32_t resource, void* context));
CSF_FUNC(uint8_t*, __fastcall, ReadControlProps, 0x0071EBD0,
         (CGuiBuilder* self, void* edx, void* control, uint8_t* node));
CSF_FUNC(uint8_t*, __fastcall, FindAttribute, 0x006932B0,
         (uint8_t* node, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, ReadIntegerValue, 0x00692C40, (uint8_t* node));
CSF_FUNC(void*, __fastcall, CreateText, 0x0071F910,
         (CGuiBuilder* self, void* edx, uint8_t* node));
CSF_FUNC(void, __fastcall, ReplaceDurationSource, 0x00720AF0,
         (void* self, void* edx, void* source));
CSF_FUNC(void*, __fastcall, CreateColorQuad, 0x0071E370,
         (CGuiBuilder* self, void* edx, uint8_t* node));

CSF_GLOBAL(char*, g_StaticFlagKeyword, 0x008A8744);
CSF_GLOBAL(char*, g_TextKeyword, 0x008A86BC);
CSF_GLOBAL(char*, g_ColorQuadKeyword, 0x008A8704);

static void* __fastcall CGuiBuilder_CreateStatic(
    CGuiBuilder* self, void* /*edx*/, uint8_t* node)
{
    void* storage = AllocateMemory(0x160);
    void* control = nullptr;
    if (storage)
        control = ConstructStatic(
            storage, nullptr, self->field_0004,
            reinterpret_cast<uint8_t*>(self) + 0x0C);

    ReadControlProps(self, nullptr, control, node);

    uint32_t property = self->field_0020;
    reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t)>(
        CSF_AT(void**, control, 0)[0x170 / 4])(control, nullptr, property);

    uint8_t* attribute = FindAttribute(node, nullptr, g_StaticFlagKeyword);
    if (attribute)
        CSF_AT(uint8_t, control, 0x154) =
            static_cast<uint8_t>(ReadIntegerValue(attribute) != 0);

    attribute = FindAttribute(node, nullptr, g_TextKeyword);
    if (attribute)
    {
        void* text = CreateText(self, nullptr, attribute);
        ReplaceDurationSource(control, nullptr, text);
    }

    attribute = FindAttribute(node, nullptr, g_ColorQuadKeyword);
    if (attribute)
    {
        void** controlVtable = CSF_AT(void**, control, 0);
        void* quad = CreateColorQuad(self, nullptr, attribute);
        reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
            controlVtable[0x1F4 / 4])(control, nullptr, quad);
    }

    return control;
}

HOOK(0x007202A0, CGuiBuilder_CreateStatic, "auto");
